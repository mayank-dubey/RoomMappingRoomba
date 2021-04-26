/*

*/
#include <IRremote.h>

int posX = 0; //initializes robot's current x position
int posY = 0; //initializes robot's curent y position
int position[3] = {posX, posY, 0}; //initializes robot's current coordinate
int grid[10][10][3]; //creates array of grid
int gridSizeX = sizeof(grid)/sizeof(grid[0]);
int gridSizeY = sizeof(grid[0])/sizeof(grid[0][0]);
String orientation = "up";
//char moves[] = {'w', 'a', 's', 'd'};

int trigPin = 7; //sets up pin to release sonar ping
int echoPin = 5; //sets up pin to recieve the info on the sonar ping
int ultra5V= 10;

//for remote control
int upFreq = 3936631966;
int downFreq = 356426642;
int leftFreq = 711683901;
int rightFreq = 2614621136;
int IR_RECEIVE_PIN = 11;


void movePosition(char m, int dist){
  int pos[] = {posX, posY};
  
  if(m=='w'){
    orientation = "up";
    //***move motors here***
    if(dist < 20){
      grid[posX][posY-1][2] = 1;
    }
    else{
      grid[posX][posY][2] = 2;
      posY -=1;
      //***move motors here***
    }
  }
  
  else if(m=='s'){
    orientation = "down";
    //***move motors here***
    if(dist < 20){
      grid[posX][posY+1][2] = 1;
    }
    else{
      grid[posX][posY][2] = 2;
      posY +=1;
      //***move motors here***
    }
  }
  
  else if(m=='d'){
    orientation = "right";
    //***move motors here***
    if(dist < 20){
      grid[posX+1][posY][2] = 1;
    }
    else{
      grid[posX][posY][2] = 2;
      posX +=1;
      //***move motors here***
    }
  }
  
  else if(m=='a'){
    orientation = "left";
    //***move motors here***
    if(dist < 20){
      grid[posX-1][posY][2] = 1;
    }
    else{
      grid[posX][posY][2] = 2;
      posX -=1;
      //***move motors here***
    }
  }
  position[0] = posX;
  position[1] = posY;
}



char decideMove(){
  char moves[] = {'w', 'a', 's', 'd'};
  int movesRank[4] = {10, 10, 10, 10};

  //prevents bot from leaving map and crashing into obstacles
  //up
  if(posY-1<0 || grid[posX][posY-1][2]==1){
    movesRank[0] = 0;
  }
  
  //left
  if(posX-1<0 || grid[posX-1][posY][2]==1){
    movesRank[1] = 0;
  }

  //down
  if(posY+1>gridSizeY-1 || grid[posX][posY+1][2]==1){
    movesRank[2] = 0;
  }

  //right
  if(posX+1>gridSizeX-1 || grid[posX+1][posY][2]==1){
      movesRank[3] = 0;
    }


  //prevents bot from alternating between two spots
  if(orientation=="down"){
    movesRank[0] -=2;
    movesRank[2] +=2;
  }
  if(orientation=="up"){
    movesRank[2] -=2;
    movesRank[0] +=2;
  }
  if(orientation=="right"){
    movesRank[1] -=2;
    movesRank[3] +=2;
  }
  if(orientation=="left"){
    movesRank[3] -=2;
    movesRank[1] +=2;
  }

  //prevents bot from retracing steps
  if(grid[posX][posY-1][2]==2){
    movesRank[0] -=3;
  }
  if(grid[posX+1][posY][2]==2){
    movesRank[3] -=3;
  }
  if(grid[posX-1][posY][2]==2){
    movesRank[1] -=3;
  }
  if(grid[posX][posY+1][2]==2){
    movesRank[2] -=3;
  }

  int big = 0;
  for(int i =0; i<4;i++){
    if(movesRank[i]>movesRank[big]){
      big = i;
    }
  }
  return moves[big];
}

void initializeGrid(){
  //gives grid coordinate values
  for(int y=0; y<gridSizeY; y++){
    for(int x=0; x<gridSizeX; x++){
      grid[x][y][0] = x; //defines x value for coordinate
      grid[x][y][1] = y; //defines y value for coordinate
      grid[x][y][2] = 0; //defines status of coordinat (0 = open, 1 = blocked, 2 = robot is there)
    }
  }
}

char remoteControl(){
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.decodedRawData);
    int freq = IrReceiver.decodedIRData.decodedRawData;
    if(freq == upFreq){
      return 'w';
    }
    else if(freq == downFreq){
      return 's';
    }
    else if(freq == leftFreq){
      return 'a';
    }
    else if(freq == rightFreq){
      return 'd';
    }
    IrReceiver.resume(); // Receive the next value
  }
}

int ultraSonicSense(){
  digitalWrite(ultra5V, HIGH);
  long duration;
  digitalWrite(trigPin, LOW);//idk what this does
  delayMicroseconds(2); //idk what this does
  digitalWrite(trigPin, HIGH); //sends out sonar ping
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);//resets trig pin
  duration = pulseIn(echoPin, HIGH);//takes recieves the ping
  int distance = duration/29/2; //distance in cm
  Serial.println(distance); //prints the distance
  return distance;
}

void setup() {
  Serial.begin(9600);
  posX = gridSizeX/2; //robot's current x position (puts it in the middle of map)
  posY = gridSizeY/2; //robot's curent y position (puts it in the middle of the map)
  position[0] = posX; 
  position[1] = posY;//robot's current coordinate
  pinMode(ultra5V, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  IrReceiver.begin(IR_RECEIVE_PIN, true); // Start the receiver
  initializeGrid();
}

void loop() {
  char movement = decideMove();
  int dist = ultraSonicSense();
  movePosition(movement, dist);
}