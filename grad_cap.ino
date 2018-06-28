/****************************************************  
 *   Nikola Obradovic
 *   Designed to work with the Adafruit LED Matrix
 *   backpack: http://www.adafruit.com/product/1427
 *   
 *   Only 8x8 LEDs were used, remaining anode pins on
 *   backpack were unused
 ****************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Pins for setting mode
int Mode0 = 5;
int Mode1 = 7;
int Mode2 = 9;
int Mode3 = 10;

int delay_time = 100;

// Stores state of switch on startup
int r0, r1, r2, r3;

// Object for controlling matrix driver
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// Accelerometer for "tilt-table" function
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

int ttXPos = 4;
int ttYPos = 4;

// Bitmaps for graphics display
static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 },
  curr_test_bmp[] =
  { B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111 },
  tassel_start_bmp[] =
  { B00000000,
    B00000000,
    B00000000,
    B00011111,
    B00011111,
    B00000000,
    B00000000,
    B00000000 },
  tassel_move1_bmp[] =
  { B00000000,
    B00000001,
    B00000111,
    B00011110,
    B00011000,
    B00000000,
    B00000000,
    B00000000 },
  tassel_move2_bmp[] =
  { B00000011,
    B00000111,
    B00001110,
    B00011100,
    B00011000,
    B00000000,
    B00000000,
    B00000000 },
  tassel_move3_bmp[] =
  { B00000110,
    B00001100,
    B00001100,
    B00011000,
    B00011000,
    B00000000,
    B00000000,
    B00000000 },
  tassel_final_bmp[] =
  { B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00000000,
    B00000000,
    B00000000 };

static const uint8_t PROGMEM
pong_1[]={
  B00000000,
  B00000000,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B00000000,
  B00000000 },
pong_2[]={
  B00000000,
  B00000000,
  B10000011,
  B10000001,
  B10000001,
  B10000001,
  B00000000,
  B00000000 },
pong_3[]={
  B00000000,
  B00000000,
  B10000000,
  B10000101,
  B10000001,
  B10000001,
  B00000001,
  B00000000 },
pong_4[]={
  B00000000,
  B00000000,
  B10000000,
  B10000001,
  B10001001,
  B10000001,
  B00000001,
  B00000000 },
pong_5[]={
  B00000000,
  B00000000,
  B10000000,
  B10000001,
  B10000001,
  B10010001,
  B00000001,
  B00000000 },
pong_6[]={
  B00000000,
  B00000000,
  B00000000,
  B10000001,
  B10000001,
  B10000001,
  B10100001,
  B00000000 },
pong_7[]={
  B00000000,
  B00000000,
  B00000000,
  B00000001,
  B10000001,
  B10000001,
  B10000001,
  B11000000 },
pong_8[]={
  B00000000,
  B00000000,
  B00000000,
  B00000001,
  B10000001,
  B10000001,
  B10010001,
  B10000000 },
pong_9[]={
  B00000000,
  B00000000,
  B00000000,
  B00000001,
  B10000001,
  B10001001,
  B10000001,
  B10000000 },
pong_10[]={
  B00000000,
  B00000000,
  B00000001,
  B00000001,
  B10000001,
  B10000101,
  B10000000,
  B10000000 },
pong_11[]={
  B00000000,
  B00000000,
  B00000001,
  B00000001,
  B10000011,
  B10000001,
  B10000000,
  B10000000 },
pong_12[]={
  B00000000,
  B00000001,
  B00000001,
  B00000101,
  B10000001,
  B10000000,
  B10000000,
  B10000000 },
pong_13[]={
  B00000000,
  B00000001,
  B00001001,
  B10000001,
  B10000001,
  B10000000,
  B10000000,
  B00000000 },
pong_14[]={
  B00000000,
  B00010001,
  B10000001,
  B10000001,
  B10000001,
  B10000000,
  B00000000,
  B00000000 },
pong_15[]={
  B00100000,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B00000000,
  B00000000,
  B00000000 },
pong_16[]={
  B10100000,
  B11000001,
  B10000001,
  B10000001,
  B00000001,
  B00000000,
  B00000000,
  B00000000 },
pong_17[]={
  B10000000,
  B10100001,
  B10000001,
  B10000001,
  B00000001,
  B00000000,
  B00000000,
  B00000000 },
pong_18[]={
  B10000000,
  B10010001,
  B10000001,
  B10000001,
  B00000001,
  B00000000,
  B00000000,
  B00000000 },
pong_19[]={
  B10000000,
  B10001001,
  B10000001,
  B10000001,
  B00000001,
  B00000000,
  B00000000,
  B00000000 },
pong_20[]={
  B00000000,
  B10000001,
  B10000101,
  B10000001,
  B10000001,
  B00000000,
  B00000000,
  B00000000 },
pong_21[]={
  B00000000,
  B00000000,
  B10000011,
  B10000001,
  B10000001,
  B10000001,
  B00000000,
  B00000000 },
pong_win_1[]={
  B00000000,
  B00000000,
  B10000000,
  B10000101,
  B10000001,
  B10000001,
  B00000001,
  B00000000 },
pong_win_2[]={
  B00000000,
  B00000000,
  B10000000,
  B10000001,
  B10001001,
  B10000001,
  B00000001,
  B00000000 },
pong_win_3[]={
  B00000000,
  B00000000,
  B10000000,
  B10000001,
  B10000001,
  B10010001,
  B00000001,
  B00000000 },
pong_win_4[]={
  B00000000,
  B00000000,
  B00000000,
  B10000000,
  B10000001,
  B10000001,
  B10100001,
  B00000001 },
pong_win_5[]={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B10000001,
  B10000001,
  B10000001,
  B11000001 },
pong_win_6[]={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B10000001,
  B10000001,
  B10100001,
  B10000001 },
pong_win_7[]={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B10000001,
  B10100001,
  B10000001,
  B10000001 },
pong_win_8[]={
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B10010001,
  B10000001,
  B10000001,
  B10000000 },
pong_win_9[]={
  B00000000,
  B00000000,
  B00000000,
  B00001001,
  B10000001,
  B10000001,
  B10000001,
  B10000000 },
pong_win_10[]={
  B00000000,
  B00000000,
  B00000100,
  B00000001,
  B10000001,
  B10000001,
  B10000001,
  B10000000 },
pong_win_11[]={
  B00000000,
  B00000000,
  B00000010,
  B00000001,
  B10000001,
  B10000001,
  B10000001,
  B10000000 },
pong_win_12[]={
  B00000000,
  B00000010,
  B00000001,
  B00000001,
  B10000001,
  B10000001,
  B10000000,
  B10000000 },
pong_over[]={
  B00000001,
  B00000000,
  B00000001,
  B00000001,
  B10000001,
  B10000001,
  B10000000,
  B10000000 };


void setup() {
  // Setup input switches
  pinMode(Mode0, INPUT_PULLUP);
  pinMode(Mode1, INPUT_PULLUP);
  pinMode(Mode2, INPUT_PULLUP);
  pinMode(Mode3, INPUT_PULLUP);
  

  // Read input switches. 
  // Switches are read once, flip switches and
  // reset controller to change mode
  r0 = digitalRead(Mode0);
  r1 = digitalRead(Mode1);
  r2 = digitalRead(Mode2);
  r3 = digitalRead(Mode3);

  // Setup matrix controller
  matrix.begin(0x70);
  matrix.setBrightness(6);
  matrix.setRotation(3);
  
  // Write the state of the switches to the
  // top row of the matrix for debugging purposes
  matrix.clear();
  if(r0)
    matrix.drawPixel(0,0,LED_ON);
  if(r1)
    matrix.drawPixel(1,0,LED_ON);
  if(r2)
    matrix.drawPixel(2,0,LED_ON);
  if(r3)
    matrix.drawPixel(3,0,LED_ON);

  // This line (as well as the following delay)
  // can be commented out to eliminate startup 
  // debugging display
  matrix.writeDisplay();
  delay(1000);

  //Setup accelerometer
  accel.begin();
  accel.setRange(ADXL345_RANGE_2_G);
}




/*
 * Displays a short pong game.
 * Can be called multiple times to loop the sequence.
 * Can be followed by a call to pong_win() to
 * finish game.
 */
