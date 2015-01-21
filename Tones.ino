
// TONES  ==========================================
#define t_C    1911
#define t_C1    1804
#define t_D    1703
#define t_Eb    1607
#define t_E    1517
#define t_F    1432
#define t_F1    1352
#define t_G    1276
#define t_Ab    1204
#define t_A    1136
#define t_Bb    1073
#define t_B    1012
#define t_c       955
#define t_c1      902
#define t_d       851
#define t_eb      803
#define t_e       758
#define t_f       716
#define t_f1      676
#define t_g       638
#define t_ab      602
#define t_a       568
#define t_bb      536
#define t_b       506
#define t_p       0  //pause
//==================================================

// Set overall tempo
long tempo = 20000;
// Set length of pause between notes
int pause = 1000;
// Loop variable to increase Rest length
int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES
// Initialize core variables
int t_ = 0;
int beat = 0;
long duration  = 0;


// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[], 
//  which sets each note's relative length (higher #, longer note) 
int melody[] = {t_e, t_e, t_e, t_c, t_e, t_g, t_G, t_c, t_G, t_E, t_A, t_B, t_Bb, t_A, t_G, t_e, t_g, t_a, t_f, t_g, t_e, t_c, t_d, t_B, t_c};
int beats[] = {6, 12, 12, 6, 12, 24, 24, 18, 18, 18, 12, 12, 6, 12, 8, 8, 8, 12, 6, 12, 12, 6, 6, 6, 12};
int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.


//int peergynt_m[] = {t_G, t_E, t_D, t_C, t_D, t_E, t_G, t_E, t_D, t_C, t_D, t_E, t_D, t_E, t_G, t_E, t_G, t_A, t_E, t_A, t_G, t_E, t_D, t_C};
//int peergynt_r[] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 16};
/* 
int smoke_m[] = {E, G, A, E, G, Bb, A, E, G, A, G, E};
int smoke_r[] = {12, 12, 18, 12, 12, 6, 18, 12, 12, 18, 12, 24};
 
int natal_m[] = {G, A, G, E, G, A, G, E, c, c, A, B, B, G, A, G, A, c, B, A, G, A, G, E};
int natal_r[] = {12, 4, 8, 16, 12, 4, 8, 16, 12, 4, 16, 12, 4, 16, 12, 4, 8, 8, 8, 8, 12, 4, 8, 16};
 
int LTS_m[] = {Bb, G, G, Bb, G, G, Bb, ;G, G, Bb, G, G, Bb, G, C, G, Bb, G, G, Bb, G, G, Bb, G, G, Bb, G, G, Bb, G, F, D, F, D, G, F, D, C, Bb, G, Bb, C, C1, C, Bb, F, D, Bb, G, F, D, C, Bb, D, C, Bb, G};
int LTS_r[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

*/

void doTone(int myTone, long length)
{
  if(!mute)
  {
    tone(PIEZO_PIN,myTone,length);
  }
}

void shortBip()
{
  doTone(t_c,100);
  delay(50);  
}

void bip()
{
  doTone(t_C,200);
  delay(200);  
}

void timeEndBeep()
{
  doTone(t_C,1000);
  delay(1000);  
}


// PLAY MELODY  =======================================
void tune() 
{
  // Set up a counter to pull from melody[] and beats[]
  for (int i=0; i<MAX_COUNT; i++) {
    
    //t_ = peergynt_m[i];
    //beat = peergynt_r[i];
    
    
    t_ = melody[i];
    beat = beats[i];
    
    
    duration = beat * tempo; // Set up timing

    beep(); 
    // A pause between notes...
    delayMicroseconds(pause);

    if (DEBUG) { // If debugging, report loop, tone, beat, and duration
      Serial.print(i);
      Serial.print(":");
      Serial.print(beat);
      Serial.print(" ");    
      Serial.print(t_);
      Serial.print(" ");
      Serial.println(duration);
    }
  }
  delayMicroseconds(pause);
}

// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration
void beep()
{
  /*
  if(mute)//if muted, then just play rests
  {
    t_ = 0;
  }
  */
 long elapsed_time = 0;
  if (t_ > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {

      digitalWrite(PIEZO_PIN,HIGH);
      delayMicroseconds(t_ / 2);

      // DOWN
      digitalWrite(PIEZO_PIN, LOW);
      delayMicroseconds(t_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (t_);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }          
}
