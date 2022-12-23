//조도센서      => A0
//수위센서      => A1
//릴레이1(펌프)  => D3 
//릴레이1(LED)  => D4 

#include <WiFi.h>
#include <FirebaseESP32.h>

//수정해야 할 부분
#define ACCESS_ID "계정 ID"
#define WIFI_SSID "와이파이 ID" 
#define WIFI_PASSWORD "와이파이 PW"

#define FIREBASE_HOST "파이어베이스 HOST"  
#define FIREBASE_AUTH "파이어베이스 AUTH"

// Define Firebase Data Object
FirebaseData firebaseData;

// sensor pin
int cdsPin = A0;
int wtrPin = A1;
int rly1 = 3;
int rly2 = 4;

// Root Path
String path = "/smartFarm/"+ACCESS_ID; //firebase db에서 /smartFarm 을 기본 값으로 함.

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(cdsPin, OUTPUT);
  pinMode(wtrPin, OUTPUT);
  pinMode(rly1, OUTPUT);
  pinMode(rly2, OUTPUT);
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("================================================");
}
void loop() {
  int cds = analogRead(A0);
  int wtr = analogRead(A1);

  int setCds = Firebase.getFloat(firebaseData, path + "/cds/on") //path + cds + on경로에 있는 값이 되었을때 릴레이1를 킵니다.
  int setWtr = Firebase.getFloat(firebaseData, path + "/wtr/on") //path + wtr + on경로에 있는 값이 되었을때 릴레이2를 킵니다.
  
  Serial.print("cds = "); 
  Serial.print(cds);  //조도
  Serial.print(" | water = "); 
  Serial.print(wtr);  //수위
  Serial.println("");
  Serial.println("upload to database.");
  Firebase.setInt(firebaseData, path + "/cds/data", cds);   //조도센서 값 DB에 업로드
  Firebase.setInt(firebaseData, path + "/wtr/data", wtr);   //수위센서 값 DB에 업로드
  Serial.println("upload end.");
  if(cds < setCds) {           //자동모드
    if(cds < setCds) {         //설정한 빛보다 현재 빛이 작을때 LED 킴
      digitalWrite(rly1, HIGH);//LED 킴
    } else {
      digitalWrite(rly1, LOW); //LED 끔
    }
    if(pump) {                 //펌프 사용
      digitalWrite(rly2, HIGH);//펌프 킴
    } else {
      digitalWrite(rly2, LOW); //펌프 끔
    }
  } else {                     //수동모드
    if(light) {                //LED 사용
      digitalWrite(rly1, HIGH);//LED 킴
    } else {
      digitalWrite(rly1, LOW); //LED 끔
    }
    if(pump) {                 //펌프 사용
      digitalWrite(rly2, HIGH);//펌프 킴
    } else {
      digitalWrite(rly2, LOW); //펌프 끔
    }
  }
  delay(5000);                 //5초 대기
}

