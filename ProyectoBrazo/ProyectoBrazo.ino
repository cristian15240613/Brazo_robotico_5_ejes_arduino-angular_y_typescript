#include <Wire.h>               // Librería wire para comunicación de arduino con dispositivos I2C
#include <LiquidCrystal_I2C.h>  // Librería para controlar el LCD con el dispositivo I2C
#include <EEPROM.h>

#define botonEmergencia 2
#define ledEmergencia 6
#define ledOK 7
#define alarma 5

const int frecuencia = 440;

int pulsaciones = 0;

boolean bandera = 0;

String mensaje = "Todo en orden";

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x3F, 8, 2); //

void setup() {
  pulsaciones = EEPROM.read(0);
  
  pinMode(ledOK, OUTPUT);
  pinMode(botonEmergencia, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(botonEmergencia), interrupcion, RISING);

  Serial.begin(9600);

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
