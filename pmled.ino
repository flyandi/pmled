/**
 * LED Driver for Paramotor or anything else
 * Mainly designed for WS2812 and Neopixels
 * http://github.com/flyandi/pmled
 * 
 * Demo: https://youtu.be/deen75N7XwA
 * 
 * Requires FastLED Library
 * 
 * Pin Layout (or change in config settings)
 * 6  = Right LED strip
 * 8  = Left LED strip
 * 10 = RC Channel
 * 
 * Mixing settings for your radio (Ex Taranis X7):
 * CH4 Input SD Weight +50% Offset 50%
 *     Input SC Weight +25% Offset -75%
 *     
 */

/**
 * @imports
 */
#include <FastLED.h>

/** 
 * @configuration
 */
#define NUM_LEDS 10
#define LEFT_PIN 8
#define RIGHT_PIN 6
#define RC_PIN 10

/**
 * @constants
 */
#define MODE_NAVIGATION 0
#define MODE_BLUE 1
#define MODE_DISCO 2
#define MODE_POLICE 3
#define MODE_OFF 4
#define MODE_LANDING 5
#define MODE_MINIMAL 6
#define MODE_BOOT 7
#define LEFT 0
#define RIGHT 1

/**
 * @globals
 */
CRGB left[NUM_LEDS];
CRGB right[NUM_LEDS];
int mode = 0;
int flash = false;
uint8_t gHue = 0; 
int channel;


/**
 * @setup
 */
