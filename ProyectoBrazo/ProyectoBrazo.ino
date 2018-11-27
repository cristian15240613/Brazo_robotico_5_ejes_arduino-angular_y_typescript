// -- LIBRERIAS --
#include <Wire.h>               // Librería wire para comunicación de arduino con dispositivos I2C
#include <LiquidCrystal_I2C.h>  // Librería para controlar el LCD con el dispositivo I2C
#include <EEPROM.h>             // Librería para guardar en la memoria EEPROM de Arduino UNO
#include <Servo.h>              // Librería para controlar servomotores
#include <SoftwareSerial.h>     // Libreria para el BT

// 050000000065000065000000000090

// -- Blue
SoftwareSerial BT(0,1);            // (RX, TX)

// -- Botónes --
#define BOTONEMERGENCIA 2       // Pin para el botón de emergencia

//-- Definición de LEDs --
#define LEDEMERGENCIA 3         // Pin para el led rojo de emergencia
#define LEDOK 4                 // Pin para el led verde de "todo en orden"

//-- BUZZER --
#define ALARMA 5                // Pin para el buzzer

const int frecuencia = 440;

//-- MOTOR A PASOS / DRIVER ULN200 --
#define IN1 13    // Pin IN1 directo al driver ULN200
#define IN2 12    // Pin IN2 directo al driver ULN200
#define IN3 11    // Pin IN3 directo al driver ULN200
#define IN4 10    // Pin IN4 directo al driver ULN200
const int RETARDOPASOS = 5;     // Retardo entre pasos para no calentar el motor a pasos
int numero_pasos = 0;     // Pasos para el motor a pasos

boolean bandera = 0;                // Estatus del botón (0 = no se ha pulsado, 1 = botón pulsado)
String MENSAJEOK = "Todo en orden"; // Mensaje de todo en orden para display

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//-- SERVOMOTORES --
Servo hombroServo;    // Objeto servo para el hombro del brazo
Servo codoServo;      // Objeto servo para el codo del brazo
Servo munecaServo;    // Objeto servo para la muñeca del brazo
Servo garraServo;     // Objeto servo para la garra del brazo

#define HOMBRO 6      // Pin del servo directo al hombro
#define CODO 7        // Pin del servo directo al codo
#define MUNECA 8      // Pin del servo directo a la muñeca
#define GARRA 9       // Pin del servo directo a la garra

String cadenaSerial;
char serialChar[41];

String movimientos = "";  // Variable para guardar el mensaje obtenido del bluetooth
int grados[10];           // Arreglo de grados por articulación


void setup() {
  Serial.begin(9600);       // Inicializamos el serial
  
  pinMode(LEDOK, OUTPUT);   // Inicializacion de led verde
  
  pinMode(IN4, OUTPUT);     // Inicialización del pin directo al IN4
  pinMode(IN3, OUTPUT);     // Inicialización del pin directo al IN3
  pinMode(IN2, OUTPUT);     // Inicialización del pin directo al IN2
  pinMode(IN1, OUTPUT);     // Inicialización del pin directo al IN1
  
  pinMode(BOTONEMERGENCIA, INPUT_PULLUP);   // Modo del botón (PULL UP)
  attachInterrupt(digitalPinToInterrupt(BOTONEMERGENCIA), interrupcion, RISING);    // Inicialización de interrupción

  digitalWrite(LEDOK, HIGH);  // Inicio del led Verde como encendido

  // Objetos servomotores
  hombroServo.attach(HOMBRO); // Relación del pin con el servomotor
  codoServo.attach(CODO);     // Relación del pin con el servomotor
  munecaServo.attach(MUNECA); // Relación del pin con el servomotor
  garraServo.attach(GARRA);   // Relación del pin con el servomotor

  // Inicializar el LCD
  lcd.init();

  //Encender la luz de fondo.
  lcd.backlight();

  // Inicia el mensaje de inicio
  lcd.print(MENSAJEOK);

  /* Ciclo para leer la memoria EEPROM, convierte los valores a cadena
   *  y los guarda a la variable "movimientos"
   */
  for(int i = 0; i < 10; i++){
    if(EEPROM.read(i) == 0){
      movimientos = movimientos + "000";
    }else if(EEPROM.read(i) > 0 && EEPROM.read(i) < 10){
      movimientos = movimientos + "00" + EEPROM.read(i);
    }else if(EEPROM.read(i) > 10 && EEPROM.read(i) < 100){
      movimientos = movimientos + "0" + EEPROM.read(i);
    }else{
      movimientos = movimientos + EEPROM.read(i);  
    }
    
  }

  // Asignación de movimientos por articulación
  asignaMovimientos();
  // Se imprimen los movimientos por Serial
  imprimeMovimientos();
  
  Serial.println("----- EN EEPROM -----");
  Serial.println(movimientos);                // Mostrado de los grados que se almacenan en la EEPROM
}

