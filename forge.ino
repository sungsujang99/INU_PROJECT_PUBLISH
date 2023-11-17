#include <Wire.h>
//y
#define dirpin1 2
#define conpin1 3
#define dirpin2 4
#define conpin2 5
//x
#define dirpin3 6
#define conpin3 7

//comunication
#define main_com 22

//sensors
#define y1_ 14//y_1 end stop
#define x_ 16// x
#define y2_ 15//y_r

const int IN1=8;
const int IN2=9;
const int IN3=10;
const int IN4=11;
int get=-99;



void setup() {
  //pinMode(infrared0,INPUT);
  pinMode(y1_,INPUT_PULLUP);
  pinMode(y2_,INPUT_PULLUP);
  pinMode(x_,INPUT_PULLUP);
  pinMode(IN1, OUTPUT);    // IN1을 출력핀으로 설정합니다.
  pinMode(IN2, OUTPUT);    // IN2을 출력핀으로 설정합니다.
  pinMode(IN3, OUTPUT);    // IN3을 출력핀으로 설정합니다.
  pinMode(IN4, OUTPUT);    // IN4을 출력핀으로 설정합니다.
  pinMode(dirpin1,OUTPUT);
  pinMode(conpin1,OUTPUT);
  pinMode(dirpin2,OUTPUT);
  pinMode(conpin2,OUTPUT);
  pinMode(dirpin3,OUTPUT);
  pinMode(conpin3,OUTPUT);
  pinMode(main_com,OUTPUT);//send HIGH if complete send LOW if not
  Wire.begin(11);//10A(set at the last)11B, 12C
  Wire.onReceive(main_get);
  Serial.begin(19200);
  Serial.println("-----------(starting)-----------");
  //init_setup();
}
void go_down(int temp){
  digitalWrite(dirpin1,LOW);
  digitalWrite(dirpin2,HIGH);
  Serial.println("down");
  for(int i=0;i<temp;i++){
    digitalWrite(conpin1,HIGH);
    digitalWrite(conpin2,HIGH);
    delayMicroseconds(1000);
    digitalWrite(conpin1,LOW);
    digitalWrite(conpin2,LOW);
    delayMicroseconds(1000);
  }
}
void go_up(int temp){
  digitalWrite(dirpin1,HIGH);
  digitalWrite(dirpin2,LOW);
  Serial.println("up");
  for(int i=0;i<temp;i++){
    digitalWrite(conpin1,HIGH);
    digitalWrite(conpin2,HIGH);
    delayMicroseconds(1000);
    digitalWrite(conpin1,LOW);
    digitalWrite(conpin2,LOW);
    delayMicroseconds(1000);
   
  }
}

void go_2(int temp){
  digitalWrite(dirpin3,HIGH);
  Serial.println("go_2");
  for(int i=0;i<temp;i++){
    digitalWrite(conpin3,HIGH);
    delayMicroseconds(1000);
    digitalWrite(conpin3,LOW);
    delayMicroseconds(1000);
  }
}
void go_1(int temp){
  digitalWrite(dirpin3,LOW);
  Serial.println("go_1");
  for(int i=0;i<temp;i++){
    digitalWrite(conpin3,HIGH);
    delayMicroseconds(1000);
    digitalWrite(conpin3,LOW);
    delayMicroseconds(1000);
  }
}

