
# Manejo de brazo robótico en arduino mediante conexión bluetooth e interfaz en android

## Introducción

Brazo robótico controlado desde una interfaz gráfica desarrollada en Android Studio y conectado mediante un dispositivo bluetooth; es capaz de mover cada una de las articulaciones de forma independiente o en conjunto para tomar y mover objetos cumpliendo su ciclo de movimientos aún después de una caía de corriente eléctrica.

## Codigo de ejemplo
<h3> Servo motor </h3>
grados[0] = movimientos.substring(0,2).toInt();
<h3>  Motor a pasos </h3>
 digitalWrite(11, LOW);  <br>
 digitalWrite(10, LOW);  <br>
 digitalWrite(9, HIGH);  <br>
 digitalWrite(8, HIGH);  <br>
<h3>  Bluetooth </h3>
SoftwareSerial BT(TX, RX);  



## Software y herramientas
<ul>
 <li type="square"> IDE Arduino </li>
 <li type="square"> IDE Android Studio </li>
 <li type="square"> Servo motor </li>
 <li type="square"> Motor a pasos </li>
 <li type="square"> Modulo HC-05 (Bluetooth) </li>
 <li type="square"> Buzzer </li>
 <li type="square"> Diodo led </li>
 <li type="square"> Pantalla LCD </li>
</ul>

## Librerias
<ul> 
  <li type = ""> LiquidCrystal </li>
  <li type = ""> Servo </li>
  <li type = ""> EEMPROM </li>
  <li type = ""> SoftwareSerial </li>
</ul>

