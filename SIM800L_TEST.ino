/**
* SIM800L EVB 
* - sim card module to send sms in TM/GLOBE
*
*/

#include <SoftwareSerial.h>

SoftwareSerial sim800l(8, 7); // TX,RX for Arduino and for the module it's TXD RXD, they should be inverted


void setup()
{
  sim800l.begin(9600); // Module baud rate, this is on max, it depends on the version
  Serial.begin(9600);
  delay(1000);

  // Set the module to SMS mode
  sim800l.println("AT+CMGF=1\r");
  delay(1000);

  // Set the module to show SMS text mode directly in the serial output
  sim800l.println("AT+CNMI=2,2,0,0,0\r");
  delay(1000);

  SendSMS("+639353787332", "hello jhon lloyd curz");
}

void loop() {

  while (Serial.available()) {
    const String input = Serial.readString();

    if (!isNumeric(input)) {
      // Serial.println("Please enter a valid integer..");
      SendSMS("+639353787332", "Please enter a valid integer..");
    }
    else if (input.toInt() >= 110) {
      // Serial.println("oh noes mabuto!");
      SendSMS("+639353787332", "oh noes mabuto!");
    }
    else if (input.toInt() < 110) {
      // Serial.println("gamatoting");
      SendSMS("+639353787332", "gamatoting");
    }
  }

  while (sim800l.available()) {
    // Request the actual SMS content
    sim800l.print("AT+CMGR=");
    delay(1000);

    // Read and print the received SMS content
    const String sms = sim800l.readString();
    Serial.println("Recieved SMS:");
    Serial.println(sms);
  }

  delay(1000); // Optional delay to avoid continuous printing
}


void SendSMS(String number, String message) {
  Serial.println("Sending SMS...");
  sim800l.print("AT+CMGF=1\r");
  delay(100);

  sim800l.print("AT+CMGS=\""+number+"\"\r");
  delay(500);

  sim800l.print(message);
  delay(500);

  sim800l.print((char)26);
  delay(500);

  sim800l.println();
  Serial.println("Text Sent.");
}


bool isNumeric(const String& str) {
  // Check each character in the string
  for (size_t i = 0; i < str.length(); i++) {
    // Allow for a negative sign at the beginning
    if (i == 0 && str[i] == '-') {
      continue;
    }

    // Allow for a decimal point
    if (i > 0 && str[i] == '.' && isdigit(str[i - 1])) {
      continue;
    }

    // If the character is not a digit, return false
    if (!isdigit(str[i])) {
      return false;
    }
  }
  // If all characters are digits, return true
  return true;
}


String getThirdLine(const String& input) {
  int firstNewline = input.indexOf('\n');

  // Check if a newline character was found
  if (firstNewline != -1) {
    int secondNewline = input.indexOf('\n', firstNewline + 1);

    // Check if a second newline character was found
    if (secondNewline != -1) {
      // Extract substring starting from the position after the second newline
      String thirdLine = input.substring(secondNewline + 1);

      // Trim leading and trailing whitespaces
      thirdLine.trim();

      return thirdLine;
    }
  }

  // Return an empty string if there is no third line
  return "";
}
