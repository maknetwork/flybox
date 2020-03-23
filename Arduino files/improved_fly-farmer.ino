// Load Wi-Fi library
#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present

char ssid[] = "WLANPK";            // your network SSID (name)
char pass[] = "Kndgr73Hsc$";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiEspServer server(80);
#include <AccelStepper.h>

#define motorInterfaceType 1
// Create a new instance of the AccelStepper class:
AccelStepper steppercd = AccelStepper(motorInterfaceType, 3, 2);
AccelStepper stepperpd = AccelStepper(motorInterfaceType, 5,4 );
AccelStepper stepperetd1 = AccelStepper(motorInterfaceType, 7, 6);
AccelStepper stepperetd2 = AccelStepper(motorInterfaceType, 9, 8);
AccelStepper stepperetd3 = AccelStepper(motorInterfaceType, 11, 10);

// Replace with your network credentials


// Set web server port number to 80
//Limit Switches

const int cdlimitfow = 22;     // the number of the pushbutton pin
const int cdlimitbac = 24;     // the number of the pushbutton pin
const int pdlimitfow= 26;     // the number of the pushbutton pin
const int pdlimitbac = 28;     // the number of the pushbutton pin
const int etd1limitfow = 30;     // the number of the pushbutton pin
const int etd1limitbac = 32;     // the number of the pushbutton pin
const int etd2limitfow = 34;     // the number of the pushbutton pin
const int etd2limitbac = 36;
const int etd3limitfow = 38;     // the number of the pushbutton pin
const int etd3limitbac = 40;
// Variable to store the HTTP request
String header;
int cdmove_finished=1;  // Used to check if move is completed
long cdinitial_homing=-1;  // Used to Home Stepper at startup
int pdmove_finished=1;  // Used to check if move is completed
long pdinitial_homing=-1;  // Used to Home Stepper at startup

int etd1move_finished=1;  // Used to check if move is completed
long etd1initial_homing=-1;  // Used to Home Stepper at startup
int etd2move_finished=1;  // Used to check if move is completed
long etd2initial_homing=-1;  // Used to Home Stepper at startup
int etd3move_finished=1;  // Used to check if move is completed
long etd3initial_homing=-1;  // Used to Home Stepper at startup



// Auxiliar variables to store the current output state
String outputcdState = "off";
String outputpdState = "off";
String outputetd1State = "off";
String outputetd2State = "off";
String outputetd3State = "off";
// Assign output variables to GPIO pins

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(9600);
  // Initialize the output variables as outputs
 Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
  // Set outputs to LOW
    pinMode(cdlimitfow, INPUT);
  pinMode(cdlimitbac, INPUT);
  pinMode(cdlimitfow, INPUT);
  pinMode(pdlimitbac, INPUT);
  pinMode(pdlimitfow, INPUT);
  pinMode(etd1limitbac, INPUT);
  pinMode(etd1limitfow, INPUT);
  pinMode(etd2limitbac, INPUT);
  pinMode(etd2limitfow, INPUT);
  pinMode(etd3limitbac, INPUT);
  pinMode(etd3limitfow, INPUT);
  delay(5); 
steppercd.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  steppercd.setAcceleration(100.0);  // Set Acceleration of Stepper
 stepperpd.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperpd.setAcceleration(100.0);  // Set Acceleration of Stepper
  stepperetd1.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperetd1.setAcceleration(100.0);  // Set Acceleration of Stepper
 stepperetd2.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperetd2.setAcceleration(100.0);  // Set Acceleration of Stepper
  stepperetd3.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)

  stepperetd3.setAcceleration(100.0);  // Set Acceleration of Stepper