void setup() { 

  pinMode(RC_PIN, INPUT);

  // Change here anything related to your LED setup 
  FastLED.addLeds<NEOPIXEL, LEFT_PIN>(left, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, RIGHT_PIN>(right, NUM_LEDS);
  delay(1000); // for recovery

  // first mode
  initMode(MODE_BOOT);  
}


/**
 * @loop
 */
void loop() { 

  EVERY_N_MILLISECONDS( 20 ) { gHue++; } 

  if(mode == MODE_BOOT) {
    Boot();
  }

  if(mode == MODE_NAVIGATION) {
    Navigation();
  }

  if(mode == MODE_LANDING) {
    Landing();
  }
  
  if(mode == MODE_DISCO) {
    Disco();
  }

  if(mode == MODE_POLICE) {
    Police();
  }

  FastLED.show();
  readChannel();
  delay(1000/120);
}


/**
 * @void
 */
void readChannel() {
  channel = pulseIn(10, HIGH, 25000);

  Serial.println(channel);

  if(channel < 1) {
    if(mode != MODE_BOOT) {
      initMode(MODE_BOOT);
    }
  } else if(channel < 1000) {
    if(mode != MODE_NAVIGATION)  {
      initMode(MODE_NAVIGATION);
    }
  } else if(channel < 1200) {
    if(mode != MODE_LANDING)  {
      initMode(MODE_LANDING);
    }
  } else if(channel <1300) {
    if(mode != MODE_OFF) {
      initMode(MODE_OFF);
    }
  } else if (channel <1400) {
     if(mode != MODE_POLICE) {
      initMode(MODE_POLICE);
     }
  } else if (channel <1500) {
    if(mode != MODE_MINIMAL)  {
      initMode(MODE_MINIMAL);
    }
   } else if (channel < 1700) {
    if(mode != MODE_DISCO)  {
      initMode(MODE_DISCO);
    }
  } else if (channel < 1800) {
    if(mode != MODE_BLUE)  {
      initMode(MODE_BLUE);
    }
    
  }
}

void Disco()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 40;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
 
    right[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    left[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void Boot() {
  EVERY_N_MILLISECONDS(500) {
    flash++;
    if(flash == 1) {
      on(LEFT, 4, 255, 0, 0);
      on(LEFT, 5, 255, 0, 0);
      on(RIGHT, 4, 255, 0, 0);
      on(RIGHT, 5, 255, 0, 0);
    } else {
      off(LEFT, 4);
      off(LEFT, 5);
      off(RIGHT, 4);
      off(RIGHT, 5);
      flash = 0;
    }
  }
}

void Navigation() {
  EVERY_N_MILLISECONDS(50) {
    flash++;
    if(flash == 1) {
      on(LEFT, 4, 255, 255, 255);
      on(RIGHT, 4, 255, 255, 255);
    } else if (flash == 3) {
      on(LEFT, 5, 255, 255, 255);
      on(RIGHT, 5, 255, 255, 255);
    } else {
      off(LEFT, 4);
      off(LEFT, 5);
      off(RIGHT, 4);
      off(RIGHT, 5);
    }

    if(flash > 20) flash = 0;
  }
}

void Landing() {
  EVERY_N_MILLISECONDS(700) {
    flash++;
    if(flash == 1) {
      on(LEFT, 9, 0, 0, 255);
      on(LEFT, 8, 0, 0, 255);
      on(LEFT, 7, 0, 0, 255);
      on(LEFT, 2, 0, 0, 255);
      off(RIGHT, 9);
      off(RIGHT, 8);
      off(RIGHT, 7);
      off(RIGHT, 2);
    } else {
      on(RIGHT, 9, 0, 0, 255);
      on(RIGHT, 8, 0, 0, 255);
      on(RIGHT, 7, 0, 0, 255);
      on(RIGHT, 2, 0, 0, 255);
      off(LEFT, 9);
      off(LEFT, 8);
      off(LEFT, 7);
      off(LEFT, 2);
      flash =0;
    }
  }
}


void Police() {
  EVERY_N_MILLISECONDS(200) {
    flash++;
    if(flash == 1) {
      fill_solid(right, NUM_LEDS, CRGB::Blue);
    } else if (flash == 3) {
      fill_solid(left, NUM_LEDS, CRGB::Red);
    } else {
      clearAll();
    }

    if(flash > 3) flash = 0;
  }
}



void clearAll() {
  FastLED.clear();
  FastLED.show();
}


void initMode(int m) {
  mode = m;

  flash = 0;

  clearAll();


  if(mode == MODE_NAVIGATION) {
    for(int i = 0; i < 10; i++) {
      on(LEFT, i, 255, 0, 0);
      on(RIGHT,i, 0, 255, 0);
    }
    
  }

  if(mode == MODE_MINIMAL) {
     on(LEFT, 4, 255, 0, 0);
    on(LEFT, 5, 255, 0, 0);
    on(RIGHT,4, 0, 255, 0);
    on(RIGHT,5, 0, 255, 0);
  }

  if(mode == MODE_LANDING) {
    on(LEFT, 3, 255, 0, 0);
    on(LEFT, 4, 255, 0, 0);
    on(LEFT, 5, 255, 0, 0);
    on(LEFT, 6, 255, 0, 0);

    on(RIGHT,3, 0, 255, 0);
    on(RIGHT,4, 0, 255, 0);
    on(RIGHT,5, 0, 255, 0);
    on(RIGHT,6, 0, 255, 0);
    
    on(LEFT, 0, 255, 255, 255);
    on(LEFT, 1, 255, 255, 255);
    on(RIGHT, 0, 255, 255, 255);
    on(RIGHT, 1, 255, 255, 255);

  }

  if(mode == MODE_BLUE) {
    for(int i = 0; i < 5; i++) {
      on(LEFT, i, 255, 0, 0);
      on(RIGHT,i, 0, 255, 0);
    }

    for(int i = 5; i < 10; i++) {
      on(LEFT, i, 0, 0, 255);
      on(RIGHT,i, 0, 0, 255);
    }

     on(LEFT, 5, 255, 255, 255);
     on(RIGHT,5, 255, 255, 255);
  }

}

void off(int type, int index) {
  on(type, index, 0, 0, 0);
}

void on(int type, int index, byte r, byte g, byte b) {

  switch(type) {
    case LEFT: left[index].setRGB(r, g, b); break;
    case RIGHT: right[index].setRGB(r, g, b); break;
  }
}
