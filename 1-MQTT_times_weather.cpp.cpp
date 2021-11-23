#include <Arduino.h>
#include <U8g2lib.h> //載入庫
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid="CTK";
const char* password="------";
const char* mqtt_server="192.168.41.22";
//dht11
#define DHTPIN 13
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);
//訂閱topic
String SubDate="esp32/date";
String sw="esp32/switch";

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//自定義自行
const uint8_t myFont[1156] U8G2_FONT_SECTION("myFont") = 
  "&\0\3\2\4\5\3\4\6\17\20\0\376\16\376\16\376\0\0\0\0\0\302%\24\247\212h\64))%"
  "\221\222\306i\42%QR\322\24\0,\10Bn(\224D\1-\7\24\314(\206\0.\7\42\216(\206"
  "\0\60\21\246\212\250\264(\11\265)Q\242ML\242L\2\61\13\245\214\250\62)\11\373\64\10\62\16\246"
  "\212h\206$\24\323L\13\253\351\60\63\20\246\212h\206$\24\323hNE\61\31\22\0\64\20\246\212("
  "C-\211*Y\222%\303\230V\0\65\16\246\212(\16iu\220\323TL\206\4\66\16\246\212\250\246\60"
  "M\7%tL\206\4\67\13\246\212(\256\305\264\230\66\1\70\17\246\212h\206$\64&C\22:&C"
  "\2\71\16\246\212h\206$\64&\203\332\30M\0:\10r\236(\206x\10\0\0\0\4\377\377N-\35"
  "\13mps,\307r,\35\36C\61\24C\61\24\303\341!\254\346X\216\345X\12Y\32\42\14kp"
  "s\60\307\206!L\263\61\7\242\34Rb)\334\206\265\246FR\226#uH\7\346\35\2[\214%\17"
  "i\260sJ\216\14\17J\216\352h\70\334\71\17\17q\226cu\254\16\205Y\32fa\232E:\60$"
  "\0]\362\37\354|\60\36r\60\7s\60\312\221(G\242\341\20\345`\16\346`\216\351\230\216%\303A"
  "^t%\17i\360rBNx\320\322\34Is \316\241\341\220\206\71\24\346P\30\17\337\221\234\220\23"
  "rBN\210\1^\246%\17i\360\201\234\222#\303C\224\225\263\362\360\326\234\225\263A\316\11\311\60\250"
  "Q\32\326\322t\15\65u\7\6b\20)\17i\360\241$G\243\34\314\201\17Q\234\3q\16\304Y\66"
  "HYS\326%\314*a\26F\225\232\224d\245\244\34\316\5e\345\34\371n\60\36r@\7t@\7"
  "t\340A\7t@\7t@\7t\340A\7\2fB(\16k\360\241\34\16\7\261\226\14C\222\205\265"
  "\260\66|\216\262\70\312\222\341\26GY\224E\203\26\325\342\34Lr\64\3ft\62\16k\360\241\34\16"
  "\207d\70Di\30E\303\220Di\30%\303C\216E\321\60$Q\65\211\242aH\242j\62D\303\220"
  "D\325\34\310\222\34\10#\0g\10(\14m\360\206[\16d\71\220\345@\226\3\331p\313\201,\7\262"
  "\34\310\206[\16d\71\20\345H\224#u \321\241\4g\37\63\16i\260\262\34\312\242A\31\206$\213"
  "\262(\213\262(\213\6i\220\262(\213\262(\213\6)\213\262h\220\262(\33.Y\32\205Q\226\204I"
  "\232%j\5n\253.\377xprJ\66\14a\26\225\223\250-\351\226Db\226D\303\20fi\230\243"
  "\332p\312\242$\252EIT\213\222\250\26%Qi\70\10o\325.\376h\260\242\341\226\344@\226\14\207"
  "\60\7\222l\270\204\325$J\242,\231\266b\64U\242\322\220\14Q\16GI)\213*Q\230%Q\0"
  "}P.\17i\360\342\34\210\353@\234$\303%J\303!\316\201l\30\262\34\216rlX\206!N\322"
  "\34I\243\244\65JZ\223x\30r$M\0}\262/\17i\360rB\66\14R\30G\241\226\224*\245"
  "a\32\6\255\224Ea\26\25\207A\211*\341\240\64\346@\62$='\235\223\216\211\234&\0\215\357,"
  "\17i\360\241x\320\342,\33\262R\326\322\70\210\71\220&q\230\245Y\252$\323\240%aV\11\263J"
  "\230U\246,Z\7\35+\1\220#'\377x\360\241\70\7r \31\16R\234c\303\71\253L\303-\312"
  "\232\206[\234\3\311p\220\342\34\210+im\70\10\225\223'\374j\60\6i\330\242l\230\206-\312\206"
  "i\330\61i\230\244\60\222\206I\12#i\230\244\60\322\61\35It(\1\226c\61\16k\360\241p\20"
  "k\311pJ\303(\32\206\244\26U\242.Q\64\14I\226D\225,\211*Y\62\14\211\222\206I\64\34"
  "r$\314\221\60GB\0\226p-\16k\360\241p\20k\225,\312J-YR\312\62)\32\246:\20"
  "eiV\31\206$\313!%\31\16IZ\316\242t\30\222\70L\0\226\350&\377h\60~\7rBN"
  "\310\221\341!JKi\251%\213\262$j-\265dQ\226D\255\245\64K\242\34\213\0\226\362'\17i"
  "\260\206\203\16\345\310\360\240\244\261\264$K\216\344\330\222\354\344\341\240s\34>\246\71\220\3\351p\320\11"
  "\21\0\0";

