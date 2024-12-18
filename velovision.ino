#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>

const int trigPin1 = 25;
const int echoPin1 = 26;

const int trigPin2 = 27;
const int echoPin2 = 32;

const int trigPin3 = 33;
const int echoPin3 = 34;

long duration1;
long duration2;
long duration3;

int distance1;
int distance2;
int distance3;

Servo myServo1;
Servo myServo2;
Servo myServo3;

const int minUs = 500;
const int maxUs = 2500;

#define SERVO_PIN1 12
#define SERVO_PIN2 13
#define SERVO_PIN3 14

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

const char* serverUrl = "http://localhost:3000/endpoint";

void setup() {
  myServo1.attach(SERVO_PIN1);
  myServo2.attach(SERVO_PIN2);
  myServo3.attach(SERVO_PIN3);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo1.write(angle);
    myServo2.write(angle);
    myServo3.write(angle);
    distance1 = calculateDistance1();
    distance2 = calculateDistance2();
    distance3 = calculateDistance3();
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrl);

      String jsonData = "{\"distance1\": " + String(distance1) + ", \"distance2\": " + String(distance2) + ", \"distance3\": " + String(distance3) + "}";

      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(jsonData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Server Response: " + response);
      } else {
        Serial.println("Error on sending POST: " + String(httpResponseCode));
      }

      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }

    delay(1000);
  }

  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo1.write(angle);
    myServo2.write(angle);
    myServo3.write(angle);
    distance1 = calculateDistance1();
    distance2 = calculateDistance2();
    distance3 = calculateDistance3();
    delay(100);
  }

  Serial.print("Distance 1: ");
  Serial.println(distance1);

  Serial.print("Distance 2: ");
  Serial.println(distance2);

  Serial.print("Distance 3: ");
  Serial.println(distance3);

  delay(1000);
}

int calculateDistance1() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  return distance1;
}

int calculateDistance2() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  return distance2;
}

int calculateDistance3() {
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3 * 0.034 / 2;
  return distance3;
}