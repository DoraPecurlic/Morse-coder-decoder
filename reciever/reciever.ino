const int LIGHT_SENSOR_PIN = A0;
const int CALIBRATION_SENSOR_PIN = A1;
const int DOT_DURATION = 300;
const int SHORT_GAP_THRESHOLD = 2;
const int MEDIUM_GAP_THRESHOLD = 6;
const int LONG_GAP_THRESHOLD = 10;
const int MAX_GAP_THRESHOLD = 50;

// Variables
int lightSensorValue = 0;
int codePointer = 0;
bool isLightOn = false;
unsigned long timer = millis();
int lightOnDuration = 0;
int lightOffDuration = 0;
bool isNotAnalysed = false;
bool isEndOfTransmission = true;
int threshold = 0;

// Morse code representation
const char MorseTree[] = {'\0', 'E', 'T', 'I', 'A', 'N', 'M', 'S', 'U', 'R', 'W', 'D', 'K', 'G', 'O', 'H', 'V', 'F', 'U', 'L', 'A', 'P', 'J', 'B', 'X', 'C', 'Y', 'Z', 'Q', '\0', '\0', '5', '4', '\0', '3', '\0', '\0', '\0', '2', '\0', '\0', '+', '\0', '\0', '\0', '\0', '\0', '1', '6', '=', '/', '\0', '\0', '\0', '(', '\0', '7', '\0', '\0', '\0', '8', '\0', '9', '0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '?', '_', '\0', '\0', '\0', '\0', '"', '\0', '\0', '.', '\0', '\0', '\0', '\0', '@', '\0', '\0', '\0', '\0', '\0', '\0', '-', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', ';', '!', '\0', ')', '\0', '\0', '\0', '\0', '\0', ',', '\0', '\0', '\0', '\0', ':', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

// Function declarations
void getMorse();
void calibrateThreshold();

void setup() {
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  Serial.begin(9600);
  delay(50);
  calibrateThreshold();
}

void loop() {
  getMorse();
}

void getMorse() {
  lightSensorValue = analogRead(LIGHT_SENSOR_PIN);

  if (lightSensorValue >= threshold) {
    if (!isLightOn) {
      lightOffDuration = millis() - timer;
      timer = millis();
      isLightOn = true;
      isNotAnalysed = true;
      isEndOfTransmission = false;
      lightOffDuration /= DOT_DURATION;
    }
  } else {
    if (isLightOn) {
      lightOnDuration = millis() - timer;
      timer = millis();
      isLightOn = false;
      isNotAnalysed = true;
      isEndOfTransmission = false;
      lightOnDuration /= DOT_DURATION;
    }

    if (!isEndOfTransmission && (millis() - timer) / DOT_DURATION >= 10) {
      Serial.print(MorseTree[codePointer]);
      codePointer = 0;
      isEndOfTransmission = true;
      Serial.print(" ");
    }
  }

  if (isNotAnalysed && !isLightOn) {
    if (lightOnDuration > 0.5 && lightOnDuration < 2) {
      codePointer = (2 * codePointer) + 1;
      isNotAnalysed = false;
    } else if (lightOnDuration >= 2) {
      codePointer = (2 * codePointer) + 2;
      isNotAnalysed = false;
    } else if (lightOnDuration >= 5) {
      Serial.println("--------- Morse code issue: *High* for too long ---------");
    }
  }

  if (isNotAnalysed && isLightOn) {
    if (lightOffDuration < 2) {
      isNotAnalysed = false;
    } else if (lightOffDuration >= 2 && lightOffDuration < 6) {
      Serial.print(MorseTree[codePointer]);
      codePointer = 0;
      isNotAnalysed = false;
    } else if (lightOffDuration >= 6 && lightOffDuration < 10) {
      Serial.print(MorseTree[codePointer]);
      codePointer = 0;
      isNotAnalysed = false;
      Serial.print(" ");
    } else if (lightOffDuration > MAX_GAP_THRESHOLD) {
      // Do nothing for now
    }
  }
}

void calibrateThreshold() {
  int initialValue = analogRead(CALIBRATION_SENSOR_PIN);
  threshold = initialValue * 1.9;
}