//cd Homing
  while (digitalRead(cdlimitfow)== LOW) {  // Make the Stepper move CCW until the switch is activated   
   
  steppercd.moveTo(cdinitial_homing);  
    steppercd.run();
    cdinitial_homing++;
    delay(5);
}

  steppercd.setCurrentPosition(0);  // Set the current position as zero for now
  steppercd.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  steppercd.setAcceleration(100.0);  // Set Acceleration of Stepper
  cdinitial_homing=1;

  while (digitalRead(cdlimitfow)== HIGH) { // Make the Stepper move CW until the switch is deactivated
   steppercd.moveTo(cdinitial_homing);  // Set the position to move to
    cdinitial_homing--;  // Decrease by 1 for next move if needed
    steppercd.run();  // Start moving the stepper
    delay(5);
  }
  
  
  steppercd.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  steppercd.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  steppercd.setAcceleration(1000.0);  // Set Acceleration of Stepper

delay(5);
// PD Homing

  while (digitalRead(pdlimitfow)==LOW) {  // Make the Stepper move CCW until the switch is activated   
   
  stepperpd.moveTo(pdinitial_homing);  
    stepperpd.run();
   pdinitial_homing++;
    delay(5);
}

  stepperpd.setCurrentPosition(0);  // Set the current position as zero for now
  stepperpd.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperpd.setAcceleration(100.0);  // Set Acceleration of Stepper
  pdinitial_homing=1;

  while (digitalRead(pdlimitfow)==HIGH) { // Make the Stepper move CW until the switch is deactivated
   stepperpd.moveTo(pdinitial_homing);  // Set the position to move to
    pdinitial_homing--;  // Decrease by 1 for next move if needed
    stepperpd.run();  // Start moving the stepper
    delay(5);
  }
  
  stepperpd.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  stepperpd.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperpd.setAcceleration(1000.0);  // Set Acceleration of Stepper

delay(5);

//ETD1 Homing
  while (digitalRead(etd1limitfow)==LOW) {  // Make the Stepper move CCW until the switch is activated   
   
  stepperetd1.moveTo(etd1initial_homing);  
    stepperetd1.run();
    etd1initial_homing++;
    delay(5);
}

  stepperetd1.setCurrentPosition(0);  // Set the current position as zero for now
  stepperetd1.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperetd1.setAcceleration(100.0);  // Set Acceleration of Stepper
  etd1initial_homing=1;

  while (digitalRead(etd1limitfow)==HIGH) { // Make the Stepper move CW until the switch is deactivated
   stepperetd1.moveTo(etd1initial_homing);  // Set the position to move to
    etd1initial_homing--;  // Decrease by 1 for next move if needed
    stepperetd1.run();  // Start moving the stepper
    delay(5);
  }
  
  stepperetd1.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  stepperetd1.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperetd1.setAcceleration(1000.0);  // Set Acceleration of Stepper

delay(5);
  // ETD2 Homing
  
  while (digitalRead(etd2limitfow)==LOW) {  // Make the Stepper move CCW until the switch is activated   
   
  stepperetd2.moveTo(etd2initial_homing);  
    stepperetd2.run();
    etd2initial_homing++;
    delay(5);
}

  stepperetd2.setCurrentPosition(0);  // Set the current position as zero for now
  stepperetd2.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperetd2.setAcceleration(100.0);  // Set Acceleration of Stepper
  etd2initial_homing=1;

  while (digitalRead(etd2limitfow)==HIGH) { // Make the Stepper move CW until the switch is deactivated
   stepperetd2.moveTo(etd2initial_homing);  // Set the position to move to
    etd2initial_homing--;  // Decrease by 1 for next move if needed
    stepperetd2.run();  // Start moving the stepper
    delay(5);
  }
  
  stepperetd2.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  stepperetd2.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperetd2.setAcceleration(1000.0);  // Set Acceleration of Stepper
delay(5);


