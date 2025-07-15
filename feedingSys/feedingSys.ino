#include "GParser.h"
#include <SoftwareSerial.h>
#include <math.h>

#define EXTRUDE_PIN       3
#define x_offset          210
#define platform_height   250
#define z_offset          140
#define R                 100 * pow(10, -3)/2 // Радиус иглы (mm)
#define n                 100 // Коэффициент динамической вязкости ()
#define lt                10 // Длина иглы (mm)
// #define 


double x;
double y;
double z;
double f;
double e;
double pInternal;
double x_real = 250;
double y_real = 0;
double z_real = 250;
double xPrev = 0;
double yPrev = 0;
double zPrev = 0;
double t;
byte setSteadyData[] = {0x02, 0x03, 0x34, 0x4d, 0x54, 0x20, 0x20, 0x42, 0x42, 0x03};

void sendPressureToRS(byte dataArray[4]){
  byte setPressureArray[] = {0x02, byte('0'), byte('8'), byte('P'), byte('S'), byte(' '), byte(' '), dataArray[0], dataArray[1], dataArray[2], dataArray[3], byte('F'), byte('0'), 0x03};

  Serial1.write(setPressureArray, 14);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial1.write(setSteadyData, 10);
}

void loop() {
  if (Serial.available() > 0) {
      char commands[30];
      int amount = Serial.readBytesUntil(';', commands, sizeof(commands));
      commands[amount] = NULL;
      Serial.print("commands = ");
      Serial.println(commands);
      GParser data(commands, ' ');
      int am = data.split();

      switch (data.getInt(0)) {
          case 1:
              x = data.getFloat(1) + x_offset;
              y = data.getFloat(2);
              z = data.getFloat(3) - platform_height + z_offset;
              e = data.getFloat(4);
              f = data.getFloat(5);
              pInternal = (8 * pow((1.75/2), 2) * e * f * n * lt * 1000)/(pow((R * 1000), 4) * sqrt(sq(x - xPrev) + sq(y - yPrev) + sq(y - yPrev)));
              // Передаем через RS-232, и зажимаем активацию
              t = (sqrt(sq(x - xPrev) + sq(y - yPrev) + sq(y - yPrev)))/f;
              
              byte dataArray[] = {
                pInternal > 0 ? int(floor(pInternal))/100%10 : 0,
                pInternal > 0 ? int(floor(pInternal))/10%10 : 0, 
                pInternal > 0 ? int(floor(pInternal))%10 : 0, 
                pInternal - floor(pInternal)
              };
              sendPressureToRS(dataArray);
              digitalWrite(EXTRUDE_PIN, HIGH);
              delay(t * 1000);
              digitalWrite(EXTRUDE_PIN, LOW);


              // is_done = move_coordinates(x, y, z, e);
              // if (is_done == 0) {
              //     x_real = x; y_real = y; z_real = z;
              // }
              break;
      }
    }
  
}
