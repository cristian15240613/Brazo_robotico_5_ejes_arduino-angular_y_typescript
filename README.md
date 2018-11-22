
# Manejo de brazo robótico en arduino mediante conexión bluetooth e interfaz en processing.

## Introducción

> Brazo robótico controlado desde una interfaz gráfica desarrollada en Processing y conectado mediante un dispositivo bluetooth; es capaz de mover cada una de las articulaciones de forma independiente o en conjunto para tomar y mover objetos cumpliendo su ciclo de movimientos aún después de una caía de corriente eléctrica.

## Codigo de ejemplo
> <h3> Servo motor </h3>
grados[0] = movimientos.substring(0,2).toInt();
<h3>  Motor a pasos </h3>
 digitalWrite(11, LOW);  
 digitalWrite(10, LOW);  
 digitalWrite(9, HIGH);  
 digitalWrite(8, HIGH);
<h3>  Bluetooth </h3>
SoftwareSerial BT(TX, RX);  



## Software y herramientas

 > IDE Arduino 
 > Servo motor 
 > Motor a pasos 
 > Buzzer 
 > Led 
 > Pantalla LCD
