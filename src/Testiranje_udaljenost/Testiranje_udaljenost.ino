#include <dht.h>

//Senzor za udaljenost
#define triggerInput 7
#define echoOutput 6
//Led RYG
#define Red 11
#define Yellow 10
#define Green 9
//Senzor temperatura-vlaga
#define SenzorDHT 12

dht DHT;
int flagIspisEkran = 0; //Potreban jer se na ekran ispisuje svakih sekundu, a udaljenost se provjerava svakih pola sekunde

void setup() {
  Serial.begin (9600);
  pinMode(triggerInput, OUTPUT);
  pinMode(echoOutput, INPUT);
  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
}

void loop() {
  //1. Pocetno setiranje
  long trajanje, udaljenost;
  digitalWrite(triggerInput, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerInput, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerInput, LOW);
  
  //2. Racunanje udaljenosti od predmeta i reakcija na udaljenost
  trajanje = pulseIn(echoOutput, HIGH);
  udaljenost = (trajanje / 2) / 29.1;
  provjeriUdaljenost(udaljenost);

  //3. Uvjetno ispisivanje na ekran svakih sekundu
  if (flagIspisEkran % 1000 == 0)
    ispisiEkran(udaljenost);
  flagIspisEkran += 500;
  flagIspisEkran = flagIspisEkran % 1000;
  delay(480); //480 + 10 + 2 + 8 = 500ms
};

void ispisiEkran(int udaljenost)
{
  DHT.read11(SenzorDHT);
  Serial.print("Udaljenost: ");
  Serial.print(udaljenost);
  Serial.print(" cm, Vlaznost zraka: ");
  Serial.print(DHT.humidity, 2);
  Serial.print("%, Temperatura: ");
  Serial.print(DHT.temperature, 2);
  Serial.print("C\n");

}
void provjeriUdaljenost(int udaljenost)
{
  if (udaljenost < 5) {  // Sve tri odjednom bljeskaju svakih pola sekunde
    digitalWrite(Red, HIGH);
    digitalWrite(Yellow, HIGH);
    digitalWrite(Green, HIGH);
    delay(8);
    digitalWrite(Red, LOW);
    digitalWrite(Yellow, LOW);
    digitalWrite(Green, LOW);
  }
  else if (udaljenost >= 5 && udaljenost < 10) { //sve tri upaljene
    digitalWrite(Red, HIGH);
    digitalWrite(Yellow, HIGH);
    digitalWrite(Green, HIGH);
    delay(8);
  }
  else if (udaljenost >= 10 && udaljenost < 20) { // zuta i zelena upaljene
    digitalWrite(Yellow, HIGH);
    digitalWrite(Green, HIGH);
    digitalWrite(Red, LOW);
    delay(8);
  }
  else if (udaljenost >= 20) { // zelena upaljena
    digitalWrite(Red, LOW);
    digitalWrite(Yellow, LOW);
    digitalWrite(Green, HIGH);
    delay(8);
  }
}
