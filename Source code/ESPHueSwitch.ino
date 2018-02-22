#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid      = "";				// Wifi name
const char* password  = "";				// Wifi password
const char* HueIP     = "";				// Hue Bridge IP	
const char* HueAPIkey = "";				// Hue API Token


int Led1    = 16;					// Led pin for command send, all functions are missing!
int Led2    = 5;					// Led pin for wifi connected, missing toggle if wifi is lost!
int Button1 = 4;					// Pin for button 1
int Button2 = 0;					// Pin for button 2
int Button3 = 2;					// Pin for button 3
int Button4 = 13;					// Pin for button 4

String OnCommand  = "{\"on\": true}";			// Command for turning on, can be customized with additional actions like color, transition time ect.
String OffCommand = "{\"on\": false}";			// Command for turning off, can be customized with additional actions like transition time ect.

int DeviceIDCommand1 = 3;				// Bulb ID on bridge for button 1
int DeviceIDCommand2 = 4;				// Bulb ID on bridge for button 2
int DeviceIDCommand3 = 5;				// Bulb ID on bridge for button 3
int DeviceIDCommand4 = 6;				// Bulb ID on bridge for button 4



unsigned long lastDebounceTime1 = 0;			// the last time the output pin was toggled
unsigned long lastDebounceTime2 = 0;			// the last time the output pin was toggled
unsigned long lastDebounceTime3 = 0;			// the last time the output pin was toggled
unsigned long lastDebounceTime4 = 0;			// the last time the output pin was toggled
unsigned long debounceDelay = 50;			// the debounce time; increase if the output flickers


int lastButtonState1 = HIGH;				// integer for button1 state
int lastButtonState2 = HIGH;				// integer for button2 state
int lastButtonState3 = HIGH;				// integer for button3 state
int lastButtonState4 = HIGH;				// integer for button4 state


void setup () {
  delay( 3000 );					// power-up safety delay, can be removed on final device!

  pinMode(Led1, OUTPUT);				// Set Led1 pinout
  pinMode(Led2, OUTPUT);				// Set Led2 pinout
  pinMode(Button1, INPUT_PULLUP);			// Set button1 as input with pullup
  pinMode(Button2, INPUT_PULLUP);			// Set button2 as input with pullup
  pinMode(Button3, INPUT_PULLUP);			// Set button3 as input with pullup
  pinMode(Button4, INPUT_PULLUP);			// Set button4 as input with pullup

  Serial.begin(115200);					// Start Serial for debugging, can be removed on final release!
  WiFi.begin(ssid, password);				// Connect to wifi!

  //Serial.print("connecting to ");			// Debugging info on wifi connection
  //Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {		// If Wifi is not connected, wait for it!
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");					// Print info about wifi connection
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  digitalWrite(Led2, HIGH);				// Set Led2 high to show wifi is on
}



