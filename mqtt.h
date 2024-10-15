///////////////////////////////////////////////////////////
// MQTT functions
///////////////////////////////////////////////////////////

#include <PubSubClient.h>  // v 2.8
WiFiClient espClient;
PubSubClient client(espClient);

String      base_topic;

#include <ArduinoJson.h>    // v 6.21.3
StaticJsonDocument<256> jsonDoc;

/**************************************************
 * Callback function for MQTT
 * ***********************************************/
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
char* buff;
const char* nomsg="";

  Debug("MQTT message received [");
  Debug(String(topic));
  Debug("] : ");

  buff = (char*)malloc(length+1);
  memcpy(buff, payload, length);
  buff[length] = '\0';
  DebugLn("[" + String(buff) + "]");

  // is payload no NULL?
  if (length > 0) { 
    if (String(topic) == (base_topic + "/send")) {
      // received mqtt message with topic "send" and payload {"protocol": pp, "length": ll, "code": nnnnnn}
      DeserializationError error = deserializeJson(jsonDoc, buff, length);
      if (! error) {
        String aux;
        int protocol = -1;
        unsigned long code = 0;
        unsigned int length = 0;
        unsigned int pulselength = 0;
        aux = String(jsonDoc["protocol"]);
        if (aux.length()) {
          protocol=aux.toInt();
        }
        aux = String(jsonDoc["length"]);
        if (aux.length()) {
          length=aux.toInt();
        }
        aux = String(jsonDoc["code"]);
        if (aux.length()) {
          code=strtoul(aux.c_str(), NULL, 10);
        }
        aux = String(jsonDoc["pulselength"]);
        if (aux.length()) {
          pulselength=aux.toInt();
        }
        if ((protocol > 0) && (length > 0) && (code != 0) ) {

          setTXmode();

          mySwitch.setProtocol(protocol);
          if (pulselength > 0) mySwitch.setPulseLength(pulselength);
          mySwitch.send(code, length);
          DebugLn("sent code 0x" + String(code,HEX) + ", length " + String(length) + ", protocol " + String(protocol) + ", pulselength " + String(pulselength));

        }

      }
    }
    else if (String(topic) == (base_topic + "/sendtxdata")) {
      // received mqtt message with topic "send" and payload txindex
      sendTxData (atoi(buff)-1);
    } 

  }
  
  free(buff);
}

/*************************************************
 * MQTT reconnect function
 * **********************************************/
bool mqtt_reconnect() {
int res;

  if (!client.connected() ) {
    DebugLn("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = String(settings.data.name) + String(random(0xffff), HEX);
    // Attempt to connect

    if(strlen(settings.data.mqttuser)) {
      res = client.connect(clientId.c_str(), settings.data.mqttuser, settings.data.mqttpassword);
    }
    else {
      res = client.connect(clientId.c_str());
    }
    if (res) {
      DebugLn("MQTT connected");
      // once connect ....resubscribe
      base_topic = String(settings.data.mqtttopic) + "/" + String(settings.data.name);
      DebugLn("base topic: " + base_topic);
      client.subscribe((base_topic + "/#").c_str());
      DebugLn("subscribe: " + base_topic + "/#");
    } else {
      Debug ("MQTT reconnection failed, rc=");
      DebugLn (client.state());
    }
  }
  else // already connected
    res = true;
  return res;
}

/*************************************************
 * MQTT init function
 * **********************************************/
void mqtt_init() {

  client.setServer(settings.data.mqttbroker, settings.data.mqttport);
  DebugLn("setServer: " + String(settings.data.mqttbroker) + ", port: " +String(settings.data.mqttport));
  client.setCallback(mqtt_callback);
  mqtt_reconnect();

}

/*************************************************
 * MQTT send function
 * **********************************************/
void mqtt_send(String subtopic, String message, bool retain){

String topic = base_topic + "/" + subtopic;

  DebugLn("mqtt_send, topic: " + topic + ", payload: " + message);
  if(mqtt_reconnect() ) {
    // send data to topic
    client.publish(topic.c_str(), message.c_str(), retain);
    Debug("mqtt send [" );
    Debug(topic);
    Debug("]: ");
    DebugLn(message);
  }
}