void pong_play()
{
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_1, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_2, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_3, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_4, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_5, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_6, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_7, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_8, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_9, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_10, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_11, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_12, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_13, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_14, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
  
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_15, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
    
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_16, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_17, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_18, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_19, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_20, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_21, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

}

/*
 * Wins a game of pong. Should be preceded by one
 * or more calls to pong_play()
 * 
 * Missing the last frame bitmap, pong_over, 
 * which is displayed manually with a blink animation.
 * Probably should have put that bit in here.
 */
void pong_win()
{
  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_1, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_2, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_3, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_4, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_5, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_6, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_7, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_8, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_9, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_10, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_11, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_win_12, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(delay_time);
}

/*
 * Scrolls a string of text across the screen.
 * Try to keep strings to under 30 characters or so.
 * Any more and the end of the string won't display
 * properly. 
 * 
 * Use the frames arg to adjust how long the text
 * is present on screen. Since scroll rate is constant,
 * this will adjust how much of the message displays.
 * This is useful for adjusting how long the display
 * is blank after the final character leaves the display.
 */
void scrollText(String text, int frames)
{
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);


  for (int8_t x=7; x>=-frames; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print(text);
    matrix.writeDisplay();
    delay(125);
  }
}

/*
 * Never finished this one.
 */
void spiralAnimation()
{
  matrix.clear();
  
}

