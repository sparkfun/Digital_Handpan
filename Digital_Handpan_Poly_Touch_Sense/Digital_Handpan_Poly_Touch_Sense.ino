/******************************************************************************
  Digital Handpan Code for Teesny 
  Joel Bartlett @ SparkFun Electronics
  Original Creation Date: October 16, 2016
  
  This sketch produces polyphonic notes when any number of eight capacitive touch pads are 
  touched, thus replicating a Handpan (or Hang) steel drum. This sketch 
  makes use of the Teensy Audio Shield (https://www.sparkfun.com/products/12767)
  as well as the Teensy Audio System Design Tool (http://www.pjrc.com/teensy/gui/).
  All capactive touch sensing is handled by the Teensy's built in Touch Sense Pins. 
  
  Hardware:
    Teensy 3.2 Dev Board (https://www.sparkfun.com/products/13736)
    Teensy Audio Board (https://www.sparkfun.com/products/12767)
    Various buttons and potentiometers 

  Development environment specifics:
    IDE: Arduino IDE V 1.6.11 with the Teensyduino Add-on installed V Teensy Loader V1.30

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
*******************************************************************************/
//These libraries are included by the Teensy Audio System Design Tool 
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
//#include <SD.h>//not using the SD library at this time
#include <SerialFlash.h>
#include <Bounce.h>

#include "note_frequency.h"
#include "scales.h"
#include "TeensyAudioDesignTool.h"

//an array to define each of the eight touch sense pins, 
//all other touch sense pins are used by the Teensy Audio Shield (16,17,18,19)
int pinTouch[] = {33,32,25,17,16,15,1,0};

int scale_index = 0;//var to keep track fo which scale is being used

int dcVal = 0;//value to control the decay of each note 

//buttons for incrementing or decrementing through each scale
Bounce button0 = Bounce(2, 15);
Bounce button1 = Bounce(3, 15);    

bool debug = 0;//Set to 1 for Serial debugging 

////////////////////////////////////////////////////////
void setup() 
{
  //initialize buttons
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  button0.update();
  button1.update();
  
  //initialize Serial
  Serial.begin(115200);
  
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
  //enable line out for troubleshooting on O-scope
  //sgtl5000_1.unmuteLineout();

}
//////////////////////////////////////////////
void loop() 
{     
  volumeCheck();//check the volume knob 

  touchCheck();//check if any of the capacitive pads have been touched

  dcValCheck();//check the decay knob

  buttonCheck();//check for button presses to change the scale 
  
}
/////////////////////////////////////////////////////
void volumeCheck()
{
  //check knob value for volume 
  int knob = analogRead(A7);
  float vol = (float)knob / 1280.0;
  sgtl5000_1.volume(vol);
  
  if(debug == 1)
  {  
  Serial.print("A7 = ");  
  Serial.println(knob);
  }
}
/////////////////////////////////////////////////////
void dcValCheck()
{
  //check knob and set value as delay on dc constant for sine wave decay
  dcVal = map(analogRead(A6), 0, 1023, 1, 1000);

  if(debug == 1)
  {  
  Serial.print("A6 = ");  
  Serial.println(dcVal);
  }

}
/////////////////////////////////////////////////////
void touchCheck()
{
  //Each capacitive touch pad will vary based on the size and material it is made of
  //The value necessary to trigger each note will require some trial and error to get the
  //sensitivity just right. Try adjusting these values to get the best response. 
    
    if (touchRead(pinTouch[0]) > 2000)
    {
      //once a pad is touched, a value from the note frquency froma table is looked up via a 2D table
      //with x corresponding to a scale and y corresponding to one of the eight notes on the drum. 
      
      sine1.frequency(note_frequency[scale[scale_index][0]]);
      dc1.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[0]) <= 2000) 
    {
      //one the pad is released, the note fades out with a decay val set by the dcVal knob
      dc1.amplitude(0, dcVal);
    }
      

    if (touchRead(pinTouch[1]) > 2000) 
    {
      sine2.frequency(note_frequency[scale[scale_index][1]]);
      dc2.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[1]) <= 2000) 
    {
      dc2.amplitude(0, dcVal);
    }

    
    if (touchRead(pinTouch[2]) > 2000)
    {
      sine3.frequency(note_frequency[scale[scale_index][2]]);
      dc3.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[2]) <= 2000) 
    {
      dc3.amplitude(0, dcVal);
    }
    

    if (touchRead(pinTouch[3]) > 2000)
    {
      sine4.frequency(note_frequency[scale[scale_index][3]]);
      dc4.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[3]) <= 2000)
    {
      dc4.amplitude(0, dcVal);
    }
       
    if (touchRead(pinTouch[4]) > 2000) 
    {
      sine5.frequency(note_frequency[scale[scale_index][4]]);
      dc5.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[4]) <= 2000)
    {
      dc5.amplitude(0, dcVal);
    }
    
    if (touchRead(pinTouch[5]) > 2000)
    {
      sine6.frequency(note_frequency[scale[scale_index][5]]);
      dc6.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[5]) <= 2000) 
    {
      dc6.amplitude(0, dcVal);
    }
    

   
    if (touchRead(pinTouch[6]) > 2000)
    {
      sine7.frequency(note_frequency[scale[scale_index][6]]);
      dc7.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[6]) <= 2000) 
    {
      dc7.amplitude(0, dcVal);
    }
    
   
    if (touchRead(pinTouch[7]) > 2000) 
    {
      sine8.frequency(note_frequency[scale[scale_index][7]]);
      dc8.amplitude(1.0, 5);
    }
    if (touchRead(pinTouch[7]) <= 2000) 
    {
      dc8.amplitude(0, dcVal);
    }


  
}
//////////////////////////////////////////////////
void buttonCheck()
{
  button0.update();
  button1.update();

  //if button 0 is pressed, increment the scale being used
  if (button0.risingEdge())
  { 
    scale_index++;
    //check for overflow
    if(scale_index > numOfScales)//numOfScales variable found in the scales.h file
    scale_index = 0;
  }

  //if button 1 is pressed, decrement the scale being used
  if (button1.risingEdge())
  { 
    scale_index--;
    //check for negative numbers
    if(scale_index < 0)
    scale_index = numOfScales;//numOfScales variable found in the scales.h file
  }

  if(debug == 1)
  {  
  Serial.print("Scale = ");  
  Serial.println(scale_index);
  }
}