//wifi連線
void wifiConn(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print("*");
    u8g2.firstPage();
    do{
      u8g2.setCursor(15, 30);
      u8g2.print("網路連結中...");
    }while (u8g2.nextPage());
    delay(500);
  }
  Serial.println("Connected successful!");
  u8g2.firstPage();
  do{
    u8g2.setCursor(15, 30);
    u8g2.print("網路已連結!");
  }while (u8g2.nextPage());
  delay(500);
}
//連線後自動執行收取訂閱主題和內容
void callback(char *topic, byte *msg, unsigned int length){
  String date, ledMsg;
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.print("message: ");
  for (int i = 0; i < length; i++){
    Serial.print(char(msg[i]));
    date += char(msg[i]);
    ledMsg += char(msg[i]);
  }
  Serial.println();
  delay(2000);
  float t=dht.readTemperature();
  float h=dht.readHumidity();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  if (String(topic)==SubDate){
    u8g2.firstPage();
    do{
      u8g2.setCursor(0, 15);
      u8g2.print("日期-時間");
      u8g2.setCursor(75, 15);
      u8g2.print(date.substring(0,4));
      u8g2.setCursor(110, 15);
      u8g2.print("年");
      u8g2.setCursor(0, 30);
      u8g2.print(date.substring(5,7));
      u8g2.setCursor(15, 30);
      u8g2.print("月");
      u8g2.setCursor(30, 30);
      u8g2.print(date.substring(8,10));
      u8g2.setCursor(45, 30);
      u8g2.print("日");
      u8g2.setCursor(60, 30);
      u8g2.print(date.substring(11,19));
      u8g2.setCursor(0, 45);
      u8g2.print("溫:");
      u8g2.setCursor(25, 45);
      u8g2.print(t,1);
      u8g2.setCursor(60, 45);
      u8g2.print("度");
      u8g2.setCursor(0, 60);
      u8g2.print("濕:");
      u8g2.setCursor(25, 60);
      u8g2.print(h,1);
      u8g2.setCursor(60, 60);
      u8g2.print("%");
    }while (u8g2.nextPage());
  
  }
  if (String(topic)=="esp32/switch"){
    if (ledMsg=="on"){
      digitalWrite(LED_BUILTIN, HIGH);
    }else if (ledMsg=="off"){
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
//如果沒連上mqtt則嚐試重連
void reConnMqtt(){
  while (! client.connected()){
    Serial.println("嚐試連線MQTT...");
    if (client.connect("esp32Client")){
      client.subscribe("esp32/date");
      client.subscribe("esp32/switch");
    }else{
      Serial.print("無法連線的原因是: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
  
void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  u8g2.begin();
  u8g2.setFont(myFont);
  u8g2.enableUTF8Print();
  wifiConn();
  dht.begin();
  //mqtt初始化
  client.setServer(mqtt_server, 1883);
  //監聽訂閱傳來的訊息
  client.setCallback(callback);
}

void loop(){
  if (! client.connected()){
    reConnMqtt();
  }
  client.loop();
}