/*
 * Displays a small square which can be controlled by
 * tilting the hat. Acceleration trigger thresholds
 * were found by experimentation, new implementations
 * will have to repeat this testing.
 */
void tiltTable()
{
  sensors_event_t event; 
  accel.getEvent(&event);

  if (event.acceleration.x > 3)
  {
    ttXPos--;
  }
  else if (event.acceleration.x < -4)
  {
    ttXPos++;
  }

  if (event.acceleration.y > -8)
  {
    ttYPos--;
  }
  else if (event.acceleration.y < -15)
  {
    ttYPos++;
  }

  if(ttYPos > 6)
  {
    ttYPos = 6;
  }
  else if (ttYPos < 0)
  {
    ttYPos = 0;
  }

  if(ttXPos > 6)
  {
    ttXPos = 6;
  }
  else if (ttXPos < 0)
  {
    ttXPos = 0;
  }

  matrix.clear();      // clear display
  matrix.fillRect(ttXPos, ttYPos, 2, 2, LED_ON);  
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(100);
}

/*
 * This mode scrolls a variety of messages across the screen.
 * Most messages have been broken up into multiple calls
 * to scrollText() to avoid issues with messages being too long.
 * 
 * Some graphical animations have been interspersed with the text.
 */
void marquee()
{

  //hello world
  scrollText(F("Hello World!"), 77);

   //toynbee tile
  scrollText(F("TOYNBEE IDEA"),70);
  scrollText(F("IN MOViE 2001"),75); //lowercase intentional
  scrollText(F("RESURRECT DEAD"),80);
  scrollText(F("ON PLANET JUPITER"),105);

  for(int i = 0; i< 4; i++)
  {
    //animation here
    matrix.clear();
    matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
    delay(500);
  
    matrix.clear();
    matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
    delay(500);
  }

  
  // Job Offers
  scrollText(F("Send job offers to:"), 112);
  scrollText(F("example@school.edu"), 104);

  //Graduation
  scrollText(F("Drexel Graduation"), 100);
  scrollText(F("2018"), 34);

  //halal cart
  scrollText(F("TRY OUR BEST"), 73);
  scrollText(F("CHICKEN GYRO..."), 84);
  scrollText(F("LAMB GYRO..."), 69);
  scrollText(F("FALAFEL SANDWICH..."), 110);  

  //PONG
  pong_play();
  pong_win();

  matrix.clear();
  matrix.drawBitmap(0, 0, pong_over, 8, 8, LED_ON);
  matrix.writeDisplay();
  //delay(delay_time);
  
  matrix.blinkRate(1);
  delay(2000);
  matrix.blinkRate(0);
  
  //Hat Factory
  scrollText(F("Help I'm trapped"), 95);
  scrollText(F("in a hat factory!"), 102);

  //hi mom
  scrollText(F("Hi Mom"), 40);

 
}

/*
 * Since the physical tassel wouldn't fit over the display,
 * I included this mode to animate one, including the
 * "tassel-turn" traditionally done at graduation ceremonies.
 * 
 * To turn the tassel, start the controller in tassel mode
 * (r1/Mode1) and press the switch or pushbutton attached to 
 * Mode0. Reset the controller to move the tassel back to the right.
 */
void tasselTurn()
{
  matrix.clear();
  matrix.drawBitmap(0, 0, tassel_start_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  while(digitalRead(Mode0)){}

  matrix.clear();
  matrix.drawBitmap(0, 0, tassel_move1_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  matrix.clear();
  matrix.drawBitmap(0, 0, tassel_move2_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  matrix.clear();
  matrix.drawBitmap(0, 0, tassel_move3_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  matrix.clear();
  matrix.drawBitmap(0, 0, tassel_final_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();

  while(1){}

}

/*
 * call a display function according to the mode variable set.
 */
void loop() {
  
  if(!r1)
  { 
    tasselTurn();
  }
  else if(!r2)
  { 
    marquee();
  }
  else if(!r3)
  { 
    tiltTable();
  }


}
