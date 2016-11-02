//Import this code back into the Teensy Audio System Design Tool to alter any portion of this system 

// GUItool: begin automatically generated code
AudioSynthWaveformDc     dc2;            //xy=189,206
AudioSynthWaveformSine   sine3;          //xy=189,250
AudioSynthWaveformDc     dc6; //xy=188,564
AudioSynthWaveformSine   sine7; //xy=188,608
AudioSynthWaveformSine   sine2;          //xy=190,164
AudioSynthWaveformSine   sine6; //xy=189,522
AudioSynthWaveformSine   sine1;          //xy=192,78
AudioSynthWaveformSine   sine4;          //xy=191,334
AudioSynthWaveformDc     dc1;            //xy=192,120
AudioSynthWaveformSine   sine5; //xy=191,436
AudioSynthWaveformSine   sine8; //xy=190,692
AudioSynthWaveformDc     dc5; //xy=191,478
AudioSynthWaveformDc     dc4;            //xy=192,378
AudioSynthWaveformDc     dc8; //xy=191,736
AudioSynthWaveformDc     dc3;            //xy=194,294
AudioSynthWaveformDc     dc7; //xy=193,652
AudioEffectMultiply      multiply3;      //xy=346,268
AudioEffectMultiply      multiply4;      //xy=346,354
AudioEffectMultiply      multiply7; //xy=345,626
AudioEffectMultiply      multiply8; //xy=345,712
AudioEffectMultiply      multiply2;      //xy=350,184
AudioEffectMultiply      multiply6; //xy=349,542
AudioEffectMultiply      multiply1;      //xy=353,95
AudioEffectMultiply      multiply5; //xy=352,453
AudioMixer4              mixer1;         //xy=565,245
AudioMixer4              mixer2; //xy=564,603
AudioMixer4              mixerMain;         //xy=736,418
AudioOutputI2S           i2s1;           //xy=881,466
AudioConnection          patchCord1(dc2, 0, multiply2, 1);
AudioConnection          patchCord2(sine3, 0, multiply3, 0);
AudioConnection          patchCord3(dc6, 0, multiply6, 1);
AudioConnection          patchCord4(sine7, 0, multiply7, 0);
AudioConnection          patchCord5(sine2, 0, multiply2, 0);
AudioConnection          patchCord6(sine6, 0, multiply6, 0);
AudioConnection          patchCord7(sine1, 0, multiply1, 0);
AudioConnection          patchCord8(sine4, 0, multiply4, 0);
AudioConnection          patchCord9(dc1, 0, multiply1, 1);
AudioConnection          patchCord10(sine5, 0, multiply5, 0);
AudioConnection          patchCord11(sine8, 0, multiply8, 0);
AudioConnection          patchCord12(dc5, 0, multiply5, 1);
AudioConnection          patchCord13(dc4, 0, multiply4, 1);
AudioConnection          patchCord14(dc8, 0, multiply8, 1);
AudioConnection          patchCord15(dc3, 0, multiply3, 1);
AudioConnection          patchCord16(dc7, 0, multiply7, 1);
AudioConnection          patchCord17(multiply3, 0, mixer1, 2);
AudioConnection          patchCord18(multiply4, 0, mixer1, 3);
AudioConnection          patchCord19(multiply7, 0, mixer2, 2);
AudioConnection          patchCord20(multiply8, 0, mixer2, 3);
AudioConnection          patchCord21(multiply2, 0, mixer1, 1);
AudioConnection          patchCord22(multiply6, 0, mixer2, 1);
AudioConnection          patchCord23(multiply1, 0, mixer1, 0);
AudioConnection          patchCord24(multiply5, 0, mixer2, 0);
AudioConnection          patchCord25(mixer1, 0, mixerMain, 0);
AudioConnection          patchCord26(mixer2, 0, mixerMain, 1);
AudioConnection          patchCord27(mixerMain, 0, i2s1, 0);
AudioConnection          patchCord28(mixerMain, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=528,409
// GUItool: end automatically generated code
