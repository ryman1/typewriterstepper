
int wire1 = 2;
int wire2 = 3;
int wire3 = 4;
int wire4 = 5;
//int stepdelay = 1000  ;
int stepdelay = 4000  ;
int fullturnsteps = 384;
int steps[8][4] = {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}};
int dialnumbercount = 40;
float dialnumbers[40];
float stepsperdialnumber = (float)fullturnsteps / (float)dialnumbercount;
float cstep = 0;
int num = 0;

int currentstep = 0;
int currentdiallocationstep = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(wire1, OUTPUT);     
  pinMode(wire2, OUTPUT);
  pinMode(wire3, OUTPUT);
  pinMode(wire4, OUTPUT);
  Serial.begin(115200);

  while (num < dialnumbercount) {
    dialnumbers[num] = round(cstep);
//    Serial.print("cstep\n");
//    Serial.print(cstep);
//    Serial.print("\n");
//    Serial.print(stepsperdialnumber);
//    Serial.print("\n");
    cstep += stepsperdialnumber;
    num++;
    
  }
}
void p(const char* s) {
  Serial.print("\n");
  Serial.print(s);
  Serial.print("\n");
}
//pitch is interstep delay in microseconds, length is length that this note is played in milliseconds
void playNote(int pitch, int length) {
  long notestarttime = millis();
  while (millis() - notestarttime < length){
    rotateClockwise(pitch/50);
  }
  delay(100);
}

void gotoDialNumber(int number, int counterclockwise){
  int howmanytostep;
  //if we're going counter clockwise and we're going to pass zero do some funky math to loop back around. 
  if (counterclockwise == 1){
    if (currentdiallocationstep > dialnumbers[number]){
//      p("loop 1");
//      howmanytostep = dialnumbers[number] + dialnumbercount - currentdiallocationstep;
      howmanytostep = dialnumbers[number] + fullturnsteps - currentdiallocationstep;
    }
    else {
//      p("loop 2");
      howmanytostep = dialnumbers[number] - currentdiallocationstep;
    }


  }
  else if (counterclockwise == 0){
    if (currentdiallocationstep < dialnumbers[number]){
//      p("loop 1");
      howmanytostep = fullturnsteps + currentdiallocationstep - dialnumbers[number];
    }
    else {
//      p("loop 2");
      howmanytostep = currentdiallocationstep - dialnumbers[number];
    }
  }
  
  
  currentdiallocationstep = dialnumbers[number];
  stepMotor(howmanytostep, counterclockwise);

}

void rotateClockwise(int waittime) { //2100 typewriter default waittime
  digitalWrite(wire1, HIGH);  
  delayMicroseconds(waittime);
  digitalWrite(wire4, LOW);  
  digitalWrite(wire2, HIGH);    
  delayMicroseconds(waittime);
  digitalWrite(wire1, LOW);
  digitalWrite(wire3, HIGH);
  delayMicroseconds(waittime);
  digitalWrite(wire2, LOW);
  digitalWrite(wire4, HIGH);
  delayMicroseconds(waittime);
  digitalWrite(wire3, LOW);
}

void rotateFullTurn(int counterclockwise){
  stepMotor(fullturnsteps, counterclockwise);
}
  

void playJingleBells(){
  playNote(8000, 250);
  playNote(8000, 250);
  playNote(8000, 500); 
  playNote(8000, 250);
  playNote(8000, 250);
  playNote(8000, 500);  
  playNote(8000, 250);
  playNote(6800, 250);
  playNote(10250, 250);
  playNote(9000, 250);
  playNote(8000, 2000); 
}

void stepMotor(int numberofsteps, int counterclockwise){
  Serial.print("\nnumberofsteps\n");
  Serial.print(numberofsteps);
//  Serial.print("\ncounterclockwise\n");
//  Serial.print(counterclockwise);
  while (numberofsteps > 0) {
    singleStep(counterclockwise);
    numberofsteps--;
  }
}


void singleStep(int counterclockwise){
  int dir;
  if (counterclockwise == 1) {
    dir = -1;
  }
  else {
    dir = 1;
  }
  currentstep += dir;

  if (dir == -1 && currentstep < 0){
    currentstep = 7;
  }
  if (dir == 1 && currentstep > 7) {
    currentstep = 0;
  }
  digitalWrite(wire1, steps[currentstep][0]);
  digitalWrite(wire2, steps[currentstep][1]);
  digitalWrite(wire3, steps[currentstep][2]);
  digitalWrite(wire4, steps[currentstep][3]);
  delayMicroseconds(stepdelay);
}

void turnAllWiresOff() {
  digitalWrite(wire1, 0);
  digitalWrite(wire2, 0);
  digitalWrite(wire3, 0);
  digitalWrite(wire4, 0);  
}


// the loop routine runs over and over again forever:
void loop() {  
//  stepMotor(90, 1);
//  delay(1000);
//  stepMotor(90, 1);
//  delay(1000);
//  stepMotor(90, 1);
//  delay(1000);
//  gotoDialNumber(30, 0);
//  delay(500);
//  gotoDialNumber(10, 0);
//  delay(500);
//  gotoDialNumber(20, 0);
//  delay(500);
//  gotoDialNumber(0, 0);
//  delay(500);
//  gotoDialNumber(30, 1);
//  delay(500);
//  gotoDialNumber(10, 1);
//  delay(500);
//  gotoDialNumber(20, 1);
//  delay(500);
//  gotoDialNumber(0, 1);
//  delay(500);
  rotateFullTurn(0);
  rotateFullTurn(0);
  gotoDialNumber(2, 0);
  rotateFullTurn(1);
  gotoDialNumber(20, 1);
  delay(500);
  gotoDialNumber(10, 0);
  turnAllWiresOff();
  delay(10000000);
}