void loop() {

    if(bandera == 0){ // Código de ejecución normal

      while(Serial.available()){            // Cuando el serial recibe un mensaje
        movimientos = "";
        cadenaSerial = Serial.readString();  // Los grados se guardan en la variable movimientos
        cadenaSerial.toCharArray(serialChar, 41);
        for(int i = 1; i<cadenaSerial.length();i++){
          if(!isWhitespace(serialChar[i])){
            movimientos += serialChar[i];
          }
        }
        
        asignaMovimientos();                // Asignación de grados por articulación
        
        Serial.println("NUEVO PARAMETRO MEMORIA EEPROM: ");
        Serial.println(movimientos);        // Mostrado de los nuevos datos para la EEPROM
    
        imprimeMovimientos();
   }

   if(movimientos.length()> 0){

// Proceso para el movimiento de las articulaciónes y guardado de grados en la EEPROM
/*  Función para mover los servomotores
 *  
 *  Partes:
 *    1 - Hombro
 *    2 - Codo
 *    3 - Muñeca
 *    4 - Garra
 */

     // -- Movimientos de los servomotores
     movimientoServo(4,grados[0]); EEPROM.write(0, grados[0]);    // Pinza
     movimientoServo(3,grados[1]); EEPROM.write(1, grados[1]);    // Muñeca
     movimientoServo(2,grados[2]); EEPROM.write(2, grados[2]);    // Codo
     movimientoServo(1,grados[3]); EEPROM.write(3, grados[3]);    // Hombro
      
     // -- Movimiento del motor a pasos
     movimientoPasos(grados[4]);   EEPROM.write(4, grados[4]);    // Base
     
     movimientoServo(1,grados[5]); EEPROM.write(5, grados[5]);    // Hombro
     movimientoServo(2,grados[6]); EEPROM.write(6, grados[6]);    // Codo
     movimientoServo(3,grados[7]); EEPROM.write(7, grados[7]);    // Muñeca
     
     movimientoServo(4,grados[8]); EEPROM.write(8, grados[8]);    // Pinza
     
     movimientoPasos(grados[9]);   EEPROM.write(9, grados[9]);    // Base
   }
    
  }else if(bandera == 1){ // Código de paro
            
    digitalWrite(LEDOK, LOW);   // Apagado de led verde
    lcd.print(MENSAJEOK);       // Mensaje de todo en orden
    paro();                     // Función de paro
    MENSAJEOK = "TODO EN ORDEN";// Nuevo mensaje de emergencia para display
    bandera = 0;                // Regreso del estatus a su estado anterior
    lcd.clear();                // Limpia LCD
    lcd.print(MENSAJEOK);       // Mensaje de todo en orden
    noTone(ALARMA);             // Apagado de buzzer
    digitalWrite(LEDOK, HIGH);  // Encendido de led Verde
  }
}

// -- Cambio de variables
void interrupcion(){
  MENSAJEOK = "¡EMERGENCIA!";
  bandera = 1;
}

// -- Todas las funciones al abortar la ejecución del brazo
void paro(){
  lcd.clear();            // Limpiado de LCD
  lcd.print(MENSAJEOK);   // Se imprime el mensaje de emergencia

  // -- Ciclo para encender y apagar el LED rojo y el buzzer
  for(int i = 0; i < 5; i++){
    tone(ALARMA, frecuencia);         // Enciende la alarma con la frecuencia marcada
    digitalWrite(LEDEMERGENCIA, HIGH);
    delay(800);
    digitalWrite(LEDEMERGENCIA, LOW);
    noTone(ALARMA);                   // Apaga la alarma  
    delay(500);
  }
}

/*  Función para mover los servomotores
 *  
 *  Partes:
 *    1 - Hombro
 *    2 - Codo
 *    3 - Muñeca
 *    4 - Garra
 */
void movimientoServo(int parte, int grados){
  if(parte == 1){
    hombroServo.write(grados);    // Movimiento de hombro X grados
    delay(500);
  }else if(parte == 2){
    codoServo.write(grados);      // Movimiento de codo X grados
    delay(500);
  }else if(parte == 3){           
    munecaServo.write(grados);    // Movimiento de muñeca X grados
    delay(500);
  }else if(parte == 4){
    garraServo.write(grados);     // Movimiento de garra X grados
    delay(500);
  }
}

