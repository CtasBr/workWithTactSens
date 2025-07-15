const int pinOutput = 2;
const int analogPin[14] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13};
const int N_dots = 3;
float analogValue[14];
float buf_analogValue[14];

float accelaration_start[3] = {-0.3, -0.06, -0.45};

float accelaration_stop[3] = {0.08, 0.15, 0.5};

float baseLine[14];
int tare[14];
int buf_tare;

void setup() {
  pinMode(pinOutput, OUTPUT);
  Serial.begin(115200);

  digitalWrite(pinOutput, HIGH);
  for (int n = 0; n < 14; n++){
    analogValue[n] = analogRead(analogPin[n]); 
    tare[n] = analogValue[n];
    buf_analogValue[n] = analogValue[n];
  }
  digitalWrite(pinOutput, LOW);
}

int av[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float accelaration[14];
int flag[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


void loop() {
  for (int i = 0; i < 40; i++){
    for (int j = 0; j < 5; j++){
      for (int n = 0; n < N_dots; n++){
        digitalWrite(pinOutput, HIGH);
        analogValue[n] = analogRead(analogPin[n]);
        av[n] += analogValue[n];
      }
      
      // digitalWrite(pinOutput, LOW);
      delay(7);
    }

    for (int n = 0; n < N_dots; n++){
      analogValue[n] = av[n]/5.0;
      av[n] = 0;

      accelaration[n] = (analogValue[n]-buf_analogValue[n])/35.0;

      if (accelaration[n] < accelaration_start[n]){
        flag[n] = 1;
      }else if (accelaration[n] > accelaration_stop[n]){
        flag[n] = 0;
      }

      if (flag[n] != 1){
        baseLine[n] = analogValue[n];
      }
      Serial.print(analogValue[n] - baseLine[n]); Serial.print(" "); 

      Serial.print(accelaration[n]); Serial.print(" ");

      buf_analogValue[n] = analogValue[n];
    }
    Serial.println("");
    // Serial.print(accelaration[n]); Serial.print(" "); Serial.print(analogValue[n] - baseLine[n]); Serial.print(" "); Serial.print(analogValue[n]); Serial.print(" "); Serial.println(flag[n]); 
    // 
  }  
}
