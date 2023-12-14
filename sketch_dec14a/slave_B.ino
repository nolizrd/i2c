#include <Wire.h>

int ownAddress = 1; // Адрес текущего слейва
unsigned char id = 'B'; // Имя текущего слейва

struct AddressTable {
  char name;
  int address;
};

AddressTable addressTable[] = {
  {'B', 1},
  {'C',2},
  {'D', 3}
};

char lastSender = ' '; // Переменная для хранения последнего отправителя

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
        lastSender = id; // Сохраняем идентификатор отправителя
        Serial.println("Message sent.");
      } else {
        Serial.println("Recipient not found in the address table.");
      }
    } else if (input.startsWith("reply ")) {
      if (lastSender != ' ') {
        String replyMessage = input.substring(6);
        sendMessage(getRecipientAddress(lastSender), replyMessage);
        Serial.println("Reply sent.");
      } else {
        Serial.println("No last sender to reply to.");
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
  char senderId = Wire.read();
  
  Serial.print("Sender's name: ");
  Serial.write(senderId);
  Serial.write("\n");
  Serial.print("Message: ");
  
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }  
  Serial.println();     
}