void reset_axis(){
  Serial.println("reset_axis");
  bool x=false;
  digitalWrite(dirpin3,LOW);
  while(true){//axis reset
    if(digitalRead(x_)==HIGH){
      x=true;
    }
    if(x){ 
      break;
    }
    digitalWrite(conpin3,HIGH);
    delayMicroseconds(1000);
    digitalWrite(conpin3,LOW);
    delayMicroseconds(1000);
  }

  digitalWrite(dirpin1,LOW);
  digitalWrite(dirpin2,HIGH);
  bool y1=false,y2=false;
  while(true){//axis y reset
    if(digitalRead(y1_)==HIGH){
      y1=true;
    }
    if(digitalRead(y2_)==HIGH){
      y2=true;
    }
    if(!y1){
      digitalWrite(conpin1,HIGH);
      delayMicroseconds(1000);
      digitalWrite(conpin1,LOW);
      delayMicroseconds(1000);
    }
    if(!y2){
      digitalWrite(conpin2,HIGH);
      delayMicroseconds(1000);
      digitalWrite(conpin2,LOW);
      delayMicroseconds(1000);
    }
    if(y1&&y2){
      break;
    }
  }
  go_up(700);//zero to first floor
  go_2(4800);//zero to reset point
  forge_move(512);
  delay(10);
  forge_move(-512);
  delay(10);
  forge_move(-512);
  delay(10);
  forge_move(512);

  done_flag();
}


void forge_move(int temp){
  int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
  int motorSpeed = 1200;     // 스텝모터의 속도를 정할 수 있습니다.
  
  if(temp>0){
    for(int i=0;i<temp;i++){//cw
     for (int i = 0; i < 8; i++)                                          // 8번 반복합니다.
     {
        digitalWrite(IN1, bitRead(lookup[i], 0));             // IN1에 함수로부터 입력받은 out값을 넣어 모터를 제어합니다.
        digitalWrite(IN2, bitRead(lookup[i], 1));             // IN2에 함수로부터 입력받은 out값을 넣어 모터를 제어합니다.
        digitalWrite(IN3, bitRead(lookup[i], 2));             // IN3에 함수로부터 입력받은 out값을 넣어 모터를 제어합니다.
        digitalWrite(IN4, bitRead(lookup[i], 3));             // IN4에 함수로부터 입력받은 out값을 넣어 모터를 제어합니다.                                                   
        delayMicroseconds(motorSpeed);                      // 모터 스피드만큼 지연합니다.
      }
   }
  }else if(temp<0){//ccw
  temp=-temp;
   for(int i =0;i<temp;i++){
    for(int i = 7;i>=0; i--){
      digitalWrite(IN1, bitRead(lookup[i], 0));             // IN1에 함수로부터 입력받은 out값을 넣어 모터를 제어합니다.
      digitalWrite(IN2, bitRead(lookup[i], 1));             // IN2에 함수로부터 입력받은 out값을 넣어 모터를 제어합니다.
      digitalWrite(IN3, bitRead(lookup[i], 2));             // IN3에 함수로부터 입력받은 out값을 넣어 모터를 제어합니다.
      digitalWrite(IN4, bitRead(lookup[i], 3));             // IN4에 함수로부터 입력받은 out값을 넣어 모터를 제어합니다.                                                   
      delayMicroseconds(motorSpeed);
    }
   }
  }
  //dead
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);

  delay(100);
}

int cal_floor(int num){
  if(num>40){
    num=num-40;
  }
  if(num%8!=0){
    return (num/8);
  }else if(num%8==0){
    return (num/8)-1;
  }
}
int cal_index(int num){
  if(num>40){
    num=num-40;
  }
  if(num%8!=0){
    return (num%8)-1;
  }else if(num%8==0){
    return 7;
  }
}
void get_item(bool check){//true <40 false >40 gets the item from the reset point
  Serial.println("get_item");
  int steps=512;
  if(!check){
    steps=-steps;
  }
  Serial.println("get");
  go_2(830);
  go_down(650);
  forge_move(steps);
  go_up(75);
  delay(50);
  forge_move(-steps);
  go_up(575);
  go_1(830);
}
void leave_item(bool check){//have to be in the reset point
  Serial.println("leave_item");
  int steps=512;
  if(!check){
    steps=-steps;

    
  }
  go_2(830);
  go_down(575);
  Serial.println("-1");
  forge_move(steps);
  Serial.println("-2");
  go_down(75);
  delay(50);
  forge_move(-steps);
  go_up(650);
  go_1(830);
}
void initial_point(int floor, int index){
  Serial.println("initial_point");
  go_down(floor*750);
  delay(50);
  go_2(index*650);
}

