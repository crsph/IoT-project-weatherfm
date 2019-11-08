#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include "pitches.h"

#define PIN D2
#define LED_COUNT 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// Thuis netwerk
//const char* ssid = "Ziggo4295399";
//const char* password = "pUJ8umxmkrxx";

// School netwerk
const char* ssid = "IOT";
const char* password = "!HVAIOT!";

int buttonPin = D1;
int buttonPress = 0;
int buzzerPin = D8;
int ldrPin = A0;
int previousButtonState = HIGH;
int httpCode;
String httpActivateAudio = "http://863ed914.ngrok.io/audio_active.php";
String httpDeactivateAudio = "http://863ed914.ngrok.io/audio_inactive.php";

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(PIN, OUTPUT);
  pinMode(ldrPin, INPUT);

  Serial.begin(115200);

  strip.begin();
  setLedToYellow();
  strip.show();
  
  activateBuzzer();
  connectWifi();
}

void loop() {
  setLedToGreen();
  strip.show();

  int ldrStatus = analogRead(ldrPin);
//  Serial.println(ldrStatus);
  HTTPClient http; //Declare object of class HTTPClient
  
  int currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == LOW and previousButtonState == HIGH ) {
    http.begin(httpActivateAudio); //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
    httpCode = http.POST(""); //Send the request

    if (httpCode > 0) {
      String response = http.getString();

      Serial.println("----- HTTP success -----");
      Serial.println(httpCode);
      Serial.println(response);
    } else {
      String response = http.getString();
    
      Serial.print("----- Error occured ------");  
      Serial.println(httpCode);
      Serial.println(response);
    }
  }
  previousButtonState = currentButtonState;

  if (ldrStatus < 700) {
    http.begin(httpDeactivateAudio);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpCode = http.POST("");

    if (httpCode > 0) {
      String response = http.getString();

      Serial.println("----- HTTP success -----");
      Serial.println(httpCode);
      Serial.println(response);
    } else {
      String response = http.getString();
    
      Serial.print("----- Error occured ------");  
      Serial.println(httpCode);
      Serial.println(response);
    }
  }
  
  http.end();
}

void connectWifi(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting to ");
  Serial.print(ssid); 
  Serial.print("...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    digitalWrite(PIN, HIGH);
    delay(500);
    digitalWrite(PIN, LOW);
    delay(500);
//    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}

void activateBuzzer(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {

  // to calculate the note duration, take one second divided by the note type.
  // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  int noteDuration = 1000 / noteDurations[thisNote];
  tone(buzzerPin, melody[thisNote], noteDuration);

  // to distinguish the notes, set a minimum time between them.
  // the note's duration + 30% seems to work well:
  int pauseBetweenNotes = noteDuration * 1.30;
  delay(pauseBetweenNotes);

  // Stops the buzzer from playing a tone
  noTone(buzzerPin);
  }
}

void setLedToYellow() {
  for(int i = 0; i < 3; i++){
    strip.setPixelColor(i, 255, 177, 0);
  }  
}

void setLedToGreen() {
  for(int i = 0; i < 3; i++){
    strip.setPixelColor(i, 0, 255, 0);
  }  
}
