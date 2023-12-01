#include <pico/stdlib.h>
#include <hardware/adc.h>
#include "DHT.h"

//defino GPIO y tipo de DHT
#define DHTPIN 15   
#define DHTTYPE DHT11 
#define R 18
#define A 20
#define V 16

//defino el DHT
DHT dht(DHTPIN, DHTTYPE);

//defino el GPIO del sensor de humedad
int sensorhum = 28;

//defino el sensor de temp interno
const float conv_factor = 3.3/(1<<12);
uint16_t lectura;
float lect_conv, temp;

//defino varibles para usar con el DHT
float ht = 0;
int hmax = 560;
int hmin = 1023;
float hmed;

void setup() 
{
  Serial.begin(9600);

  pinMode (R, OUTPUT);
  pinMode (V, OUTPUT);
  pinMode (A, OUTPUT);

  //inicializo port ADC para temp interna
  adc_init();
  adc_set_temp_sensor_enabled(true);
  adc_select_input(4);
  
  //inicializo el DHT
  dht.begin();
  
  delay(3000);
  Serial.println("-----------------------------------");
  Serial.println("  Inicializando Miragua :D!!  ");
  Serial.println("-----------------------------------");
  delay (1500);
  Serial.println("                                   ");
  Serial.println("  Llamando a Pachamama...          ");
  Serial.println("                                   ");
  delay(1500);
  Serial.println("  Probando la frescura de la tierra... ");
  Serial.println("                                   ");
  delay(1500);
  Serial.println("  Preguntandole a Dave el Loco... ");
  Serial.println("                                   ");
  delay (4000);
  Serial.println("  Aprovado por el Webi Wabo 👍😎  ");
  delay(1500);
}

void loop() 
{
  delay(2500); // delay entre lecturas
  // mido temperatura y humedad en el DHT
  float t=dht.readTemperature();
  float h=dht.readHumidity();

    //punto de control
    //Serial.print ("Temperatura Ambiente segun DHT11 (C): ");
    //Serial.println(t);
    //Serial.print ("Humedad Ambiente segun DHT11 (%): ");
    //Serial.println(h);
  
  
  // leemos la humedad de la tierra
  hmed = analogRead(sensorhum);
  ht = ((hmed-hmin)/(hmax-hmin))*100;
  
  // leemos la temperatura del RPI en celsius
  lectura = adc_read();
  lect_conv = lectura * conv_factor;
  temp = 27.0 - ((lect_conv-0.706)/(1<<12));
  
  Serial.println("######################################");
  Serial.print("Temperatura del controlador RPI: ");
  Serial.println(temp);
  Serial.println("--------------------------------------");
  
  // validamos que no de error la lectura del Higrometro y DHT
  if (isnan(ht) || isnan(t) || isnan (h)) 
    {
    Serial.println(F("Failed to read from sensor!"));
    return;
    }  
  else
    { 
    if (ht>125) //si la humedad en tierra pasa el umbral de humedad
        {
        Serial.println("Humedad de la tierra alta");
        Serial.print("Humedad medida en tierra (%): ");
        Serial.println(ht);
        Serial.println("--------------------------------------");
        digitalWrite(V, LOW);
        digitalWrite(A, LOW);
        digitalWrite(R, HIGH);
        }
        else
          {
          if (ht<125 && ht >15)
            {
            digitalWrite(R, LOW);
            digitalWrite(A, LOW);
            digitalWrite(V, HIGH);
            Serial.println("Humedad en tierra ideal ");
            Serial.print("Humedad medida en tierra (%): ");
            Serial.println(ht);
            Serial.println("--------------------------------------");
            }
          else
            {
            digitalWrite(V, LOW);
            digitalWrite(R, LOW);
            digitalWrite(A, HIGH);
            Serial.println("Humedad en tierra baja ");
            Serial.print("Humedad medida en tierra (%): ");
            Serial.println(ht);
            Serial.println("--------------------------------------");
            }
          }
    if (t>26) //si la temperatura ambiente supera el umbral
        {
        Serial.println("Temparuta ambiente mayor a 26C");
        Serial.print("Temperatura ambiente medida (C): ");
        Serial.println(t);
        Serial.println("--------------------------------------");
        }
        else
        {
        Serial.println("Temparuta ambiente menor a 26C");
        Serial.print("Temperatura ambiente medida (C): ");
        Serial.println(t);
        Serial.println("--------------------------------------");
        }
      }  
    if (ht<=55) //si la humedad en tierra es menor o igual al minimo
      {
       Serial.println("Humedad en tierra baja ");
       Serial.print("Humedad medida en tierra (%): ");
       Serial.println(ht);
       Serial.println("--------------------------------------"); 
      }
    }
