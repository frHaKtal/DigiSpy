// _______   __    _______  __       _______..______   ____    ____ 
//|       \ |  |  /  _____||  |     /       ||   _  \  \   \  /   / 
//|  .--.  ||  | |  |  __  |  |    |   (----`|  |_)  |  \   \/   /  
//|  |  |  ||  | |  | |_ | |  |     \   \    |   ___/    \_    _/   
//|  '--'  ||  | |  |__| | |  | .----)   |   |  |          |  |     
//|_______/ |__|  \______| |__| |_______/    | _|          |__|     
//                          by _frHaKtal_

#include <SoftSerial.h>

SoftSerial sim800l(1, 2); // RX,TX for Arduino and for the module it's TXD RXD, they should be inverted
int test = 0;
String phone = "+33xxxxxxxxx"; // your phone numbers

void setup()
{
  sim800l.begin(9600);   //Module baude rate, this is on max, it depends on the version
  sim800l.println("AT\r\n");
  delay(1000);
  sim800l.println("AT+CLIP=1\r\n");//enable caller id
  delay(1000);
  //sendATcommand("AT+CLIP=1", "OK", 2000);
  sim800l.print("AT+CMIC=2,15\n");
  delay(1000);
  sim800l.println("AT+IPR=9600");
  delay(1000);
  sim800l.print("AT+CWHITELIST=3,1,"); sim800l.println(phone); //Phone number to whitelist
  delay(1000);
}
 
void loop()
{

//while ((sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0);

  if (test == 0 && (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 1){
    test=1;
    SendSMS(phone,"DigiSpy is Online...");
  }
sim800l.print("ATA\r");
delay(1000);
}

void SendSMS(String number, String text)
{
  sim800l.print("AT+CMGF=1\r"); //Set the module to SMS mode
  delay(1000);
  sim800l.print("AT+CMGS=\""); sim800l.print(number); sim800l.println("\"");  //Your phone number don't forget to include your country code, example +212123456789"
  delay(1000);
  sim800l.print(text);       //This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
  delay(1000);
  sim800l.print((char)26);// (required according to the datasheet)
  delay(1000);
  sim800l.println();
  delay(1000);
  
}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout) {
  uint8_t x = 0, answer = 0;
  char response[100];
  unsigned long previous;
  memset(response, '\0', 100); // initalize string
  delay(100);

  while (sim800l.available() > 0) {
    sim800l.read(); // clears the buffer
  }

  sim800l.println(ATcommand);

  x = 0;
  previous = millis();

  do {
    if (sim800l.available() != 0) {
      response[x] = sim800l.read();
      x++;
      if (strstr(response, expected_answer) != NULL) {
        answer = 1;
      }
    }
  } while ((answer == 0) && ((millis() - previous) < timeout));

  return answer;
}
 