void loop() {

  if (WiFi.status() == WL_CONNECTED) {			//Check WiFi connection status
    int reading1 = digitalRead(Button1);		// Read button 1
    int reading2 = digitalRead(Button2);		// Read button 2
    int reading3 = digitalRead(Button3);		// Read button 3
    int reading4 = digitalRead(Button4);		// Read button 4



    if (reading1 == LOW && lastButtonState1 == HIGH) {	// If button1 is pressed and it was not pressed before!
      lastButtonState1 = LOW;
      lastDebounceTime1 = millis();
      // Serial.println("Trykknap 1 trykket!");
      if(readStatus(DeviceIDCommand1) == 1){		// Check if bulb is on, then turn ot off!
        sendStatus(0, DeviceIDCommand1);		// Call sendStatus to turn off device!
      }else{
        sendStatus(1, DeviceIDCommand1);		// Call sendStatus to turn on device!
      }

    } else if (reading1 == HIGH && lastButtonState1 == LOW && ((millis() - lastDebounceTime1) > debounceDelay)) {
      lastButtonState1 = HIGH;

    } else if (reading1 == LOW) {
      lastDebounceTime1 = millis();
    }


    if (reading2 == LOW && lastButtonState2 == HIGH) {
      lastButtonState2 = LOW;
      lastDebounceTime2 = millis();
      // Serial.println("Trykknap 2 trykket!");
      if(readStatus(DeviceIDCommand2) == 1){
        sendStatus(0, DeviceIDCommand2);
      }else{
        sendStatus(1, DeviceIDCommand2);
      }

    } else if (reading2 == HIGH && lastButtonState2 == LOW && ((millis() - lastDebounceTime2) > debounceDelay)) {
      lastButtonState2 = HIGH;

    } else if (reading2 == LOW) {
      lastDebounceTime2 = millis();
    }


    if (reading3 == LOW && lastButtonState3 == HIGH) {
      lastButtonState3 = LOW;
      lastDebounceTime3 = millis();
      // Serial.println("Trykknap 3 trykket!");
      if(readStatus(DeviceIDCommand3) == 1){
        sendStatus(0, DeviceIDCommand3);
      }else{
        sendStatus(1, DeviceIDCommand3);
      }

    } else if (reading3 == HIGH && lastButtonState3 == LOW && ((millis() - lastDebounceTime3) > debounceDelay)) {
      lastButtonState3 = HIGH;

    } else if (reading3 == LOW) {
      lastDebounceTime3 = millis();
    }


    if (reading4 == LOW && lastButtonState4 == HIGH) {
      lastButtonState4 = LOW;
      lastDebounceTime4 = millis();
      // Serial.println("Trykknap 4 trykket!");
      if(readStatus(DeviceIDCommand4) == 1){
        sendStatus(0, DeviceIDCommand4);
      }else{
        sendStatus(1, DeviceIDCommand4);
      }

    } else if (reading4 == HIGH && lastButtonState4 == LOW && ((millis() - lastDebounceTime4) > debounceDelay)) {
      lastButtonState4 = HIGH;

    } else if (reading4 == LOW) {
      lastDebounceTime4 = millis();
    }


  }
}


int readStatus(int deviceID) {
  int deviceStatus;
  HTTPClient http;					//Declare an object of class HTTPClient

  String URLCreation = "http://";			// Create Hue API url
  URLCreation = URLCreation + HueIP;
  URLCreation = URLCreation + "/api/";
  URLCreation = URLCreation + HueAPIkey;
  URLCreation = URLCreation + "/lights/";
  URLCreation = URLCreation + deviceID;

  http.begin(URLCreation);				//Specify request destination
  int httpCode = http.GET();				//Send the request

  if (httpCode > 0) {					//Check the returning code
    String payload = http.getString();			//Get the request response payload
    Serial.println(payload);				//Print the response payload, removed on final release!

    if (payload.indexOf("\"on\":true,") >= 0) {
      deviceStatus = 1;
    } else {
      deviceStatus = 0;
    }
  }

  http.end();						//Close connection
  return deviceStatus;
}





void sendStatus(int stats, int deviceID) {
  HTTPClient http;					// Start Http request

  String URLCreation = "http://";			// Create Hue API url
  URLCreation = URLCreation + HueIP;
  URLCreation = URLCreation + "/api/";
  URLCreation = URLCreation + HueAPIkey;
  URLCreation = URLCreation + "/lights/";
  URLCreation = URLCreation + deviceID;
  URLCreation = URLCreation + "/state";

  http.begin(URLCreation);
  String req;
  if (stats == 1) {					// If turn on == true
    req = OnCommand;					// Set the put command to OnCommand
  } else {						// Else
    req = OffCommand;					// Set the put command to OffCommand
  }

  int httpCode = http.sendRequest("PUT", req);		// start connection and send HTTP header
  if (httpCode) {					// ask for file
    if (httpCode == 200) {				// file found at server, returning code 200
      String payload = http.getString();		// Get det payload of the response
      Serial.println(payload);				// Print the payload to client, removed on final release!
    }
  }
}