// ETD3 Homing



  while (digitalRead(etd3limitfow)==LOW) {  // Make the Stepper move CCW until the switch is activated   
   
  stepperetd3.moveTo(etd3initial_homing);  
    stepperetd3.run();
    etd3initial_homing++;
    delay(5);
}

  stepperetd3.setCurrentPosition(0);  // Set the current position as zero for now
  stepperetd3.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperetd3.setAcceleration(100.0);  // Set Acceleration of Stepper
  etd3initial_homing=1;

  while (digitalRead(etd3limitfow)==HIGH) { // Make the Stepper move CW until the switch is deactivated
   stepperetd3.moveTo(etd3initial_homing);  // Set the position to move to
    etd3initial_homing--;  // Decrease by 1 for next move if needed
    stepperetd3.run();  // Start moving the stepper
    delay(5);
  }
  
  stepperetd3.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  stepperetd3.setMaxSpeed(1000.0);      // Set Max Speed of Stepper (Faster for regular movements)
  stepperetd3.setAcceleration(1000.0);  // Set Acceleration of Stepper

  
  // Connect to Wi-Fi network with SSID and password
  
}

void loop(){
   WiFiEspClient client = server.available();  // listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /cd/start") >= 0) {
              Serial.println("GPIO 5 on");
           while (digitalRead(cdlimitbac)== LOW) { // Make the Stepper move CW until the switch is deactivated
   steppercd.moveTo(cdinitial_homing);  // Set the position to move to
    cdinitial_homing--;  // Decrease by 1 for next move if needed
    steppercd.run();  // Start moving the stepper
    delay(5);}

    steppercd.setCurrentPosition(0);  // Set the current position as zero for now
 
  cdinitial_homing=1;

  while (digitalRead(cdlimitfow)== LOW) { // Make the Stepper move CW until the switch is deactivated
   steppercd.moveTo(cdinitial_homing);  // Set the position to move to
    cdinitial_homing++;  // Decrease by 1 for next move if needed
    steppercd.run();  // Start moving the stepper
    delay(5);
  }
  }
             else if (header.indexOf("GET /pd/start") >= 0) {
              Serial.println("GPIO 5 off");
            while (digitalRead(pdlimitbac)== LOW)  {// Make the Stepper move CW until the switch is deactivated
   stepperpd.moveTo(pdinitial_homing);  // Set the position to move to
    pdinitial_homing--;  // Decrease by 1 for next move if needed
    stepperpd.run();  // Start moving the stepper
    delay(5);
  }
             stepperpd.setCurrentPosition(0);  // Set the current position as zero for now
  
  pdinitial_homing=1;

  while (digitalRead(pdlimitfow)== LOW) { // Make the Stepper move CW until the switch is deactivated
   steppercd.moveTo(pdinitial_homing);  // Set the position to move to
    pdinitial_homing++;  // Decrease by 1 for next move if needed
    stepperpd.run();  // Start moving the stepper
    delay(5);
  }}
            else if (header.indexOf("GET /etd1/start") >= 0) {
              Serial.println("GPIO 4 on");
          while (digitalRead(etd1limitbac)==LOW) {  // Make the Stepper move CCW until the switch is activated   
   
  stepperetd1.moveTo(etd1initial_homing);  
    stepperetd1.run();
    etd1initial_homing--;
    delay(5);
}
  }else if (header.indexOf("GET /etd2/start") >= 0) {
              Serial.println("GPIO 4 off");
            while (digitalRead(etd2limitbac)==LOW) { // Make the Stepper move CW until the switch is deactivated
   stepperetd2.moveTo(etd2initial_homing);  // Set the position to move to
    etd2initial_homing--;  // Decrease by 1 for next move if needed
    stepperetd2.run();  // Start moving the stepper
    delay(5);
  }
  
            }else if (header.indexOf("GET /etd3/start") >= 0) {
              Serial.println("GPIO 4 off");
            while (digitalRead(etd3limitbac)==LOW) { // Make the Stepper move CW until the switch is deactivated
   stepperetd3.moveTo(etd3initial_homing);  // Set the position to move to
    etd3initial_homing--;  // Decrease by 1 for next move if needed
    stepperetd3.run();  // Start moving the stepper
    delay(5);
  }
  
            }
           
  
            
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
                        client.println("<input id=\"input-id\" name=\"sasa\" value=\"hello\">");

            // Display current state, and ON/OFF buttons for GPIO 5  
           
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}




void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
