
# Manejo de brazo robótico en arduino mediante conexión bluetooth e interfaz en AppInventor

## Desarrolladores
Christian Alvarez
Sandra Mireles
Jessica Nicasio

## Introducción

Brazo robótico controlado desde una interfaz gráfica desarrollada en AppInventor y conectado mediante un dispositivo bluetooth; es capaz de mover cada una de las articulaciones de forma independiente o en conjunto para tomar y mover objetos cumpliendo su ciclo de movimientos aún después de una caía de corriente eléctrica.

## Codigo principal
<h3> Servo motor </h3>
grados[0] = movimientos.substring(0,2).toInt();
<h3>  Motor a pasos </h3>
 digitalWrite(11, LOW);  <br>
 digitalWrite(10, LOW);  <br>
 digitalWrite(9, HIGH);  <br>
 digitalWrite(8, HIGH);  <br>
<h3>  Bluetooth </h3>
SoftwareSerial BT(TX, RX);  

## Sistema operativo
<ul>
 <li type="square"> Linux Elementary Locky </li>
 <li type="square"> K Ubuntu 18.0 </li>
</ul>

## Software y herramientas
<ul>
 <li type="square"> IDE Arduino </li>
 <li type="square"> AppInventor </li>
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

## Contacto
Chrisian: 15240613@itleon.edu.mx
Sandra: mirelesa4@gmail.com
Jessica: jessi_dragon55@hotmail.com
