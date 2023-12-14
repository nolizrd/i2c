#include <Wire.h>

int ownAddress = 3; // Адрес текущего слейва
unsigned char id = 'D'; // Имя текущего слейва

struct AddressTable {
  char name;
  int address;
};

AddressTable addressTable[] = {
  {'B', 1},
  {'C', 2},
  {'D', 3}
};

void setup() {
  Serial.begin(9600);
  Wire.begin(ownAddress);
  Wire.onReceive(receiveEvent);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input.startsWith("send ")) {
      char recipientName = input.charAt(5);
      int recipientAddress = getRecipientAddress(recipientName);
      
      if (recipientAddress != -1) {
        String message = input.substring(7);
        sendMessage(recipientAddress, message);
        Serial.println("Message sent.");
      } else {
        Serial.println("Recipient not found in the address table.");
      }
    }
  }
}

void sendMessage(int recipientAddress, String message) {
  Wire.beginTransmission(recipientAddress);
  Wire.write(id);
  Wire.write(message.c_str());
  Wire.endTransmission();
}

int getRecipientAddress(char recipientName) {
  for (int i = 0; i < sizeof(addressTable) / sizeof(addressTable[0]); i++) {
    if (addressTable[i].name == recipientName) {
      return addressTable[i].address;
    }
  }
  return -1; // Возвращаем -1, если адресата не найдено
}

void receiveEvent(int length) {
  lastSenderId = Wire.read(); // Обновление: сохраняем адресат на каждый receiveEvent
  
  Serial.print("Sender's ID: '");
  Serial.write(lastSenderId);
  Serial.println("'");

  Serial.print("Message: ");
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  char x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

// Добавляем функцию для отправки ответа последнему написавшему
void replyToLastSender(String message) {
  if (lastSenderId != -1) {
    sendMessage(lastSenderId, message);
    Serial.println("Reply sent to last sender.");
  } else {
    Serial.println("No last sender to reply to.");
  }
}