void index_in() {
  Serial.println("index_in");
  bool isFront = false;
  int steps = 512;
  
  if (get <= 40) {
    isFront = true;
  } else {
    steps = -steps;
  }

  get_item(isFront); // gets the item

  int floor = cal_floor(get);
  int index = cal_index(get);

  go_up(floor * 750);
  Serial.println("floor");
  go_1(index * 650);
  Serial.println("goup");

  forge_move(steps);
  go_down(75);
  forge_move(-steps);

  go_up(75);
  initial_point(floor, index); // go to 1 floor, 1 index
  delay(100);
  done_flag();
}

void index_out() {
  Serial.println("index_out");
  bool isFront = false;
  int steps = 512;
  get = -get;

  if (get <= 40) {
    isFront = true;
  } else {
    steps = -steps;
  }

  int floor = cal_floor(get);
  int index = cal_index(get);

  go_up(floor * 750);
  go_1(index * 650);
  go_down(75);
  forge_move(steps);

  go_up(75);
  forge_move(-steps);

  initial_point(floor, index);
  leave_item(isFront);
  delay(100);
  done_flag();
}


void main_get(int num){
  Serial.println("main_get");
  int steps=512;
  // int get;

  byte toRe = Wire.read();
  get = (int)toRe - 80;
  
  Serial.print("get is ");
  Serial.println(get,DEC);
  // get=Serial.parseInt();//1 ~ 80 to put in -80 ~ -1 to out 99 to reset
  if(get==99 || (get>=-80 && get <=80 && get!=0)){
    if(get==99){
    reset_axis();
    // done_flag();
  }else if(get>=1 && get<=80){//in
    index_in();
    // done_flag();
  }else if(get>=-80 && get<=-1){//out
    index_out();
    // done_flag();
  }


  }
  
}


void done_flag(){
  Serial.println("done_flag");
  digitalWrite(main_com,HIGH);
  delay(1000);
  digitalWrite(main_com,LOW);
  get=-99;
}


void loop() {

  // digitalWrite(main_com,LOW);
  // //zero to 0 floor 700, each steps for a single floor 750
  // //axis x:zero to first index->4800 zero to rail -> 5630 //650
  // //only be receining index num
  // //reset code:-1
  // bool check=false; bool isFront=false; 
  // int steps=2048;
  // int get=-99;
  // //99->reset
  // get=Serial.parseInt();
  // Serial.println(get,DEC);
  // if(get==99){
  //   reset_axis();
  // }else if(get>=1 && get<=80 && !check){//in
  //   if(get<=40){
  //     isFront=true;
  //   }else{
  //     steps=-steps;
  //   }
  //   get_item(isFront);//gets the item
  //   int floor = cal_floor(get);
  //   int index = cal_index(get);
  //   go_up(floor*750);
  //   go_1(index*650);
  //   forge_move(steps);
  //   go_down(75);
  //   forge_move(-steps);
  //   delay(50);
  //   go_up(75);
  //   delay(50);
  //   initial_point(floor, index);//go to 1 floor, 1 index
  //   delay(100);
  //   done_flag();
  // }else if(get>=-80 && get<=-1 && !check){//out
  //   get=-get;
  //   if(get<=40){
  //     isFront=true;
  //   }else{
  //     steps=-steps;
  //   }
  //   int floor = cal_floor(get);
  //   int index = cal_index(get);
  //   go_up(floor*750);
  //   go_1(index*650);
  //   go_down(75);
  //   forge_move(steps);
  //   delay(50);
  //   go_up(75);
  //   forge_move(-steps);
  //   delay(50);
  //   initial_point(floor, index);
  //   leave_item(isFront);
  //   delay(100);
  //   done_flag();
    
  // }
}




