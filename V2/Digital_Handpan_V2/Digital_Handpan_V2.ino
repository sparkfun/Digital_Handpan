/******************************************************************************
  Digital Handpan V2 Code for Teesny 
  Joel Bartlett @ SparkFun Electronics
  Original Creation Date: December 16, 2017
  
  This sketch produces polyphonic notes when any number of eight capacitive touch pads are 
  touched, thus replicating a Handpan (or Hang) steel drum. This sketch 
  makes use of the Teensy Audio Shield (https://www.sparkfun.com/products/12767)
  as well as the Teensy Audio System Design Tool (http://www.pjrc.com/teensy/gui/).
  All capactive touch sensing is handled by the Teensy's built in Touch Sense Pins. 
  
  Hardware:
    Teensy 3.2 Dev Board (https://www.sparkfun.com/products/13736)
    Teensy Audio Board (https://www.sparkfun.com/products/12767)
    Teensy View OLED Screen (https://www.sparkfun.com/products/14048)
    Various buttons and potentiometers 

  Development environment specifics:
    IDE: Arduino IDE V 1.8.5 with the Teensyduino Add-on installed V Teensy Loader V1.30

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
*******************************************************************************/
//These libraries are included by the Teensy Audio System Design Tool 
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>
#include <TeensyView.h>
#include <Wire.h>
#include <MAX17043GU.h>

#define MAX17043_ADDRESS 0x36// Battery Fuel Gauge 

#include "note_frequency.h"
#include "TeensyAudioDesignTool.h"

//Alternate (Audio) Teensy View Setup
#define PIN_RESET 2
#define PIN_DC    21
#define PIN_CS    20
#define PIN_SCK   14
#define PIN_MOSI  7


#define volKnob A13 //grey wire 
#define decayKnob A12 //black wire 

MAX17043GU battery;
TeensyView oled(PIN_RESET, PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI);

//an array to define each of the eight touch sense pins, 
//all other touch sense pins are used by the Teensy Audio Shield (16,17,18,19)
int pinTouch[] = {0,1,15,16,17,25,32,33}; //25 = white, 32 = yellow, 33 = blue

int scale_index = 0;//var to keep track fo which scale is being used
int note_index = 57;//starting note value (A) 
int octave_index = 0;

int dcVal = 0;//value to control the decay of each note 
int padNumber = 0;//debug for printing pad values to OLED

////////////////////CHANGE THIS WHEN ADDING MORE SCALES
int numOfScales = 10 ;//indexed at zero (subtract 1) 
///////////////////////////////////////////////////////

int scale[8];

//buttons for incrementing or decrementing through each scale
Bounce button0 = Bounce(4, 15);
Bounce button1 = Bounce(3, 15);  
//buttons for incrementing or decrementing the octave
Bounce foot0 = Bounce(5, 30);
Bounce foot1 = Bounce(8, 30);    

float vol = 0;

float voltage;
float percentage;

int counter = 0;

