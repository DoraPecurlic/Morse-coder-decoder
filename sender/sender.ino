const int LED_PIN = 13;
const int DOT_DELAY = 300;

// Morse code representation for letters
const char* MORSE_CODE[] = {
  ".-",   // A
  "-...", // B
  "-.-.", // C
  "-..",  // D
  ".",    // E
  "..-.", // F
  "--.",  // G
  "....", // H
  "..",   // I
  ".---", // J
  "-.-",  // K
  ".-..", // L
  "--",   // M
  "-.",   // N
  "---",  // O
  ".--.", // P
  "--.-", // Q
  ".-.",  // R
  "...",  // S
  "-",    // T
  "..-",  // U
  "...-", // V
  ".--",  // W
  "-..-", // X
  "-.--", // Y
  "--.."  // Z
};

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char inputChar = Serial.read();
    if (isLowerCaseLetter(inputChar)) {
      flashSequence(MORSE_CODE[inputChar - 'a']);
    } else if (inputChar == ' ') {
      delay(DOT_DELAY * 7); // Space between words
    }
  }
}

void flashSequence(const char* sequence) {
  for (int i = 0; sequence[i] != '\0'; i++) {
    flashDotOrDash(sequence[i]);
  }
  delay(DOT_DELAY * 3); // Space between characters
}

void flashDotOrDash(char dotOrDash) {
  digitalWrite(LED_PIN, HIGH);
  delay((dotOrDash == '.') ? DOT_DELAY : DOT_DELAY * 3);
  digitalWrite(LED_PIN, LOW);
  delay(DOT_DELAY); // Space between dots and dashes
}

bool isLowerCaseLetter(char ch) {
  return ch >= 'a' && ch <= 'z';
}