// -- Función para mover el motor a pasos 
void movimientoPasos(int grados){
  int pasos = grados;
  if(pasos>0){
    delay(RETARDOPASOS);                // Retardo entre pasos
    pasos = (pasos * 1.42222222222);    // 512 Vueltas son 360 grados (560/360 = 1.422222)
  }

  while (pasos>numero_pasos){     // Giro hacia la izquierda en grados
       pasoIzquierda();
       numero_pasos = numero_pasos + 1;
   }
   while (pasos<numero_pasos){    // Giro hacia la derecha en grados
        pasoDerecha();
        numero_pasos = numero_pasos -1;
   }

   apagado();   // Sin polaridades
}

void asignaMovimientos(){
    /*
     *  El string de los grados (obtenido por serial) se parte de 3 caracteres en 3
     *  Se parten y reparten de la siguiente manera:
     */
  
    grados[0] = movimientos.substring(0,3).toInt();       // 1.- Movimiento 1 : Pinza
    grados[1] = movimientos.substring(3,6).toInt();       // 2.- Movimiento 1 : Muñeca
    grados[2] = movimientos.substring(6,9).toInt();       // 3.- Movimiento 1 : Codo
    grados[3] = movimientos.substring(9,12).toInt();      // 4.- Movimiento 1 : Hombro
    grados[4] = movimientos.substring(12,15).toInt();     // 5.- Movimiento 1 : Base
    grados[5] = movimientos.substring(15,18).toInt();     // 6.- Movimiento 2 : Hombro
    grados[6] = movimientos.substring(18,21).toInt();     // 7.- Movimiento 2 : Codo
    grados[7] = movimientos.substring(21,24).toInt();     // 8.- Movimiento 2 : Muñeca
    grados[8] = movimientos.substring(24,27).toInt();     // 9.- Movimiento 2 : Pinza
    grados[9] = movimientos.substring(27,30).toInt();     // 10.- Movimiento 2 : Base
}

// Muestra de los grados por articulación
void imprimeMovimientos(){
    Serial.println("Movimiento 1 pinza: ");
    Serial.println(grados[0]);

    Serial.println("Movimiento 1 nuñeca: ");
    Serial.println(grados[1]);

    Serial.println("Movimiento 1 codo: ");
    Serial.println(grados[2]);

    Serial.println("Movimiento 1 hombro: ");
    Serial.println(grados[3]);

    Serial.println("Movimiento 1 base: ");
    Serial.println(grados[4]);
    
    Serial.println("Movimiento 2 hombro: ");
    Serial.println(grados[5]);

    Serial.println("Movimiento 2 codo: ");
    Serial.println(grados[6]);

    Serial.println("Movimiento 2 muñeca: ");
    Serial.println(grados[7]);
    
    Serial.println("Movimiento 2 pinza: ");
    Serial.println(grados[8]);

    Serial.println("Movimiento 2 base: ");
    Serial.println(grados[9]);
}

// ----------------- FUNCIONES MOTOR A PASOS ---------------

void pasoDerecha(){         // Pasos a la derecha - Asignación de polaridades
  digitalWrite(IN4, LOW); 
  digitalWrite(IN3, LOW);  
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN1, HIGH);  
  delay(RETARDOPASOS); 
  digitalWrite(IN4, LOW); 
  digitalWrite(IN3, HIGH);  
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN1, LOW);  
  delay(RETARDOPASOS); 
  digitalWrite(IN4, HIGH); 
  digitalWrite(IN3, HIGH);  
  digitalWrite(IN2, LOW);  
  digitalWrite(IN1, LOW);  
  delay(RETARDOPASOS); 
  digitalWrite(IN4, HIGH); 
  digitalWrite(IN3, LOW);  
  digitalWrite(IN2, LOW);  
  digitalWrite(IN1, HIGH);  
  delay(RETARDOPASOS);
}

void pasoIzquierda() {        // Pasos a la izquierda - Asignación de polaridades
  digitalWrite(IN4, HIGH); 
  digitalWrite(IN3, HIGH);  
  digitalWrite(IN2, LOW);  
  digitalWrite(IN1, LOW);  
  delay(RETARDOPASOS); 
  digitalWrite(IN4, LOW); 
  digitalWrite(IN3, HIGH);  
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN1, LOW);  
  delay(RETARDOPASOS); 
  digitalWrite(IN4, LOW); 
  digitalWrite(IN3, LOW);  
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN1, HIGH);  
  delay(RETARDOPASOS); 
  digitalWrite(IN4, HIGH); 
  digitalWrite(IN3, LOW);  
  digitalWrite(IN2, LOW);  
  digitalWrite(IN1, HIGH);  
  delay(RETARDOPASOS); 
}
        
void apagado() {         // Apagado del Motor
 digitalWrite(IN4, LOW); 
 digitalWrite(IN3, LOW);  
 digitalWrite(IN2, LOW);  
 digitalWrite(IN1, LOW);
}
