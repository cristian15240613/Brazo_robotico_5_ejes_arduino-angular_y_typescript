#include <Wire.h>               // Librería wire para comunicación de arduino con dispositivos I2C
#include <LiquidCrystal_I2C.h>  // Librería para controlar el LCD con el dispositivo I2C
#include <EEPROM.h>
#include <Servo.h>

#define botonEmergencia 2
#define ledEmergencia 3
#define ledOK 4
#define alarma 5

const int frecuencia = 440;

int pulsaciones = 0;

boolean bandera = 0;

String mensaje = "Todo en orden";

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x3F, 8, 2); //

Servo hombroServo;
Servo codoServo;
Servo munecaServo;
Servo garraServo;

#define HOMBRO 6
#define CODO 7
#define MUNECA 8
#define GARRA 9

String movimientos = "";
int grados[10];

void setup() {
  pulsaciones = EEPROM.read(0);
  
  pinMode(ledOK, OUTPUT);
  pinMode(botonEmergencia, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(botonEmergencia), interrupcion, RISING);

  Serial.begin(9600);
  hombroServo.attach(HOMBRO);
  codoServo.attach(CODO);
  munecaServo.attach(MUNECA);
  garraServo.attach(GARRA);

  // Inicializar el LCD
  lcd.init();

  //Encender la luz de fondo.
  lcd.backlight();

}

void loop() {

  if(bandera == 0){ // Código de ejecución normal
    digitalWrite(ledOK, HIGH);
    lcd.print(mensaje);
    noTone(alarma);

    while(Serial.available()){
    movimientos = Serial.readString();
    Serial.println(movimientos);
    
    grados[0] = movimientos.substring(0,2).toInt();
    grados[1] = movimientos.substring(2,4).toInt();
    grados[2] = movimientos.substring(4,6).toInt();
    grados[3] = movimientos.substring(6,8).toInt();
    grados[4] = movimientos.substring(8,10).toInt();
    grados[5] = movimientos.substring(10,12).toInt();
    grados[6] = movimientos.substring(12,14).toInt();
    grados[7] = movimientos.substring(14,16).toInt();
    grados[8] = movimientos.substring(16,18).toInt();
    grados[9] = movimientos.substring(18,20).toInt();
    
    Serial.println("Movimiento 1 base: ");
    Serial.println(grados[0]);

    Serial.println("Movimiento 2 base: ");
    Serial.println(grados[1]);

    Serial.println("Movimiento 1 hombro: ");
    Serial.println(grados[2]);

    Serial.println("Movimiento 2 hombro: ");
    Serial.println(grados[3]);

    Serial.println("Movimiento 1 codo: ");
    Serial.println(grados[4]);

    Serial.println("Movimiento 2 codo: ");
    Serial.println(grados[5]);
    
    Serial.println("Movimiento 1 muñeca: ");
    Serial.println(grados[6]);

    Serial.println("Movimiento 2 muñeca: ");
    Serial.println(grados[7]);

    Serial.println("Movimiento 1 garra: ");
    Serial.println(grados[8]);

    Serial.println("Movimiento 2 garra: ");
    Serial.println(grados[9]);
   }

   if(movimientos != ""){
    movimientoServo(1,grados[2]);
    movimientoServo(1,grados[3]);

    movimientoServo(2,grados[4]);
    movimientoServo(2,grados[5]);

    movimientoServo(3,grados[6]);
    movimientoServo(3,grados[7]);

    movimientoServo(4,grados[8]);
    movimientoServo(4,grados[9]);
   }
    
  }else if(bandera == 1){ // Código de paro
      pulsaciones++;
      EEPROM.write(0,pulsaciones);
      
      Serial.println("Pulsaciones: ");
      Serial.println(pulsaciones);
      
    digitalWrite(ledOK, LOW);
    lcd.print(mensaje);
    paro();
    mensaje = "todo en orden";
    bandera = 0;
    
  }
}

// -- Cambio de variables
void interrupcion(){
  mensaje = "Emergencia";
  bandera = 1;
}

// -- Todas las funciones al abortar la ejecución del brazo
void paro(){
  lcd.clear();
  lcd.print(mensaje);  
  
  for(int i = 0; i < 5; i++){
    tone(alarma, frecuencia);         // Enciende la alarma con la frecuencia marcada
    digitalWrite(ledEmergencia, HIGH);
    delay(800);
    digitalWrite(ledEmergencia, LOW);
    noTone(alarma);                   // Apaga la alarma  
    delay(500);
  }
}

/* Partes:
 *  1 - Hombro
 *  2 - Codo
 *  3 - Muñeca
 *  4 - Garra
 */
void movimientoServo(int parte, int grados){
  if(parte == 1){
    hombroServo.write(grados);
    delay(100);
  }else if(parte == 2){
    codoServo.write(grados);
    delay(100);
  }else if(parte == 3){
    munecaServo.write(grados);
    delay(100);
  }else if(parte == 4){
    garraServo.write(grados);
    delay(400);
  }
}