bool debug = false;  
////////////////////////////////////////////////////////
void setup() 
{
  //initialize buttons
  pinMode(4, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  button0.update();
  button1.update();
  foot0.update();
  foot1.update();
  
  //initialize Serial
  Serial.begin(115200);

  // Start I2C
  Wire.begin();
  delay(100);
  battery.restart();
  
  //set aside audio memory 
  AudioMemory(64);

  //initialize audio settings
  sine1.amplitude(0.125);
  sine1.frequency(440);
  sine2.amplitude(0.125);
  sine2.frequency(440);
  sine3.amplitude(0.125);
  sine3.frequency(440);
  sine4.amplitude(0.125);
  sine4.frequency(440);
  sine5.amplitude(0.125);
  sine5.frequency(440);
  sine6.amplitude(0.125);
  sine6.frequency(440);
  sine7.amplitude(0.125);
  sine7.frequency(440);
  sine8.amplitude(0.125);
  sine8.frequency(440);
  
  dc1.amplitude(0);
  dc2.amplitude(0);
  dc3.amplitude(0);
  dc4.amplitude(0);
  dc5.amplitude(0);
  dc6.amplitude(0);
  dc7.amplitude(0);
  dc8.amplitude(0);


  //initialize volume
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.8);
  sgtl5000_1.lineOutLevel(5);

    //Initialize the OLED
  oled.begin();
  // clear(ALL) will clear out the OLED's graphic memory.
  // clear(PAGE) will clear the Arduino's display buffer.
  oled.clear(ALL);  // Clear the display's memory (gets rid of artifacts)
  // To actually draw anything on the display, you must call the
  // display() function.
  oled.display();
  // Give the splash screen some time to shine
  delay(1000);

  changeScale();//start off at middle C
}
//////////////////////////////////////////////
void loop() 
{    
  volumeCheck();//check the volume knob 

  touchCheck();//check if any of the capacitive pads have been touched

  dcValCheck();//check the decay knob

  buttonCheck();//check for button presses to change the scale 

  counter++;
  if(counter == 10000);//don't check battery all the time, slows opperation
  {
    getBattery();
    counter = 0;
  }
  
  oledPrint();//print to TeensyView

}
void getBattery()
{
  // Get the values
  voltage = battery.voltageLevel();
  percentage = battery.fuelLevel();  
}
void oledPrint()
{
  oled.clear(PAGE);
  
  oled.setCursor(0, 0);
  oled.print("Scale = ");  

  if(note_index == 60)
  oled.print("C ");
  if(note_index == 61)
  oled.print("C# ");
  if(note_index == 62)
  oled.print("D ");
  if(note_index == 63)
  oled.print("D# ");
  if(note_index == 64)
  oled.print("E ");
  if(note_index == 65)
  oled.print("F ");
  if(note_index == 66)
  oled.print("F# ");
  if(note_index == 67)
  oled.print("G ");
  if(note_index == 68)
  oled.print("G# ");
  if(note_index == 57)
  oled.print("A ");
  if(note_index == 58)
  oled.print("A# ");
  if(note_index == 59)
  oled.print("B ");

  
  if(scale_index == 0)
  oled.print("Major");
  if(scale_index == 1)
  oled.print("Minor");
  if(scale_index == 2)
  oled.print("Akebono");
  if(scale_index == 3)
  oled.print("Pygmy");
  if(scale_index == 4)
  oled.print("Equinox");
  if(scale_index == 5)
  oled.print("Sapphire");
  if(scale_index == 6)
  oled.print("Gypsy");
  if(scale_index == 7)
  oled.print("SlvrSpring");
  if(scale_index == 8)
  oled.print("Integral");
  if(scale_index == 9)
  oled.print("Dorian");
  if(scale_index == 10)
  oled.print("GldArcadia");

  oled.setCursor(0, 8);
  oled.print("Volume = ");
  int newVol = map(vol, 0.0, 0.8, 0, 100);
  oled.print(newVol);
  oled.print("%");
  
  oled.setCursor(0, 16);
  oled.print("Decay = ");
  oled.print((int)dcVal/10);
  oled.print("%  Oct:");
  oled.print(octave_index);

  
  oled.setCursor(0, 24);
  oled.print("Batt = ");  
  oled.print(percentage);
  oled.print("% ");
  oled.print(voltage);
  oled.print("V");

  oled.display();

  delay(10);
}
/////////////////////////////////////////////////////
void volumeCheck()
{
  vol = (float)analogRead(volKnob) / 1280.0;

  mixerMain.gain(0, vol);
  mixerMain.gain(1, vol);
}
/////////////////////////////////////////////////////
void dcValCheck()
{
  //check knob and set value as delay on dc constant for sine wave decay
  dcVal = map(analogRead(decayKnob), 0, 1023, 1, 1000);
}
/////////////////////////////////////////////////////
void touchCheck()
{
  //Each capacitive touch pad will vary based on the size and material it is made of
  //The value necessary to trigger each note will require some trial and error to get the
  //sensitivity just right. Try adjusting these values to get the best response. 
    
    if (touchRead(pinTouch[0]) > 1800)
    {
      //once a pad is touched, a value from the note frquency froma table is looked up via a 2D table
      //with x corresponding to a scale and y corresponding to one of the eight notes on the drum. 
      
      if(octave_index == 1)
        sine1.frequency(note_frequency[scale[0]+12]);
      else if(octave_index == -1)
        sine1.frequency(note_frequency[scale[0]-12]);
      else if(octave_index == 2)
        sine1.frequency(note_frequency[scale[0]+24]);
      else if(octave_index == -2)
        sine1.frequency(note_frequency[scale[0]-24]);
      else 
        sine1.frequency(note_frequency[scale[0]]);
      dc1.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[0]) <= 1800) 
    {
      //one the pad is released, the note fades out with a decay val set by the dcVal knob
      dc1.amplitude(0, dcVal);
    }
      

    if (touchRead(pinTouch[1]) > 1800) 
    {
      if(octave_index == 1)
        sine2.frequency(note_frequency[scale[1]+12]);
      else if(octave_index == -1)
        sine2.frequency(note_frequency[scale[1]-12]);
      else if(octave_index == 2)
        sine2.frequency(note_frequency[scale[1]+24]);
      else if(octave_index == -2)
        sine2.frequency(note_frequency[scale[1]-24]);
      else 
        sine2.frequency(note_frequency[scale[1]]);
      dc2.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[1]) <= 1800) 
    {
      dc2.amplitude(0, dcVal);
    }

    
    if (touchRead(pinTouch[2]) > 1900)
    {
      if(octave_index == 1)
        sine3.frequency(note_frequency[scale[2]+12]);
      else if(octave_index == -1)
        sine3.frequency(note_frequency[scale[2]-12]);
      else if(octave_index == 2)
        sine3.frequency(note_frequency[scale[2]+24]);
      else if(octave_index == -2)
        sine3.frequency(note_frequency[scale[2]-24]);
      else 
        sine3.frequency(note_frequency[scale[2]]);
      dc3.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[2]) <= 1900) 
    {
      dc3.amplitude(0, dcVal);
    }
    

    if (touchRead(pinTouch[3]) > 1800)
    {
      if(octave_index == 1)
        sine4.frequency(note_frequency[scale[3]+12]);
      else if(octave_index == -1)
        sine4.frequency(note_frequency[scale[3]-12]);
      else if(octave_index == 2)
        sine4.frequency(note_frequency[scale[3]+24]);
      else if(octave_index == -2)
        sine4.frequency(note_frequency[scale[3]-24]);
      else 
        sine4.frequency(note_frequency[scale[3]]);
      dc4.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[3]) <= 1800)
    {
      dc4.amplitude(0, dcVal);
    }
       
    if (touchRead(pinTouch[4]) > 1800) 
    {
      if(octave_index == 1)
        sine5.frequency(note_frequency[scale[4]+12]);
      else if(octave_index == -1)
        sine5.frequency(note_frequency[scale[4]-12]);
      else if(octave_index == 2)
        sine5.frequency(note_frequency[scale[4]+24]);
      else if(octave_index == -2)
        sine5.frequency(note_frequency[scale[4]-24]);
      else 
        sine5.frequency(note_frequency[scale[4]]);
      dc5.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[4]) <= 1800)
    {
      dc5.amplitude(0, dcVal);
    }
    
    if (touchRead(pinTouch[5]) > 1800)
    {
      if(octave_index == 1)
        sine6.frequency(note_frequency[scale[5]+12]);
      else if(octave_index == -1)
        sine6.frequency(note_frequency[scale[5]-12]);
      else if(octave_index == 2)
        sine6.frequency(note_frequency[scale[5]+24]);
      else if(octave_index == -2)
        sine6.frequency(note_frequency[scale[5]-24]);
      else 
        sine6.frequency(note_frequency[scale[5]]);
      dc6.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[5]) <= 1800) 
    {
      dc6.amplitude(0, dcVal);
    }
    

   
    if (touchRead(pinTouch[6]) > 1800)
    {
      if(octave_index == 1)
        sine7.frequency(note_frequency[scale[6]+12]);
      else if(octave_index == -1)
        sine7.frequency(note_frequency[scale[6]-12]);
      else if(octave_index == 2)
        sine7.frequency(note_frequency[scale[6]+24]);
      else if(octave_index == -2)
        sine7.frequency(note_frequency[scale[6]-24]);
      else 
        sine7.frequency(note_frequency[scale[6]]);
      dc7.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[6]) <= 1800) 
    {
      dc7.amplitude(0, dcVal);
    }
    
   
    if (touchRead(pinTouch[7]) > 1700) 
    {
      if(octave_index == 1)
        sine8.frequency(note_frequency[scale[7]+12]);
      else if(octave_index == -1)
        sine8.frequency(note_frequency[scale[7]-12]);
      else if(octave_index == 2)
        sine8.frequency(note_frequency[scale[7]+24]);
      else if(octave_index == -2)
        sine8.frequency(note_frequency[scale[7]-24]);
      else 
        sine8.frequency(note_frequency[scale[7]]);
      dc8.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[7]) <= 1700) 
    {
      dc8.amplitude(0, dcVal);
    }


  
}
//////////////////////////////////////////////////
void buttonCheck()
{
  button0.update();
  button1.update();
  foot0.update();
  foot1.update();

  //if button 0 is pressed, increment the scale being used
  if (button0.fallingEdge())
  { 
    
    note_index++;
    padNumber++;
    //check for overflow
    if(note_index > 68)
    {
      note_index = 57; 
    }
    if(padNumber > 7)
    {
      padNumber = 0;
    }
    octave_index = 0;
    changeScale();
  }

  //if button 1 is pressed, decrement the scale being used
  if (button1.fallingEdge())
  { 
    scale_index++;
    padNumber--;
    //check for overflow
    if(scale_index > numOfScales)
    {
      scale_index = 0;
    }
    if(padNumber < 0)
    {
      padNumber = 7;
    } 

    octave_index = 0;
    changeScale();
  }

  //if foot pedal 0 is pressed, increment the octave 
  if (foot0.risingEdge())
  { 
    octave_index++;

    if(octave_index > 2)
    octave_index = 2;
  }

  //if button 1 is pressed, decrement the scale being used
  if (foot1.risingEdge())
  { 
    octave_index--;

    if(octave_index < -2)
    octave_index = -2;
  }
  

}
void changeScale()
{
  //Change numOfScales variable at top if adding new scale!!
  
  int root = note_index;
  
  if(scale_index == 0)//Major Scale  2,2,3,2,3,2,2  (WholeStep, WS, WS+HalfStep, WS, WS+HS, WS, WS)
  {         
      scale[0] = root;
      scale[1] = root+2;
      scale[2] = root+4;
      scale[3] = root+7;
      scale[4] = root+9;
      scale[5] = root+12;
      scale[6] = root+14;
      scale[7] = root+16;
  }
  if(scale_index == 1)//Minor Scale  3,2,2,3,2,3,2
  {
      scale[0] = root;
      scale[1] = root+3;
      scale[2] = root+5;
      scale[3] = root+7;
      scale[4] = root+10;
      scale[5] = root+12;
      scale[6] = root+15;
      scale[7] = root+17;
  }
  if(scale_index == 2)//Akebono Scale  2,1,4,1,4,2,1
  {
      scale[0] = root;
      scale[1] = root+2;
      scale[2] = root+3;
      scale[3] = root+7;
      scale[4] = root+8;
      scale[5] = root+12;
      scale[6] = root+14;
      scale[7] = root+15;
  }
  if(scale_index == 3)//Pygmy Scale  2,1,4,3,2,2,1
  {
      scale[0] = root;
      scale[1] = root+2;
      scale[2] = root+3;
      scale[3] = root+7;
      scale[4] = root+10;
      scale[5] = root+12;
      scale[6] = root+14;
      scale[7] = root+15;
  }
  if(scale_index == 4)//Equinox Scale  4,1,2,2,2,1,4
  {
      scale[0] = root;
      scale[1] = root+4;
      scale[2] = root+5;
      scale[3] = root+7;
      scale[4] = root+9;
      scale[5] = root+11;
      scale[6] = root+12;
      scale[7] = root+16;
  }
  if(scale_index == 5)//Sapphire    3,2,4,1,2,3,2
  {
      scale[0] = root;
      scale[1] = root+3;
      scale[2] = root+5;
      scale[3] = root+9;
      scale[4] = root+10;
      scale[5] = root+12;
      scale[6] = root+15;
      scale[7] = root+17;
  }
  if(scale_index == 6)//Gypsy    1,3,1,2,1,2,2
  {
      scale[0] = root;
      scale[1] = root+1;
      scale[2] = root+4;
      scale[3] = root+5;
      scale[4] = root+7;
      scale[5] = root+8;
      scale[6] = root+10;
      scale[7] = root+12;
  }
  if(scale_index == 7)//Silver Spring  4,1,2,2,3,4,1
  {
      scale[0] = root;
      scale[1] = root+4;
      scale[2] = root+5;
      scale[3] = root+7;
      scale[4] = root+9;
      scale[5] = root+12;
      scale[6] = root+16;
      scale[7] = root+17;
  }  
  if(scale_index == 8)//Integral  2,1,4,1,2,2,2
  {
      scale[0] = root;
      scale[1] = root+2;
      scale[2] = root+3;
      scale[3] = root+7;
      scale[4] = root+8;
      scale[5] = root+10;
      scale[6] = root+12;
      scale[7] = root+14;
  }  
  if(scale_index == 9)//Dorian  2,1,2,2,2,3,2
  {
      scale[0] = root;
      scale[1] = root+2;
      scale[2] = root+3;
      scale[3] = root+5;
      scale[4] = root+7;
      scale[5] = root+9;
      scale[6] = root+12;
      scale[7] = root+14;
  }  
  if(scale_index == 10)//Golden Arcadia  4,3,4,1,2,4,1
  {
      scale[0] = root;
      scale[1] = root+4;
      scale[2] = root+7;
      scale[3] = root+11;
      scale[4] = root+12;
      scale[5] = root+14;
      scale[6] = root+18;
      scale[7] = root+19;
  }  
}

