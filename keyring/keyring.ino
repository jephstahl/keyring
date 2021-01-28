// Variables
enum gameModes {SETUP, SHOW, LOST, FOUND};
Color gameColors[] =  {RED, YELLOW, GREEN, BLUE};
Color wipeColors[] = {OFF, RED, YELLOW, GREEN};
byte gameMode = SETUP;
byte id=0;
int connectedFaces[2];
int numBlinks;
int level = 0;
Timer colorDisplay;
enum syncStates {INERT, CONFIRM, RESOLVE};
byte syncState = INERT;


void setup() {
  randomize();
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (gameMode) {
    case SETUP:
      setupLoop();
      break;
    case SHOW:
      showLoop();
      break;
    case LOST:
      lostLoop();
      break;
    case FOUND:
      foundLoop();
      break;
  }
}

void setupLoop() {
  
  if (checkArrangement()==2) {                                      //see if connected to two blinks
    setColor(YELLOW);
    if (buttonSingleClicked()) {
      setColor(GREEN);
      id = 1;                                                       //set id to 1 on blink that button pushed
      setValueSentOnFace( id , connectedFaces[0] );                 //send id to next blink
    } 
    if (id !=1) {
      if (didValueOnFaceChange( connectedFaces[0] )) {              //if blink gets an id
        id = getLastValueReceivedOnFace(connectedFaces[0])+1;       //set its id to be 1 more
        setValueSentOnFace( id , connectedFaces[1] );               //send id to next blink
        int tempFace = connectedFaces[0];                           //swap connected face order so always comes in on 1
        connectedFaces[0] = connectedFaces [1];
        connectedFaces[1] = tempFace;
        setColor(CYAN);
        gameMode=SHOW;
      } else if (didValueOnFaceChange( connectedFaces[1] )) {       //same thing but if id comes in on 2nd connection
          id = getLastValueReceivedOnFace(connectedFaces[1])+1;
          setValueSentOnFace( id , connectedFaces[0] );
          setColor(CYAN);
          gameMode=SHOW;
      }
    }
  }
  else {
    return;
    //setColor(RED);
  }
  if (id ==1){
    if (didValueOnFaceChange(connectedFaces[1])) {
      numBlinks = getLastValueReceivedOnFace(connectedFaces[1]);
     
      setColor(CYAN);
      gameMode=SHOW;
    }
  }
}

int checkArrangement() {
  int connections = 0;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired( f )) {
      connectedFaces[connections]=f;
      connections++;
    }
  }
  return connections;  
}

void showLoop() {
  switch (level) {
    case (0):
      displayColor();
      break;
  }
  //randomize a colour
  //show the colour
  //when timer is up turn off colour
  //and start lost
}

void displayColor() {
  switch (syncState) {
    case INERT:
      int trueColor = random(3);                            // Get a random colour 0-3
      wipe(0);
      colorDisplay.set(2000);
      break;
    case CONFIRM:
      setColor(BLUE);//(gameColors[trueColor]);   //CODE DOESN'T GET HERE
    //  if (colorDisplay.isExpired()) {
    //    gameMode = LOST;
    //  }
      break;
  } 
}

void wipe(int wipeColor) {
  if (id == 1) {                                                // if start blink
    if (!didValueOnFaceChange(connectedFaces[1])){
      setValueSentOnFace(63 , connectedFaces[0] );
      //setColor(wipeColors[wipeColor]); 
    } else {
      
      //setColor(wipeColors[wipeColor]); 
      setColor(RED);                   //CODE GETS HERE
      syncState=CONFIRM;
    }
  } else if (didValueOnFaceChange(connectedFaces[1])) {
      setColor(YELLOW);//wipeColors[wipeColor]);        //CODE GETS HERE
      setValueSentOnFace(63 , connectedFaces[0] );
      syncState=CONFIRM;
    }
  
}

void lostLoop() {
  setColor(RED);
  //randomize 1 of the blinks to be lost
  //randomize a new colour (not the same one) an array of colours and just add a random number mod number of colours
  //display the new colours on button push
  //and start found
}

void foundLoop() {
  //wait for user to select a blink
  //check to see if it is the correct one
  //display win/loss animation
  //next level
}
