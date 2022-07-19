#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   26  
#define SS_PIN    5  
#define MISO_PIN  19 
#define MOSI_PIN  23 
#define SCK_PIN   18 

const char *ssid_Router = "*****"; //Enter the router name
const char *password_Router = "*****"; //Enter the router password

bool isRequest = false;


HTTPClient httpClient;

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  //Serial
  Serial.begin(115200);
  
  //wifiSetup
  Serial.println("Setup start");
  WiFi.begin(ssid_Router, password_Router);
  Serial.println(String("Connecting to ")+ssid_Router);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setup End");

  //RFID
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);
  mfrc522.PCD_Init();     // Init MFRC522
  delay(4);               // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
}

void loop() {
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
    }

  
  char mes1[50];
  sprintf(mes1, "%02x%02x%02x%02x", mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3]);

  
  // postRequest();
  StaticJsonDocument<JSON_OBJECT_SIZE(4)> json_array;
  char json_string[255];
  // JSONに変換したいデータを連想配列で指定する
  json_array["id"] = mes1;
  // JSONフォーマットの文字列に変換する
  serializeJson(json_array, json_string, sizeof(json_string));
  // HTTPClinetでPOSTする
  httpClient.begin("http://192.168.12.102:3000/api/save");
  // postするのはjsonなので、Content-Typeをapplication/jsonにする
  httpClient.addHeader("Content-Type", "application/json");
  // POSTしてステータスコードを取得する
  int status_code = httpClient.POST((uint8_t *)json_string, strlen(json_string));
  if (status_code == 200)
  {
    Serial.printf("Status200");
  }
  else
  {
    Serial.printf("NG");
  }
   httpClient.end();
  
  delay(3000);
}
