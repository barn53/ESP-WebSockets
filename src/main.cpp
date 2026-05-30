#include <Arduino.h>

#include "wifi.h"

#include <WebSocketsClient.h>

#include <Hash.h>

unsigned long messageInterval = 5000;
bool connected = false;

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length)
{

    switch (type) {
    case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected!\n");
        connected = false;
        break;
    case WStype_CONNECTED: {
        Serial.printf("[WSc] Connected to url: %s\n", payload);
        connected = true;
        // send message to server when Connected
        // Serial.println("[WSc] SENT: Connected");
        // webSocket.sendTXT("Connected");
    } break;
    case WStype_TEXT:
        Serial.printf("[WSc] RESPONSE: %s\n", payload);
        break;
    case WStype_BIN:
        Serial.printf("[WSc] get binary length: %u\n", length);
        hexdump(payload, length);
        break;
    case WStype_PING:
        // pong will be send automatically
        Serial.printf("[WSc] get ping\n");
        break;
    case WStype_PONG:
        // answer to a ping we send
        Serial.printf("[WSc] get pong\n");
        break;
    }
}

void setup()
{
    Serial.begin(115200);

    // Serial.setDebugOutput(true);

    setupWiFi();

    //webSocket.begin("echo.websocket.org", 80, "/");
    //webSocket.beginSSL("stream.binance.com", 9443, "/ws");
    webSocket.beginSSL("stream.binance.com", 9443, "/ws");

    webSocket.onEvent(webSocketEvent);
}

bool once(false);
void loop()
{
    webSocket.loop();

    if (connected && !once) {
        String msg(R"({"method":"SUBSCRIBE","params":["btceur@trade"],"id": 1})");
        Serial.println(msg);
        webSocket.sendTXT(msg);
        once = true;
    }
}
