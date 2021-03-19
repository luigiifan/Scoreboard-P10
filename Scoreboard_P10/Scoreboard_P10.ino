#include <SPI.h>
#include <Wire.h>
#include <DMD2.h>
#include <fonts/BigNumberFull.h>
#include <fonts/System_Mid5x7.h>
#include <fonts/Mono5x7.h>
#include <fonts/System4x7.h>
#include <fonts/BIG7SEGMENT.H>
#include <fonts/FIXEDNUMS7x15.h>
#include <fonts/System4x7.h>
#include <SoftwareSerial.h>
SoftwareSerial bluetooth (3,2);

int Lscore = 0;
int Rscore = 0;
int Round = 1;
int Lfoul = 0;
int Rfoul = 0;
const unsigned long tInterval = 1000UL;
unsigned long pTime;
unsigned long cTime;
int menit = 0;
int saveMenit = menit;
int detik = 0;
int waktuHabis = 0;
int displayMenit;
int displayDetik;
boolean countStart = false;
char BT;
char dmdBuff[10];
char dmdBuff2[2];
SoftDMD dmd (2,1);

void setup() {
  dmd.setBrightness(20);
  dmd.begin();
  Serial.begin(9600);
  detik += 60 * menit;
  bluetooth.begin(9600);
  showScore();
}

void showScore(){
    dmd.selectFont(BigNumber);
    sprintf(dmdBuff, "%.2d", Lscore);
    dmd.drawString(1, 0, dmdBuff);
    dmd.selectFont(Mono5x7);
    sprintf(dmdBuff, "%d", Lfoul);
    dmd.drawString(17, 9, dmdBuff);
    sprintf(dmdBuff, "%d", Rfoul);
    dmd.drawString(42, 9, dmdBuff);
    dmd.selectFont(System_Mid5x7);
    sprintf(dmdBuff, "%d", Round);
    dmd.drawString(29, 8, dmdBuff);
    dmd.selectFont(BigNumber);
    sprintf(dmdBuff, "%.2d", Rscore);
    dmd.drawString(50, 0, dmdBuff);
    dmd.selectFont(System4x7);
    showTimer();
  }

void hideTimer(){
  dmd.selectFont(BigNumber);
  sprintf(dmdBuff, "%.2d", Lscore);
  dmd.drawString(1, 0, dmdBuff);
  dmd.selectFont(Mono5x7);
  sprintf(dmdBuff, "%d", Lfoul);
  dmd.drawString(17, 9, dmdBuff);
  sprintf(dmdBuff, "%d", Rfoul);
  dmd.drawString(42, 9, dmdBuff);
  dmd.selectFont(System_Mid5x7);
  sprintf(dmdBuff, "%d", Round);
  dmd.drawString(29, 8, dmdBuff);
  dmd.selectFont(BigNumber);
  sprintf(dmdBuff, "%.2d", Rscore);
  dmd.drawString(50, 0, dmdBuff);
}

void resetScore(){
  Lscore = 0;
  Rscore = 0;
  Lfoul = 0;
  Rfoul = 0;
  Round = 1;
  waktuHabis = 0;
  menit = saveMenit;
  detik = 0;
  detik += 60 * menit;
  countStart = false;
  showScore();
}

void teamSide(){
  if (BT == 'L'){
    hideTimer();
    dmd.drawLine(21,0,21,6);
    dmd.drawLine(20,1,20,5);
    dmd.drawLine(19,2,19,4);
    dmd.drawLine(18,3,18,3);
    delay(2000);
    hideTimer();
  }
  
  if (BT == 'M'){
    hideTimer();
    dmd.drawLine(42,0,42,6);
    dmd.drawLine(43,1,43,5);
    dmd.drawLine(44,2,44,4);
    dmd.drawLine(45,3,45,3);
    delay(2000);
    hideTimer();
  }
}

void syncApp(){
  String dataValue = String(Lscore) + "," + String(Lfoul) + "," + String(Rscore) + "," + String(Rfoul) + "," + String(Round) + "," + String(waktuHabis) + ",";
  bluetooth.println(dataValue);
  Serial.println(dataValue);
}

void MaxScore(){
  if (BT == 'A' && Lscore > 99){
    Lscore = 99;
  }
  if (BT == 'E' && Rscore > 99){
    Rscore = 99;
  }
  if (BT == 'C' && Lfoul > 9){
    Lfoul = 9;
  }
  if (BT == 'G' && Rfoul > 9){
    Rfoul = 9;
  }
  if (BT == 'I' && Round > 2){
    Round = 2;
  }
  showScore();
}

void MinScore(){
  if (BT == 'B' && Lscore < 0){
    Lscore = 0;
  }
  if (BT == 'F' && Rscore < 0){
    Rscore = 0;
  }
  if (BT == 'D' && Lfoul < 0){
    Lfoul = 0;
  }
  if (BT == 'H' && Rfoul < 0){
    Rfoul = 0;
  }
  if (BT == 'J' && Round < 1){
    Round = 1;
  }
  showScore();
}

void timeOut(){
  countStart = false;
  detik = 0;
  waktuHabis = 1;
  syncApp();
}

void showTimer(){
  dmd.selectFont(System4x7);
  displayMenit = detik/60;
  displayDetik = detik % 60;
  sprintf(dmdBuff, "%.2d", displayMenit);
  sprintf(dmdBuff2, "%.2d", displayDetik);
  dmd.drawFilledBox(22,0,43,6, GRAPHICS_OFF);
  dmd.drawString(22, -1, String(dmdBuff) + ":" + String(dmdBuff2));
}

void loop() {
  while(Serial.available()){
    BT = Serial.read();
    switch(BT){
      case 'A':
        Lscore++;
        MaxScore();
        syncApp();
        showScore();
        break;
      case 'B':
        Lscore--;
        MinScore();
        syncApp();
        showScore();
        break;
      case'C':
        Lfoul++;
        MaxScore();
        syncApp();      
        showScore();
        break;
      case 'D':
        Lfoul--;
        MinScore();
        syncApp();      
        showScore();       
        break;
      case 'E':
        Rscore++;
        MaxScore();
        syncApp();
        showScore();
        break;
      case 'F':
        Rscore--;
        MinScore();
        syncApp();
        showScore();
        break;   
      case 'G':
        Rfoul++;
        MaxScore();
        syncApp();
        showScore();        
        break;
      case 'H':
        Rfoul--;
        MinScore();
        syncApp();
        showScore();
        break;
      case 'I':
        Round++;
        MaxScore();
        syncApp();
        showScore();
        break;
      case 'J':
        Round--;
        MinScore();
        syncApp();
        showScore();
        break;
      case 'K':
        resetScore();
        syncApp();
        break;
//      case 'L':
//        teamSide();
//        break;
//      case 'M':
//        teamSide();
        break;
      case 'N':
        syncApp();
        break;
      case 'O':
        countStart = true;
        break;
      case 'P':
        countStart = false;
        break;
      case 'Q':
        menit++;
        saveMenit = menit;
        detik = 0;
        detik += 60 * menit;
        showTimer();
        break;
      case 'R':
        menit--;
        saveMenit = menit;
        detik = 0;
        detik += 60 * menit;
        showTimer();
        break;
    }
  }
  
  cTime = millis();
  if(cTime - pTime > tInterval){
    if(countStart == true){
      pTime = cTime;
      detik--;
      if(detik < 0) timeOut();
    showTimer();
    }
    pTime = cTime;
  }
}
