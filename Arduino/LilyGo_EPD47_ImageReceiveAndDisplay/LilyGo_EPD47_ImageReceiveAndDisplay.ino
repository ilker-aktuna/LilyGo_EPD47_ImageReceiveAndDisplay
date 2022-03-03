#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "Arduino.h"
#include <WiFiClient.h>
#include <ArduinoJson.h>

#include "epd_driver.h"

#include "base64.hpp"

#include "settings.h"

#include "opensans8b.h"

String appVersion="1.0";

GFXfont  currentFont;

uint8_t *framebuffer;

String ipaddr="";
enum alignment {LEFT, RIGHT, CENTER};

WebServer server(80);


void getAppVersion() {
  String postBody = server.arg("plain");
  Serial.setTimeout(10000);
  DynamicJsonDocument doc(10000);
  DeserializationError error = deserializeJson(doc, postBody);
    if (error) {
        Serial.print(F("Error parsing JSON "));
        Serial.println(error.c_str()); 
        String msg = error.c_str();
        server.send(400, F("text/html"),
                "Error in parsin json body! <br>" + msg);
    } else {
        JsonObject postObj = doc.as<JsonObject>(); 
        Serial.print(F("HTTP Method: "));
        Serial.println(server.method()); 
        if (server.method() == HTTP_POST) {
                DynamicJsonDocument doc1(512);
                doc1["status"] = "OK";
                doc1["version"] = appVersion;
                doc1["message"] = F("Fine!");
 
                Serial.print(F("Stream..."));
                String buf1;
                serializeJson(doc1, buf1);
 
                server.send(201, F("application/json"), buf1);
    }
    }
}

void turnOff() {
  String postBody = server.arg("plain");
  Serial.setTimeout(10000);
  DynamicJsonDocument doc(10000);
  DeserializationError error = deserializeJson(doc, postBody);
    if (error) {
        Serial.print(F("Error parsing JSON "));
        Serial.println(error.c_str()); 
        String msg = error.c_str();
        server.send(400, F("text/html"),
                "Error in parsin json body! <br>" + msg);
    } else {
        JsonObject postObj = doc.as<JsonObject>(); 
        Serial.print(F("HTTP Method: "));
        Serial.println(server.method()); 
        if (server.method() == HTTP_POST) {
            if (postObj.containsKey("name")) {
                epd_poweroff_all();
                esp_deep_sleep_start();
            }
        }
                DynamicJsonDocument doc1(512);
                doc1["status"] = "OK";
                doc1["message"] = F("Fine!");
 
                Serial.print(F("Stream..."));
                String buf1;
                serializeJson(doc1, buf1);
 
                server.send(201, F("application/json"), buf1);
    }
}

