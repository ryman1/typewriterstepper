int wire1 = 2;
int wire2 = 3;
int wire3 = 4;
int wire4 = 5;
//microseconds between steps
int stepdelay = 4000;
//how many motor steps it take to spin 360 degrees
int fullturnsteps = 384;
//full sequence of wire power combinations to step through every coil
int steps[8][4] = {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}};
//how many numbers are on the combination lock
const int dialnumbercount = 40;
//this array will hold the exact number of steps from zero it takes to get to each number on the lock
float dialnumbers[dialnumbercount];
float stepsperdialnumber = (float)fullturnsteps / (float)dialnumbercount;
float cstep = 0;
int num = 0;
//currentstep indicates which of the steps in the "steps" array the motor is currently on
int currentstep = 0;
//the actual number of steps from zero we are currently at
int currentdiallocationstep = 0;

// the setup routine runs once when you press reset:
void setup() {
  pinMode(wire1, OUTPUT);
  pinMode(wire2, OUTPUT);
  pinMode(wire3, OUTPUT);
  pinMode(wire4, OUTPUT);
  Serial.begin(115200);

  while (num < dialnumbercount) {
    dialnumbers[num] = round(cstep);
    cstep += stepsperdialnumber;
    num++;
    
  }
}

//print to serial function
void p(const char* s) {
  Serial.print("\n");
  Serial.print(s);
}
//pitch is the interstep delay in microseconds, length is the  length that this note is played in milliseconds
void playNote(int pitch, int length) {
  long notestarttime = millis();
  while (millis() - notestarttime < length){
    rotateClockwise(pitch/50);
  }
  delay(100);
}

//go to specified number on dial by spinning in specified direction
void gotoDialNumber(int number, int counterclockwise){
  int howmanytostep;
  //if we're going counter clockwise and we're going to pass zero do some funky math to loop back around. If we're not going past zero just do the math normally.
  if (counterclockwise == 1){
    if (currentdiallocationstep > dialnumbers[number]){
      howmanytostep = dialnumbers[number] + fullturnsteps - currentdiallocationstep;
    }
    else {
      howmanytostep = dialnumbers[number] - currentdiallocationstep;
    }


  }
  
  //if we're going clockwise and we're going to pass zero do some funky math to loop back around. If we're not going past zero just do the math normally.
  else if (counterclockwise == 0){
    if (currentdiallocationstep < dialnumbers[number]){
      howmanytostep = fullturnsteps + currentdiallocationstep - dialnumbers[number];
    }
    else {
      howmanytostep = currentdiallocationstep - dialnumbers[number];
    }
  }
  
  
  currentdiallocationstep = dialnumbers[number];
  stepMotor(howmanytostep, counterclockwise);

}


//rotate clockwise a few steps using the specified interstep delay
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

void bruteForce(){

  for (int num1 = 0; num1 < dialnumbercount; num1++){
    rotateFullTurn(0);
    rotateFullTurn(0);
    gotoDialNumber(num1, 0);
    rotateFullTurn(1);
    for (int num2 = num1 + 1; num2 != num1; num2 = dialAdd(num2, 1)){
      gotoDialNumber(num2, 1);
      for (int num3 = num2 - 1; num3 != num2; num3 = dialAdd(num3, -1)){
        gotoDialNumber(num3, 0);
        p("pull test");
        Serial.print(num1);
        Serial.print(num2);
        Serial.print(num3);
      }
    }
  }
}

int dialAdd(int currentdialnumber, int numbertomove){
  int output = currentdialnumber + numbertomove;
  if (output < 0){
    output += dialnumbercount;
  }
  else if(output > dialnumbercount - 1){
    output -= dialnumbercount;
  }
  return output;
}
// the loop routine runs over and over again forever:
void loop() {  
  bruteForce()
}

