/*
 Basic MQTT example with SSL
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define BUFFER_SIZE 2048

const char *ssid =  "DasInternetss";   // cannot be longer than 32 characters!
const char *pass =  "XTPMHB1311";   //

const uint8_t *pload = (uint8_t*)"Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis mi consectetuer lacinia. Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestibulum volutpat pretium libero. Cras id dui. Aenea";

// Update these with values suitable for your network.
IPAddress server(192, 168, 1, 88);

WiFiClientSecure wclient;
PubSubClient client(wclient, server);

void callback(const MQTT::Publish& pub)
{
  Serial.print(pub.topic());
  Serial.print(" => ");
  if (pub.has_stream())
  {
    uint8_t buf[BUFFER_SIZE];
    int read;
    while (read = pub.payload_stream()->read(buf, BUFFER_SIZE))
    {
      Serial.write(buf, read);
    }
    pub.payload_stream()->stop();
    Serial.println("");
  } 
  else
    Serial.println(pub.payload_string());

  client.publish("/mqtt/topic/0", pload, 512);
  client.publish("/mqtt/topic/0", pload, 1024);
  client.publish("/mqtt/topic/0", pload, 1536);
  client.publish("/mqtt/topic/0", pload, 2048);


}


void setup()
{
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  client.set_callback(callback);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected())
    {
      if (client.connect("arduinoClient")) 
      {
        client.publish(MQTT::Publish("/mqtt/topic/0", "hello world qos=0").set_qos(0));
        // client.publish(MQTT::Publish("/mqtt/topic/1", "hello world qos=1").set_qos(1));
        // client.publish(MQTT::Publish("/mqtt/topic/2", "hello world qos=2").set_qos(2));
        // client.subscribe("/mqtt/topic/0");
        // client.subscribe("/mqtt/topic/1");
        // client.subscribe("/mqtt/topic/2");
        client.subscribe("test");
      }
    }

    if (client.connected())
      client.loop();
  }
}

