#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0x02, 0x01, 0x00 };
//IPAddress ip(172, 16, 0, 100);
//IPAddress server(10, 30, 4, 210);
char server[] = "broker.mqttdashboard.com";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("leoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("leo", "hello world");
      // ... and resubscribe
      client.subscribe("leo");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 1 seconds before retrying
      delay(1000);
    }
  }
}

void setup()
{
  Serial.begin(57600);

  pinMode(4, OUTPUT); // 4 = Trig
  pinMode(3, INPUT); // 2 = Echo

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  //  client.loop();
  
  digitalWrite(4, HIGH);
  delayMicroseconds(10);
  digitalWrite(4, LOW);
  int pulseWidth = pulseIn(3, HIGH);
  Serial.print("Pulse Width: ");
  Serial.println(pulseWidth);
  long distance = pulseWidth / 29 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  
  String distance_str = String(distance);
  char distance_char[10];
  distance_str.toCharArray(distance_char,10);

  String analog_str = String(analogRead(A0));
  char analog_char[10];
  analog_str.toCharArray(analog_char,10);

  client.publish("leo", distance_char);
  client.publish("a0", analog_char);
  Serial.println("//////////////////////////////////////");

  delay(1000);
}
