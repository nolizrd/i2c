#define ROW_1 2
#define ROW_2 3
#define COL_1 4
#define COL_2 5

void setup() {
  Serial.begin(9600);
  Serial.println("Started!");
  pinMode(ROW_1, OUTPUT);
  pinMode(ROW_2, OUTPUT);
  pinMode(COL_1, INPUT_PULLUP);
  pinMode(COL_2, INPUT_PULLUP);
}

void loop() {
  
  for (int row = 1; row <= 2; row++) {
    for (int col = 1; col <= 2; col++) {
      digitalWrite(ROW_1, (row == 1) ? HIGH : LOW);
      digitalWrite(ROW_2, (row == 2) ? HIGH : LOW);
      
      if (digitalRead((col == 1) ? COL_1 : COL_2) == LOW) {
        
        handleButtonPress(row, col);
      }
    }
  }
}

void handleButtonPress(int row, int col) {
  int buttonNumber = 0;
  if (row == 1 && col == 1) {
    buttonNumber = 3;
  } else if (row == 1 && col == 2) {
    buttonNumber = 4;
  } else if (row == 2 && col == 1) {
    buttonNumber = 1;
  } else if (row == 2 && col == 2) {
    buttonNumber = 2;
  }

  Serial.print("Button pressed: ");
  Serial.println(buttonNumber);
  delay(200);
}
