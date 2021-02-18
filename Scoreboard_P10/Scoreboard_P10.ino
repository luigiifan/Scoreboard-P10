#include <SPI.h>
#include <Wire.h>
#include <DMD2.h>
#include <fonts/BigNumberFull.h>
#include <fonts/System_Mid5x7.h>
#include <fonts/Mono5x7.h>
#include <fonts/System4x7.h>
#include <fonts/BIG7SEGMENT.H>
#include <fonts/FIXEDNUMS7x15.h>
#include <SoftwareSerial.h>
SoftwareSerial bluetooth (3,2); //TX, RX

/*LIST OF BYTE RECEIVE
 *
 *LSCORE+ = A 
 *LSCORE- = B
 *LFOUL+ = C
 *LFOUL- = D
 *-----------
 *RSCORE+ = E
 *RSCORE- = F
 *RFOUL+ = G
 *RFOUL- = H
 *-----------
 *ROUND+ = I
 *ROUND- = J
 *-----------
 *RESET_SCOREBOARD = K
 *-----------
 *ASIDE = L
 *BSIDE = M
 *
 */


//Scoreboard
int Lscore = 0;
int Rscore = 0;
int Round = 1;
int Lfoul = 0;
int Rfoul = 0;
int tukarSisi = 0;
char BT;
char dmdBuff[10];

SoftDMD dmd (2,1);

void setup() {
  dmd.setBrightness(20);
  dmd.begin();
  Serial.begin(9600);
  bluetooth.begin(9600);
  showScore();
}

//Display the current score
void showScore(){
  if (tukarSisi == 1){
    dmd.clearScreen();
    dmd.selectFont(FIXEDNUMS7x15);
    sprintf(dmdBuff, "%.2d", Lscore);
    dmd.drawString(48, 2, dmdBuff);
    dmd.drawLine(18,0,18,15);
    dmd.selectFont(Mono5x7);
    sprintf(dmdBuff, "%d", Lfoul);
    dmd.drawString(37, 9, dmdBuff);
    sprintf(dmdBuff, "%d", Rfoul);
    dmd.drawString(22, 9, dmdBuff);
    dmd.selectFont(System4x7);
    sprintf(dmdBuff, "%.2d", Round);
    dmd.drawString(28, -1, dmdBuff);
    dmd.selectFont(FIXEDNUMS7x15);
    dmd.drawLine(45,0,45,15);
    sprintf(dmdBuff, "%.2d", Rscore);
    dmd.drawString(1, 2, dmdBuff);
  }
  else{
    dmd.clearScreen();
    dmd.selectFont(FIXEDNUMS7x15);
    sprintf(dmdBuff, "%.2d", Lscore);
    dmd.drawString(1, 2, dmdBuff);
    dmd.drawLine(18,0,18,15);
    dmd.selectFont(Mono5x7);
    sprintf(dmdBuff, "%d", Lfoul);
    dmd.drawString(22, 9, dmdBuff);
    sprintf(dmdBuff, "%d", Rfoul);
    dmd.drawString(37, 9, dmdBuff);
    dmd.selectFont(System_Mid5x7);
    sprintf(dmdBuff, "%d", Round);
    dmd.drawString(29, -1, dmdBuff);
    dmd.selectFont(FIXEDNUMS7x15);
    dmd.drawLine(45,0,45,15);
    sprintf(dmdBuff, "%.2d", Rscore);
    dmd.drawString(48, 2, dmdBuff);
  }
}

//Reset Score
void resetScore(){
  if (BT == 'K'){
    Lscore = 0;
    Rscore = 0;
    Lfoul = 0;
    Rfoul = 0;
    Round = 1;
  }
showScore();
}

//Team Side Indicator
void teamSide(){
  if (BT == 'L'){ //A SIDE
    dmd.drawLine(23,0,23,6);
    dmd.drawLine(22,1,22,5);
    dmd.drawLine(21,2,21,4);
    dmd.drawLine(20,3,20,3);
    delay(500);
    showScore();
    delay(500);
    dmd.drawLine(23,0,23,6);
    dmd.drawLine(22,1,22,5);
    dmd.drawLine(21,2,21,4);
    dmd.drawLine(20,3,20,3);
    delay(500);
    showScore();
    delay(500);
    dmd.drawLine(23,0,23,6);
    dmd.drawLine(22,1,22,5);
    dmd.drawLine(21,2,21,4);
    dmd.drawLine(20,3,20,3);
    delay(3000);
    showScore();
  }
  if (BT == 'M'){
    dmd.drawLine(40,0,40,6);
    dmd.drawLine(41,1,41,5);
    dmd.drawLine(42,2,42,4);
    dmd.drawLine(43,3,43,3);
    delay(500);
    showScore();
    delay(500);
    dmd.drawLine(40,0,40,6);
    dmd.drawLine(41,1,41,5);
    dmd.drawLine(42,2,42,4);
    dmd.drawLine(43,3,43,3);
    delay(500);
    showScore();
    delay(500);
    dmd.drawLine(40,0,40,6);
    dmd.drawLine(41,1,41,5);
    dmd.drawLine(42,2,42,4);
    dmd.drawLine(43,3,43,3);
    delay(3000);
    showScore();
  }
  }

void syncApp(){
  String dataValue = String(Lscore) + "," + String(Lfoul) + "," + String(Rscore) + "," + String(Rfoul) + "," + String(Round) + ",";
  
  bluetooth.println(dataValue);
  Serial.println(dataValue);
}

//Maximum Score (changes depending on what game is being played.)
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

//Minimum Score (default: can't go -1 after 0)
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

void loop() {
  if(bluetooth.available()){ //bluetooth or Serial
    BT = bluetooth.read();
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
      case 'L':
        teamSide();
        syncApp();
        break;
      case 'M':
        teamSide();
        syncApp();
        break;
      case 'N':
        syncApp();
        break;
    }
  }
}// Void Loop
  
