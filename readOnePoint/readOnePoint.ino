const int pinOutput = 2;
const int analogPin[14] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13}; // массив названий аналоговых пинов
const int N_dots = 1; // кол-во считываемых датичиков
float analogValue[14]; // массив для хранения информации о считанных данных
float buf_analogValue[14]; // буфер для хранения информации о считанных данных на предыдущей итерации

float accelaration_start[3] = {-0.015, -0.015, -0.45}; // ускорение для начала работы

float accelaration_stop[3] = {0.02, 0.015, 0.5}; // ускорение для конца работы

float baseLine[14]; // массив тарированных значений

void setup() {
  pinMode(pinOutput, OUTPUT);
  Serial.begin(115200);

  digitalWrite(pinOutput, HIGH);
  for (int n = 0; n < 14; n++){ // первое тарирование на свободном датчике
    analogValue[n] = analogRead(analogPin[n]); 
    buf_analogValue[n] = analogValue[n];
  }
  digitalWrite(pinOutput, LOW);
}

int av[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // массив для хранения среднего арифметического
float accelaration[14]; // массив для хранения ускорений
int flag[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // массив для хранения флагов о начале отключения тарирования


void loop() {
  for (int i = 0; i < 40; i++){
    for (int j = 0; j < 5; j++){
      for (int n = 0; n < N_dots; n++){
        digitalWrite(pinOutput, HIGH);
        analogValue[n] = analogRead(analogPin[n]);
        av[n] += analogValue[n];
      }
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
  }  
}
