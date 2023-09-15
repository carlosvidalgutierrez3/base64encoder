/*
 * Encodes two values of 2 bytes in base64 code. 
 */

#include <Arduino.h>

uint8_t hexCharToDecimal(char c); 
String binaryToBase64(uint8_t* binaryData, size_t length);
void hexToBinary(const String& hexStr, uint8_t* binaryData);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  uint16_t data_value1 = 1932;  // value between 0 and 65535 (2^16 - 1) <----- INPUT sensor id    (2-byte)          
  uint16_t data_value2 = 65;  // <-------------------------------------------- INPUT sensor data  (2-byte)
  uint8_t bytes[4]; // <------------------------------------------------------ EDIT here ...
  
  bytes[0] = data_value1%256;
  bytes[1] = data_value1/256;
  bytes[2] = data_value2%256;
  bytes[3] = data_value2/256; // <-------------------------------------------- ... and here to add bytes

  Serial.println("Input data:");
  Serial.print("Value 1: ");
  Serial.println(data_value1);
  Serial.print("Value 2: ");
  Serial.println(data_value2);
  Serial.println("");

  String hexStrAll = "";

  Serial.println("Each byte (Decimal) > Each byte (Hex)");
  for(int i=0; i<sizeof(bytes); i++){
    Serial.print(bytes[i]);
    Serial.print(" > ");
    
    // Convert decimal to hexadecimal string
    String hexStr = String(bytes[i], HEX);

    // If the result is a single digit, add a leading zero
    if (hexStr.length() == 1) hexStr = "0" + hexStr;
    Serial.println(hexStr);

    hexStrAll = hexStrAll + hexStr;
  }
  
  // Convert Hexadecimal to Binary
  uint8_t binaryData[hexStrAll.length() / 2];
  hexToBinary(hexStrAll, binaryData);

  // Convert Binary to Base64
  String base64Str = binaryToBase64(binaryData, hexStrAll.length() / 2);

  Serial.println("");
  Serial.println("Full string of bytes (Hex) > Full string of bytes (base64)");
  Serial.print(hexStrAll);
  Serial.print(" > ");
  Serial.println(base64Str);
}

void loop() {
  // Your code here
  
}

// Convert a single hexadecimal character to its corresponding decimal value
uint8_t hexCharToDecimal(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  else if (c >= 'A' && c <= 'F')
    return 10 + c - 'A';
  else if (c >= 'a' && c <= 'f')
    return 10 + c - 'a';
  else
    return 0;  // Invalid character
}

// Convert a hexadecimal string to binary
void hexToBinary(const String& hexStr, uint8_t* binaryData) {
  for (size_t i = 0; i < hexStr.length(); i += 2) {
    uint8_t msb = hexCharToDecimal(hexStr[i]);
    uint8_t lsb = hexCharToDecimal(hexStr[i + 1]);
    binaryData[i / 2] = (msb << 4) | lsb;
  }
}

String binaryToBase64(uint8_t* binaryData, size_t length) {
  const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  String base64Str;

  size_t i = 0;
  while (i < length) {
    uint8_t b0 = i < length ? binaryData[i++] : 0;
    uint8_t b1 = i < length ? binaryData[i++] : 0;
    uint8_t b2 = i < length ? binaryData[i++] : 0;

    uint8_t b64_0 = b0 >> 2;
    uint8_t b64_1 = ((b0 & 0x03) << 4) | (b1 >> 4);
    uint8_t b64_2 = ((b1 & 0x0F) << 2) | (b2 >> 6);
    uint8_t b64_3 = b2 & 0x3F;

    base64Str += base64chars[b64_0];
    base64Str += base64chars[b64_1];
    base64Str += base64chars[b64_2];
    base64Str += base64chars[b64_3];
  }

  // Handle padding
  
  if (length % 3 == 1) {
    base64Str[base64Str.length() - 1] = '=';
    base64Str[base64Str.length() - 2] = '=';
  } else if (length % 3 == 2) {
    base64Str[base64Str.length() - 1] = '=';
  }

  return base64Str;
}
