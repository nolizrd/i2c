#include <Wire.h>

int ownAddress = 2; // Адрес текущего слейва
unsigned char id = 'C'; // Имя текущего слейва
char lastSender = ' '; // Переменная для хранения последнего отправителя

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

        Serial.print("Addressee's name: ");
        Serial.write(recipientName);
        Serial.write("\n");
        Serial.print("Message: ");
        Serial.print(message);
        
      
        Serial.write("\nMessage delivered.\n");
      } else {
        Serial.println("Recipient not found in the address table.\n");
      }
    } else if (input.startsWith("reply ")) {
      if (lastSender != ' ') {
        String replyMessage = input.substring(6);
        sendMessage(getRecipientAddress(lastSender), replyMessage);
        Serial.println("Reply sent to the last sender.");
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
  if (Wire.available()) {
    char senderId = Wire.read();  // Прочитать идентификатор отправителя
    lastSender = senderId;  // Сохранить идентификатор последнего отправителя

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
}
