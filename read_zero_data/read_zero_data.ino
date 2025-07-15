const int analogPin = A0;
const float alpha = 0.1;
const float alpha_baseline = 0.95;
const int threshold = 2;
unsigned long lastUpdate = 0;
const long interval = 20;

int baseline = 512;
int rawValue = 0;
int filteredValue = 0;
bool eventDetected = false;

void setup() {
  Serial.begin(115200);
  baseline = analogRead(analogPin);
}

void loop() {
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();
    
    rawValue = analogRead(analogPin);
    filteredValue = alpha * rawValue + (1 - alpha) * filteredValue;
    int delta = baseline - rawValue;

    if (abs(rawValue - baseline) < threshold) {
      baseline = baseline * alpha_baseline + rawValue * (1 - alpha_baseline);
    }

    
    if (delta > threshold) {
      eventDetected = true;
      Serial.print("EVENT: ");
      Serial.println(delta);
    }else if (delta < 0){
      baseline = baseline * alpha_baseline + rawValue * (1 - alpha_baseline);
      Serial.print("NOT_EVENT: ");
    }
    
    if (eventDetected && (rawValue > baseline - threshold/2)) {
      eventDetected = false;
    }
    
    Serial.print(eventDetected ? delta : 0);
    Serial.print(" ");
    Serial.print(rawValue);
    Serial.print(" ");
    Serial.print(delta);
    Serial.print(" ");
    Serial.println(baseline);
  }
}
