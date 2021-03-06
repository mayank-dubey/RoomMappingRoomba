/*

*/
//#include <IRremote.h>

int posX = 0; //initializes robot's current x position
int posY = 0; //initializes robot's curent y position
int position[3] = {posX, posY, 0}; //initializes robot's current coordinate
int grid[3][3][3]; //creates array of grid
int gridSizeX = sizeof(grid)/sizeof(grid[0]);
int gridSizeY = sizeof(grid[0])/sizeof(grid[0][0]);
String orientation = "up";
//char moves[] = {'w', 'a', 's', 'd'};

int trigPin = 13; //sets up pin to release sonar ping
int echoPin = 12; //sets up pin to recieve the info on the sonar ping
//int ultra5V= 11;

// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

int moveTime = 500;
int rotateTime = 0.7854*moveTime;

//for remote control
/*int upFreq = 3936631966;
int downFreq = 356426642;
int leftFreq = 711683901;
int rightFreq = 2614621136;
int IR_RECEIVE_PIN = 11;*/



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

void turnLeft(){
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(rotateTime);
  
  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void turnRight(){
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(rotateTime);
  
  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void moveForward(){
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(moveTime);
  
  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void movePosition(char m){
  int pos[] = {posX, posY};
  
  if(m=='w'){
    if(orientation != "up"){
      //***rotate motors***
      if(orientation=="down" || orientation=="right"){
        turnLeft();
         
      }
      if(orientation =="down"){
        turnLeft();
         
      }
      else{
        turnRight();
         
      }
      
      Serial.print("rotating...   ");
      delay(3000);
      Serial.println("rotated up");
      orientation="up";
    }
    else{
      grid[posX][posY][2] = 2;
      posY -=1;
      //***move motors here***
      moveForward();
    }
  }
  
  else if(m=='s'){
   if(orientation != "down"){
    //***rotate motors***
    if(orientation =="up" || orientation == "left"){
      turnLeft();
       
    }
    if(orientation == "up"){
      turnLeft();
       
    }
    else{
      turnRight();
       
    }
    
      Serial.print("rotating...   ");
      delay(3000);
      Serial.println("rotated down");
    orientation="down";
   } 
   else{
     grid[posX][posY][2] = 2;
     posY +=1;
    //***move motors here***
    moveForward();
    }
  }
  
  else if(m=='d'){
   if(orientation != "right"){
    //**rotate motors***
    if(orientation == "left" || orientation == "down"){
      turnLeft();
       
    }
    if(orientation == "left"){
      turnLeft();
       
    }
    else{
      turnRight();
       
    }
    
      Serial.print("rotating...   ");
      delay(3000);
      Serial.println("rotated right");
    orientation = "right";
   }
   else{
     grid[posX][posY][2] = 2;
     posX +=1;
     //***move motors here***
     moveForward();
   }
  }
  
  else if(m=='a'){
    if(orientation != "left"){
      //***rotate motors***
      if(orientation =="right" || orientation == "up"){
        turnLeft();
         
      }
      if(orientation == "right"){
        turnLeft();
         
      }
      else{
        turnRight();
         
      }
      
      Serial.print("rotating...   ");
      delay(3000);
      Serial.println("rotated left");
      orientation="left";
    }
    else{
      grid[posX][posY][2] = 2;
      posX -=1;
      //***move motors here***
      moveForward();
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
  if(orientation == "up" && ultraSonicSense() <20){
      grid[posX][posY-1][2]=1;
  }
  if(posY-1<0 || grid[posX][posY-1][2]==1){
    movesRank[0] = 0;
  }
  
  //left
  if(orientation =="left" && ultraSonicSense() < 20){
      grid[posX-1][posY][2]=1;
  }
  if(posX-1<0 || grid[posX-1][posY][2]==1){
    movesRank[1] = 0;
  }

  //down
  if(orientation == "down" && ultraSonicSense() < 20){
      grid[posX][posY+1][2]=1;
  }
  if(posY+1>gridSizeY-1 || grid[posX][posY+1][2]==1){
    movesRank[2] = 0;
  }

  //right
  if(orientation =="right" && ultraSonicSense() < 20){
      grid[posX+1][posY][2]=1;
  }
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
  
  if(moves[big]=='w'){
    Serial.println("Move UP");
  }
  else if(moves[big]=='a'){
    Serial.println("Move LEFT");
  }
  else if(moves[big]=='s'){
    Serial.println("Move DOWN");
  }
  else if(moves[big]=='d'){
    Serial.println("Move RIGHT");
  }
  Serial.print("Rank: ");
  Serial.println(movesRank[big]);
  
  return moves[big];
}

/*char remoteControl(){
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
}*/

int ultraSonicSense(){
  //digitalWrite(ultra5V, HIGH);
  long duration;
  digitalWrite(trigPin, LOW);//idk what this does
  delayMicroseconds(2); //idk what this does
  digitalWrite(trigPin, HIGH); //sends out sonar ping
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);//resets trig pin
  duration = pulseIn(echoPin, HIGH);//takes recieves the ping
  int distance = duration/29/2; //distance in cm
  return distance;
}

void setup() {
  Serial.begin(9600);
  posX = gridSizeX/2; //robot's current x position (puts it in the middle of map)
  posY = gridSizeY/2; //robot's curent y position (puts it in the middle of the map)
  position[0] = posX; 
  position[1] = posY;//robot's current coordinate
  //pinMode(ultra5V, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //IrReceiver.begin(IR_RECEIVE_PIN, true); // Start the receiver
  initializeGrid();
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  int obstaclesX[3];
  int obstaclesY[3];
  for(int steps = 0; steps<15; steps++){
    Serial.println("\n\nDeciding...");
    delay(1000);
    char movement = decideMove();
    movePosition(movement);
  }
  int c=0;
  Serial.println("DONE");
  for(int x = 0; x <gridSizeX; x++){
    for(int y = 0; y <gridSizeY; y++){
      if(grid[x][y][2]==1){
        obstaclesX[c]=x;
        obstaclesY[c] = y;
        c++;
      }
    }
  }
  for(int i=0; i<3; i++){
    //x and y are flipped
    Serial.print(obstaclesY[i]);
    Serial.print(", ");
    Serial.println(obstaclesX[i]);
  }
}
