#include <SoftwareSerial.h>
#include <DFRobot_SHT20.h>
#include <ArduinoJson.h>
SoftwareSerial softSerial(7, 8);
String sendMsg = "";//发送的数据
String receiveMsg = "";//接收的指令
DFRobot_SHT20 sht20;
float humidity;
float temperature;
int ASignal0 = A0;
int ASignal1 = A1;
int ASignal2 = A2;
int ASignal3 = A3;
int soil0;
int soil1;
int soil2;
int soil3;
int Relay = 3;
int seconds;
int clientID = 2;
int detectInterval = 15;


void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  softSerial.begin(9600);
  softSerial.listen();
  sht20.initSHT20();
  delay(100);
  sht20.checkSHT20();
  pinMode(ASignal0, INPUT);
  pinMode(ASignal1, INPUT);
  pinMode(ASignal2, INPUT);
  pinMode(ASignal3, INPUT);
  pinMode(Relay, OUTPUT);
}

void loop() {
  for(int i = 0; i < 5; i++){//重复发送5次，用于准备清洗数据，取中值
    DetectAirCondition(humidity, temperature);
    soil0 = DetectSoilMoisture0();
    soil1 = DetectSoilMoisture1();
    soil2 = DetectSoilMoisture2();
    soil3 = DetectSoilMoisture3();
    sendMsg = GetData(temperature, humidity, soil0, soil1, soil2, soil3);
    //Serial.println(sendMsg);
    SendData(sendMsg);
    delay(1000);
  }
  sendMsg = "";
  for(int i = 0; i < detectInterval; i++){//延迟detectInterval分钟
    delay(60000);
  }
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

int DetectSoilMoisture0(){//检测土壤湿度
  return analogRead(ASignal0);
}

int DetectSoilMoisture1(){//检测土壤湿度
  return analogRead(ASignal1);
}

int DetectSoilMoisture2(){//检测土壤湿度
  return analogRead(ASignal2);
}

int DetectSoilMoisture3(){//检测土壤湿度
  return analogRead(ASignal3);
}


void PumpWork(int second){//控制水泵工作
  digitalWrite(Relay, HIGH);
  delay(second * 1000);
  digitalWrite(Relay, LOW);
}

String GetData(float AT, float AM, int SM, int SMM, int SMMM, int SMMMM){
  return String("{\"clientID\":" + String(clientID) + ",\"AT\":" + String(AT) + ",\"AM\":" + String(AM) + ",\"SM0\":" + String(SM) + ",\"SM1\":" + String(SMM) + ",\"SM2\":" + String(SMMM) + ",\"SM3\":" + String(SMMMM) + "}");
}