void clearScreen() {
  String postBody = server.arg("plain");
  Serial.setTimeout(10000);
  DynamicJsonDocument doc(10000);
  DeserializationError error = deserializeJson(doc, postBody);
    if (error) {
        Serial.print(F("Error parsing JSON "));
        Serial.println(error.c_str()); 
        String msg = error.c_str();
        server.send(400, F("text/html"),
                "Error in parsin json body! <br>" + msg);
    } else {
        JsonObject postObj = doc.as<JsonObject>(); 
        Serial.print(F("HTTP Method: "));
        Serial.println(server.method()); 
        if (server.method() == HTTP_POST) {
            if (postObj.containsKey("name")) {
              //epd_poweron();
              epd_clear();
              setFont(OpenSans8B);
    drawString(10, 500, "Connected to "+String(ssid), LEFT);
    drawString(310, 500, "IP address: "+ipaddr, LEFT);
    drawString(610, 500, "Ready, send image file...", LEFT);
              epd_poweroff();
            }
        }
                DynamicJsonDocument doc1(512);
                doc1["status"] = "OK";
                doc1["message"] = F("Fine!");
 
                Serial.print(F("Stream..."));
                String buf1;
                serializeJson(doc1, buf1);
 
                server.send(201, F("application/json"), buf1);
    }
}
void setImage() {
    String postBody = server.arg("plain");
    //Serial.println(postBody);
               epd_poweron();
              //epd_clear();
              Serial.setTimeout(10000);
    DynamicJsonDocument doc(300000);
    DeserializationError error = deserializeJson(doc, postBody);
    if (error) {
        // if the file didn't open, print an error:
        Serial.print(F("Error parsing JSON "));
        Serial.println(error.c_str());
 
        String msg = error.c_str();
 
        server.send(400, F("text/html"),
                "Error in parsin json body! <br>" + msg);
 
    } else {
        JsonObject postObj = doc.as<JsonObject>();
 
        Serial.print(F("HTTP Method: "));
        Serial.println(server.method());
 
        if (server.method() == HTTP_POST) {
            if (postObj.containsKey("name") && postObj.containsKey("type")) {

              const char* encoded = postObj["type"];
              const int x = postObj["startx"];
              const int y = postObj["starty"];
              const int x2 = postObj["width"];
              const int y2 = postObj["height"];

              Serial.printf("decodedLength: %d\n",BASE64::decodeLength(encoded));
              uint8_t raw[BASE64::decodeLength(encoded)];
              BASE64::decode(encoded, raw);

               Rect_t area = {
                .x = x,
                .y = y,
                .width = x2,
                .height =  y2
              };

              epd_poweron();
              if(x==0 && y==0) {
                epd_clear();
              }
              epd_draw_grayscale_image(area, (uint8_t *) raw);
              epd_poweroff();

 
                Serial.println(F("done."));


                DynamicJsonDocument doc1(512);
                doc1["status"] = "OK";
                doc1["message"] = F("Fine!");
 
                Serial.print(F("Stream..."));
                String buf1;
                serializeJson(doc1, buf1);
 
                server.send(201, F("application/json"), buf1);
                Serial.print(F("done."));

 
            }else {
                DynamicJsonDocument doc(512);
                doc["status"] = "KO";
                doc["message"] = F("No data found, or incorrect!");
 
                Serial.print(F("Stream..."));
                String buf;
                serializeJson(doc, buf);
 
                server.send(400, F("application/json"), buf);
                Serial.print(F("done."));
            }
        }
    }
}

void restServerRouting() {
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the EPD47 Display Web Server"));
    });
    server.on(F("/setImage"), HTTP_POST, setImage);
    server.on(F("/clearScreen"), HTTP_POST, clearScreen);
    server.on(F("/turnOff"), HTTP_POST, turnOff);
    server.on(F("/appVersion"), HTTP_POST, getAppVersion);
}

// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void) {
  Serial.begin(115200);
  
    epd_init();

    framebuffer = (uint8_t *)heap_caps_malloc(EPD_WIDTH * EPD_HEIGHT / 2, MALLOC_CAP_SPIRAM);
    memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

    epd_poweron();
    epd_clear();
    

    delay(3000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  ipaddr=WiFi.localIP().toString();;

    setFont(OpenSans8B);
    drawString(10, 500, "Connected to "+String(ssid), LEFT);
    drawString(310, 500, "IP address: "+ipaddr, LEFT);
    drawString(610, 500, "Ready, send image file...", LEFT);

    epd_poweroff();
 
  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane esp8266.local
  if (MDNS.begin(mdnsname)) {
    Serial.println("MDNS responder started");
  }

  // Set server routing
  restServerRouting();
  // Set not found response
  server.onNotFound(handleNotFound);
  // Start server
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void) {
 server.handleClient();
}

void setFont(GFXfont const &font) {
  currentFont = font;
}

void drawString(int x, int y, String text, alignment align) {
  char * data  = const_cast<char*>(text.c_str());
  int  x1, y1; 
  int w, h;
  int xx = x, yy = y;
  get_text_bounds(&currentFont, data, &xx, &yy, &x1, &y1, &w, &h, NULL);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;
  int cursor_y = y + h;
  write_string(&currentFont, data, &x, &cursor_y, framebuffer);
  epd_draw_grayscale_image(epd_full_screen(), framebuffer); 
}
