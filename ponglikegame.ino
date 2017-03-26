//Thorn (Cronck)
//February 18th, 2017
//pong_game_clone

//Include the Arduboy Library
#include <Arduboy2.h>
#include <ArduboyPlaytune.h>
//Initialize the arduboy device 
Arduboy2 arduboy;
ArduboyPlaytune tunes(arduboy.audio.enabled);

// Variables are added before setup to initialize our game programs
// we can add value to a varible at the same time
int gamestate = 0;
int justpressed = 0;
int ballx = 62;
int bally = 0;
int ballsize = 4;
int ballright = 1;
int balldown = 1;
int paddlewidth = 4;
int paddleheight = 9;
int playerx = 0;
int playery = 0;
int computerx = 127 - paddlewidth;
int computery = 0;
int playerscore = 0;
int computerscore = 0;
void resetgame() {
  ballx = 63;
  playerscore = 0;
  computerscore = 0;
}

// Sounds
// Trying to duplicate GameBoy start up 
const byte PROGMEM start [] = {
0x90,0x30, 0,107, 0x80, 0x90,0x60, 1,244, 0x80, 0xf0};
// mario coin sound'ish
const byte PROGMEM point [] = {
0x90,83, 0,75, 0x80, 0x90,88, 0,225, 0x80, 0xf0};
// npc point should be a negitive sound
const byte PROGMEM hit [] = { 
0x90,60, 0,31, 0x80, 0x90,61, 0,31, 0x80, 0x90,62, 0,31, 0x80, 0xf0};


//The setup() function runs once when you turn your Arduboy on
void setup() {
  //Start the Arduboy properly and display the Arduboy logo
  arduboy.begin();
  // audio setup
  tunes.initChannel(PIN_SPEAKER_1);
  #ifndef AB_DEVKIT
  // if not a DevKit
  tunes.initChannel(PIN_SPEAKER_2);
  #else
  // if it's a DevKit
  tunes.initChannel(PIN_SPEAKER_1); // use the same pin for both channels
  tunes.toneMutesScore(true);       // mute the score when a tone is sounding
  #endif
  //Seed the random number generator
  srand(7/8);
  //Set the game to 60 frames per second
  arduboy.setFrameRate(60);
  //trying to get sound, this is the gameboy startup sound
  tunes.playScore(start);
  delay(1500);
  //Get rid of the Arduboy logo and clear the screen
  arduboy.clear();
}

//The loop() function repeats forever after setup() is done
void loop() {
  
  //Prevent the Arduboy from running too fast
  if(!arduboy.nextFrame()) {
    return;
  }
  
  //Clear whatever is printed on the screen
  arduboy.clear();
  
  //Gameplay code goes here
  switch( gamestate ) {
    case 0:
      //Title screen
      arduboy.setCursor(30, 25);
      arduboy.print("PONG Clone");
      //Change the gamestate
      if(arduboy.pressed(A_BUTTON) and justpressed == 0) {
        justpressed = 1;
        gamestate = 1;
      }
      break;
    case 1:
      //Gameplay screen
      arduboy.setCursor(20, 0);
      arduboy.print(playerscore);
      arduboy.setCursor(101, 0);
      arduboy.print(computerscore);
      
      //Draw the ball
      arduboy.fillRect(ballx, bally, ballsize, ballsize, WHITE);
      
      //Move the ball right
      if(ballright == 1) {
        ballx = ballx + 1;
      }
      //Move the ball left
      if(ballright == -1) {
        ballx = ballx - 1;
      }
      
      //Move the ball down
      if(balldown == 1) {
        bally = bally + 1;
      }
      //Move the ball up
      if(balldown == -1) {
        bally = bally - 1;
      }
      //Reflect the ball off of the top of the screen
      if(bally == 0) {
        balldown = 1;
      }
      //Reflect the ball off of the bottom of the screen
      if(bally + ballsize == 63) {
        balldown = -1;
      }
      
      //Draw the player's paddle
      arduboy.fillRect(playerx, playery, paddlewidth, paddleheight, WHITE);
      //If the player presses Up and the paddle is not touching the top of the screen, move the paddle up
      if(arduboy.pressed(UP_BUTTON) and playery > 0) {
        playery = playery - 1;
      }
      //If the player presses down and the paddle is not touching the bottom of the screen, move the paddle down
      if(arduboy.pressed(DOWN_BUTTON) and playery + paddleheight < 63) {
        playery = playery + 1;
      }
      
      //Draw the computer's paddle
      arduboy.fillRect(computerx, computery, paddlewidth, paddleheight, WHITE);
      // if statement for AI purpose, this gives a delay to npc reaction time
      if(ballx > 115 or rand() % 20 == 1){
           //If the ball is higher than the computer's paddle, move the computer's paddle up
           if(bally < computery) {
           computery = computery - 1;
           }
           //If the bottom of the ball is lower than the bottom of the computer's paddle, move the comptuer's paddle down
           if(bally + ballsize > computery + paddleheight) {
             computery = computery + 1;
           }
      }
      //If the ball moves off of the screen to the left...
      if(ballx < -10) {
        //Move the ball back to the middle of the screen
        ballx = 63;
        //Give the computer a point
        computerscore = computerscore + 1;
      tunes.playScore(hit);
      }
      //If the ball moves off of the screen to the right....
      if(ballx > 130) {
        //Move the ball back to the middle of the screen
        ballx = 63;
        //Give the player a point
        playerscore = playerscore + 1;
      tunes.playScore(point);
      }
      //Check if the player wins
      if(playerscore == 5) {
        gamestate = 2;
      }
      //Check if the computer wins
      if(computerscore == 5) {
        gamestate = 3;
      }
      //If the ball makes contact with the player's paddle, bounce it back to the right
      if(ballx == playerx + paddlewidth and playery < bally + ballsize and playery + paddleheight > bally) {
        ballright = 1;
      }
      //If the ball makes contact with the computer's paddle, bounce it back to the left
      if(ballx + ballsize == computerx and computery < bally + ballsize and computery + paddleheight > bally) {
        ballright = -1;
      }
      break;
    case 2:
      //Win screen
      arduboy.setCursor(30, 25);
      arduboy.print("You WON!");
      //Change the gamestate
      if(arduboy.pressed(A_BUTTON) and justpressed == 0) {
        justpressed = 1;
        resetgame();
        gamestate = 0;
      }
      break;
    case 3:
      //Game over screen
      arduboy.setCursor(30, 25);
      arduboy.print("Game Over Man!");
      //Change the gamestate
      if(arduboy.pressed(A_BUTTON) and justpressed == 0) {
        justpressed = 1;
        resetgame();
        gamestate = 0;
      }
      break;
  }
  //Check if the button is being held down
  if(arduboy.notPressed(A_BUTTON)) {
    justpressed = 0;
  }


  //Refresh the screen to show whatever's printed to it
  arduboy.display();
}
