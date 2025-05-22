#include <DFRobot_DS1307.h>
#include "DHT.h"
#include <SD.h>



const int DHTPIN = 4;
const int cspin = 10;
const int moister = A0;
const int moistp = 8;
const int solarpin = A1;
const int pumpin = 7;

#define DHTTYPE DHT11


DHT dht(DHTPIN, DHTTYPE);
File myFile;
DFRobot_DS1307 rtc;

void setup()
{
   
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  
  dht.begin();

   if (!SD.begin(cspin)) {
    Serial.println("initialization success!");
    return;
  }
  Serial.println("initialization done.");

    myFile = SD.open("anh.txt", FILE_WRITE);

      if (myFile) {
        Serial.println("filecheck");
        myFile.println("start");
        myFile.close();
        Serial.println("hooray");
  } else {
    Serial.println("womp");
  }

  Serial.println("Begin ok!");

  while( !(rtc.begin()) ){

    Serial.println("Communication with device failed, please check connection");
    delay(3000);

  }

  rtc.start();

  rtc.setSqwPinMode(rtc.eSquareWave_1Hz);

  pinMode(moistp, OUTPUT);
  pinMode(pumpin, OUTPUT);
  pinMode(moister, INPUT);
  pinMode(solarpin, INPUT);
}






void loop()
{
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();
  int solar = analogRead(solarpin);
  int moisture = moist();
  real(humid, temp, solar, moisture);
  pump(moisture);
}





void real(float humid, float temp, int solar, int moisture)
{
  uint16_t getTimeBuff[7] = {0};
  rtc.getTime(getTimeBuff);
  char outputarr[128];
  sprintf(outputarr, "time: %d/%d/%d-%d %d:%d:%d",

            getTimeBuff[6],
            getTimeBuff[5],
            getTimeBuff[4],
            getTimeBuff[3],
            getTimeBuff[2],
            getTimeBuff[1],
            getTimeBuff[0]

            );
  delay(1000);
  Serial.print(outputarr);
  Serial.print(" - ");
  Serial.print("Humidity: ");
  Serial.print(humid,1);
  Serial.print(", Temperature: ");
  Serial.print(temp,1);
  Serial.print(", Light: ");
  Serial.print(solar);
  Serial.print(", Moisture: ");
  Serial.println(moisture);
  if(getTimeBuff[0] == 30|| getTimeBuff[0] == 0)
  {
    myFile = SD.open("anh.txt", FILE_WRITE);

        if (myFile) {
          Serial.print("recording ");

          myFile.print(outputarr);
          myFile.print(" - ");
          myFile.print("Humidity: ");
          myFile.print(humid,1);
          myFile.print(", Temperature: ");
          myFile.print(temp,1);
          myFile.print(", Light: ");
          myFile.print(solar);
          myFile.print(", Moisture: ");
          myFile.println(moisture);
         
          myFile.close();
    } else {
      Serial.print("womp");
    }
  }
}




static moist()
{
  float moistne;
  digitalWrite(moistp, HIGH);
  moistne = (analogRead(moister));
  digitalWrite(moistp, LOW);
  return(moistne);
}





void pump(float moist)
{
  if(moist < 400)
  {
    digitalWrite (pumpin, HIGH);
  }
  else
  {
    digitalWrite (pumpin, LOW);
  }
}
