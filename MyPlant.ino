#include <SoftwareSerial.h>
#include <DFRobot_SHT20.h>
#include <ArduinoJson.h>
SoftwareSerial softSerial(7, 8);
String sendMsg = "";//发送的数据
String receiveMsg = "";//接收的指令
DFRobot_SHT20 sht20;
float humidity;
float temperature;
//int ASignal = A0;
//int soil;
int Relay1 = 1;
int Relay2 = 2;
int Relay3 = 3;
int Relay4 = 4;
int seconds;
int clientID = 5;
//int detectInterval = 600;


void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  softSerial.begin(9600);
  softSerial.listen();
  sht20.initSHT20();
  delay(100);
  sht20.checkSHT20();
  //pinMode(ASignal, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly
  receiveMsg = ReceiveInstruction();
  if(receiveMsg.length() != 0){
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(receiveMsg);
    if(!root.success()){
      //Serial.println("parseObject failed!");
    }else{
      int id = root["clientID"];
      int WF = root["WF"];
      int water = root["volume"];
      int serial = root["serial"];
      seconds = (water * 10) / 500;
      //seconds = water;
      if( WF== 1){
        PumpWork(seconds, serial);
        //delay(seconds * 1000);
      }else{
        DetectAirCondition(humidity, temperature);
        //soil = DetectSoilMoisture();
        sendMsg = GetData(temperature, humidity);
        //Serial.println(sendMsg);
        SendData(sendMsg);
        sendMsg = "";
      }
    }
  }
  delay(30000);
}


void SendData(String someData){//从WIFI模块发送数据
 softSerial.print(someData);
 delay(2000);
}


String ReceiveInstruction(){//从WIFI模块接收数据
  String instruction = "";
  if(softSerial.available()){
    size_t counti = softSerial.available();
    byte buf[counti];
    softSerial.readBytes(buf, counti);
    for(int i = 0; i < counti; i++){
      instruction += String((char)buf[i]);
    }
  }
  delay(2000);
  return instruction;
}

void DetectAirCondition(float &humd, float &temp){//检测空气温湿度
  humd = sht20.readHumidity();
  temp = sht20.readTemperature();
  delay(1000);
}

//int DetectSoilMoisture(){//检测土壤湿度
//  return analogRead(ASignal);
//}

void PumpWork(int second, int serial){//控制水泵工作
  digitalWrite(serial, HIGH);
  delay(second * 1000);
  digitalWrite(serial, LOW);
}

String GetData(float AT, float AM){//发送前形成数据字符串
  return String("{\"clientID\":" + String(clientID) + ",\"AT\":" + String(AT) + ",\"AM\":" + String(AM) + "}");
}
