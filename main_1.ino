#define a_com 22 //trial originally 22
#define b_com 23
#define c_com 24

#define a_1 2
#define a_2 3
#define b_1 4
#define b_2 5
#define c_1 6
#define c_2 7
#define first 8

#define motor1 9
#define motor2 10

#include <Wire.h>
#include <string.h>

void setup() {
  pinMode(a_com,INPUT);
  pinMode(b_com,INPUT);
  pinMode(c_com,INPUT);

  pinMode(a_1,INPUT);
  pinMode(a_2,INPUT);
  pinMode(b_1,INPUT);
  pinMode(b_2,INPUT);
  pinMode(c_1,INPUT);
  pinMode(c_2,INPUT);
  Wire.begin();
  Serial.begin(19200);
  // Wire.beginTransmission(12);//
}

void reset(){
  for(int i=0;i<3;i++){
    if(i==0){
      // Wire.beginTransmission(10);
    }else if(i==1){
      Wire.beginTransmission(11);
      delay(1000);
    }else if(i==2){
      Wire.beginTransmission(12);
      delay(1000);
    }
    Wire.write(99+80);
    Wire.endTransmission();
    delayMicroseconds(3000);
  }
  bool checkA=false;
  bool checkB=false;
  bool checkC=false;
  // while(true){
  //   if(digitalRead(a_com)==HIGH){
  //     checkA=true;
  //   }else if(digitalRead(b_com)==HIGH){
  //     checkB=true;
  //   }else if(digitalRead(c_com)==HIGH){
  //     checkC=true;
  //   }
    
  // }
  delay(1000);
  Serial.println("done");

}

bool done_flag(int rack){
  while(true){
    if(rack==1){
    if(digitalRead(a_com)==HIGH){
      return true;
    }
  }else if(rack==2){
    if(digitalRead(b_com)==HIGH){
      return true;
    }
  }else if(rack==3){
    if(digitalRead(c_com)==HIGH){
      return true;
    }
  }
  }
  
}
void rail_in(int rack, int index){
  while(true){
    if(digitalRead(first)==LOW){
      break;
    }
  }
  digitalWrite(motor1,HIGH);
  digitalWrite(motor2,LOW);
  if(rack==1){
    if(index<=40 && index>0){
      while(true){
        
        if(digitalRead(a_1)==LOW){
          digitalWrite(motor1,LOW);
          digitalWrite(motor2,LOW);
          break;
        }
      }
    }
    if(index>40){
      while(true){
        
        if(digitalRead(a_2)==LOW){
          digitalWrite(motor1,LOW);
          digitalWrite(motor2,LOW);
          break;
        }
      }
    }
  }else if(rack==2){
    if(index<=40 && index>0){
      while(true){
        
        if(digitalRead(b_1)==LOW){
          digitalWrite(motor1,LOW);
          digitalWrite(motor2,LOW);
          break;
        }
      }
    }
    if(index>40){
      while(true){
        
        if(digitalRead(b_2)==LOW){
          digitalWrite(motor1,LOW);
          digitalWrite(motor2,LOW);
          break;
        }
      }
    }
  }else if(rack==3){
    if(index<=40 && index>0){
      while(true){
       
        if(digitalRead(c_1)==LOW){
          digitalWrite(motor1,LOW);
          digitalWrite(motor2,LOW);
          break;
        }
      }
    }
    if(index>40){
      while(true){
        
        if(digitalRead(c_2)==LOW){
          digitalWrite(motor1,LOW);
          digitalWrite(motor2,LOW);
          break;
        }
      }
    }
   
  }

}
void rail_out(int rack,int index){
  // Serial.println(index,DEC);
  digitalWrite(motor1,LOW);
  digitalWrite(motor2,HIGH);
  while(true){
    if(digitalRead(first)==LOW){
      digitalWrite(motor1,LOW);
      digitalWrite(motor2,LOW);
      break;
    }
    
  }
  
}


void execute_in(int rack, int index){
  rail_in(rack, index);
  if(rack==1){
    // Wire.beginTransmission(10);
  }else if(rack == 2){
    Wire.beginTransmission(11);
  }else if(rack==3){
    Wire.beginTransmission(12);
  }
  byte toSend = index+80;
  Wire.write(toSend);
  Wire.endTransmission();
  delay(2000);
  while(true){
    // Serial.println("waiting");
    if(done_flag(rack)){
      break;
    }
  }
  Serial.println("done");
}
void execute_out(int rack, int index){
  bool turnOn =false;
  if(rack==1){
    // Wire.beginTransmission(10);
  }else if(rack == 2){
    Wire.beginTransmission(11);
  }else if(rack==3){
    Wire.beginTransmission(12);
  }
  byte toSend = index+80;
  Wire.write(toSend);
  Wire.endTransmission();
  // delay(2000);
  while(true){
    // Serial.println("waiting");
    if(done_flag(rack)){
      // Serial.println("1");
      turnOn=true;
      break;
    }
  }
  if(turnOn){
    rail_out(rack, index);
  }
  Serial.println("done");
}
void loop() {
  char receivedChar[10];
  char terminator =';';
  int line=NULL;
  int rack_num;
  int index_num;

  if(Serial.available()>0){
    int bytesRead = Serial.readBytesUntil(terminator,receivedChar,sizeof(receivedChar)-1);
    if(bytesRead){
      receivedChar[bytesRead]='\0';
      line=atoi(receivedChar);
    }
  }


  // if(line!=NULL){
  //   delay(50000);
  //   Serial.println(line%100,DEC);
  // }
  if(line!=NULL){
    if(line==99){
      reset();
      
    }else if(line<-100 && line > -381){
      rack_num=line/100*-1;
      index_num=line%100;
      execute_out(rack_num,index_num);
      
    }else if(line>100 && line < 381){
      rack_num=line/100;
      index_num=line%100;
      // Serial.println(line,DEC);
      // Serial.println(index_num,DEC);
      execute_in(rack_num,index_num);
      
    }
  }
}
