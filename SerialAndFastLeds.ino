#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>

//THREADS
ThreadController controll = ThreadController();
Thread threadRead = Thread();
Thread threadLight = Thread();

//SETTING STRIPS
#define PIN 2
#define NUM_LEDS 875
#define PIN2 1
#define NUM_LEDS2 0
#define PIN3 5
#define NUM_LEDS3 0

//ACTIVE STRIPS
boolean isActiveStrip1 = true;
boolean isActiveStrip2 = true;
boolean isActiveStrip3 = true;

//CREATING STRIPS
CRGB strip1[NUM_LEDS];
CRGB strip2[NUM_LEDS2];
CRGB strip3[NUM_LEDS3];

//STRIP EVENT
int strip1Event;
int strip2Event;
int strip3Event;

//STRIP RGBS
int r1 = 0, g1 = 0, b1 = 0;
int r2 = 0, g2 = 0, b2 = 0;
int r3 = 0, g3 = 0, b3 = 0;

//STRIP COUNTERS
int strip1Count;
int strip2Count;
int strip3Count;
int strip1CountJ;
int strip2CountJ;
int strip3CountJ;

//VELOCITY
int Speed = 5;

void serialReadCall() {
  if (Serial.available() > 0) {
    String recivedDataStr = "";
    char recivedChar;
    while (Serial.available() > 0) {
      recivedChar = Serial.read();
      if (recivedChar != '\n') {
        // Concatena valores
        recivedDataStr.concat(recivedChar);
        delay(10);
      }
    }
    Serial.println(recivedDataStr);

    //LEITURA RGB #R255G255B255S0
    if (recivedDataStr.indexOf("#") >= 0) {
      String Red1 = "";
      String Green1 = "";
      String Blue1 = "";
      String Red2 = "";
      String Green2 = "";
      String Blue2 = "";
      String Red3 = "";
      String Green3 = "";
      String Blue3 = "";
      int indexR = recivedDataStr.indexOf("R");
      int indexG = recivedDataStr.indexOf("G");
      int indexB = recivedDataStr.indexOf("B");
      int indexS = recivedDataStr.indexOf("S");
      int indexSize = recivedDataStr.length();
      int str_len = recivedDataStr.length() + 1;
      char char_array[str_len];
      recivedDataStr.toCharArray(char_array, str_len);
      String idStrip = "";
      for (int cont = indexS + 1; cont < indexSize + 1; cont++) {
        idStrip.concat(char_array[cont]);
      }
      int idStri = idStrip.toInt();

      if (idStri == 0 ) {

        for (int cont = indexR + 1; cont < indexG; cont++) {
          Red1.concat(char_array[cont]);
        }
        for (int cont = indexG + 1; cont < indexB; cont++) {
          Green1.concat(char_array[cont]);
        }
        for (int cont = indexB + 1; cont < indexS; cont++) {
          Blue1.concat(char_array[cont]);
        }
        Serial.println("RED 1: " + Red1 + " Green1: " + Green1 + " Blue1: " + Blue1);
        Serial.println("RED 2: " + Red1 + " Green2: " + Green1 + " Blue2: " + Blue1);
        r1 = Red1.toInt();
        g1 = Green1.toInt();
        b1 = Blue1.toInt();
        r2 = Red1.toInt();
        g2 = Green1.toInt();
        b2 = Blue1.toInt();
        r3 = Red1.toInt();
        g3 = Green1.toInt();
        b3 = Blue1.toInt();
      }

      if (idStri == 1) {
        for (int cont = indexR + 1; cont < indexG; cont++) {
          Red1.concat(char_array[cont]);
        }
        for (int cont = indexG + 1; cont < indexB; cont++) {
          Green1.concat(char_array[cont]);
        }
        for (int cont = indexB + 1; cont < indexS; cont++) {
          Blue1.concat(char_array[cont]);
        }
        r1 = Red1.toInt();
        g1 = Green1.toInt();
        b1 = Blue1.toInt();
      }
      else if (idStri == 2) {


        for (int cont = indexR + 1; cont < indexG; cont++) {
          Red2.concat(char_array[cont]);
        }
        for (int cont = indexG + 1; cont < indexB; cont++) {
          Green2.concat(char_array[cont]);
        }
        for (int cont = indexB + 1; cont < indexS; cont++) {
          Blue2.concat(char_array[cont]);
        }

        r2 = Red2.toInt();
        g2 = Green2.toInt();
        b2 = Blue2.toInt();

      }
      else if (idStri == 3) {


        for (int cont = indexR + 1; cont < indexG; cont++) {
          Red3.concat(char_array[cont]);
        }
        for (int cont = indexG + 1; cont < indexB; cont++) {
          Green3.concat(char_array[cont]);
        }
        for (int cont = indexB + 1; cont < indexS; cont++) {
          Blue3.concat(char_array[cont]);
        }

        r3 = Red3.toInt();
        g3 = Green3.toInt();
        b3 = Blue3.toInt();

      }


    }//FIM DA LEITURA RGB

    //LEITURA DE EVENTO
    if (recivedDataStr.indexOf("@") >= 0) {
      int indexId = recivedDataStr.indexOf("@");
      int indexD = recivedDataStr.indexOf("D");
      int indexS = recivedDataStr.indexOf("S");
      int indexSize = recivedDataStr.length();
      int str_len = recivedDataStr.length() + 1;
      char char_array[str_len];
      recivedDataStr.toCharArray(char_array, str_len);
      String Speed = "";
      String idEvent = "";
      String idStrip = "";

      for (int cont = indexId + 1; cont < indexD; cont++) {
        idEvent.concat(char_array[cont]);

      }

      for (int cont = indexD + 1; cont < indexS; cont++) {
        Speed.concat(char_array[cont]);

      }

      for (int cont = indexS + 1; cont < indexSize; cont++) {
        idStrip.concat(char_array[cont]);

      }
      int ID = idEvent.toInt();

      int idStri = idStrip.toInt();


      if (idStri == 1) {
        strip1Event = ID;
        strip1Count = 0;
        strip1CountJ = 0;
      }
      else if (idStri == 2) {
        strip2Event = ID;
        strip2Count = 0;
        strip2CountJ = 0;
      }
      else if (idStri == 3) {
        strip3Event = ID;
        strip3Count = 0;
        strip3CountJ = 0;
      }
      else if (idStri == 0) {
        strip1Event = ID;
        strip2Event = ID;
        strip3Event = ID;

        strip1Count = 0;
        strip2Count = 0;
        strip3Count = 0;

        strip1CountJ = 0;
        strip2CountJ = 0;
        strip3CountJ = 0;
      }
    }
    //FIM EVENTO READ

  }

}

void lightCall() {

  //STRIP 1 EVENT
  if (isActiveStrip1)
    switch (strip1Event) {
      case 0: {
          simpleRainbow(0, 1);
        }

        break;
      case 1: {
          setAll(r1, g1, b1, 1);
        }

        break;
      case 2: {
          sincroRainbow(0);
        }

        break;
    }

  //STRIP 2 EVENT
  if (isActiveStrip2)
    switch (strip2Event) {
      case 0: {
          simpleRainbow(0, 2);
        }

        break;
      case 1: {
          setAll(r2, g2, b2, 2);
        }

        break;
      case 2: {

        }

        break;
    }

  //STRIP 3 EVENT
  if (isActiveStrip3)
    switch (strip3Event) {
      case 0: {
          simpleRainbow(0, 3);
        }

        break;
      case 1: {
          setAll(r3, g3, b3, 3);
        }

        break;
      case 2: {

        }

        break;
    }


}

//SETUP INFORMATIONS
void setup() {
  Serial.begin(115200);

  Serial.println("Teensy ON");
  Serial.println();

  //INITIALIZE ALL STRIPS
  FastLED.addLeds<WS2812B, PIN, GRB>(strip1, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812B, PIN2, GRB>(strip2, NUM_LEDS2).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812B, PIN3, GRB>(strip3, NUM_LEDS3).setCorrection( TypicalLEDStrip );

  //Setup StartEvents
  strip1Event = 0;
  strip2Event = 2;
  strip3Event = 2;


  //ADDING THREADS
  threadRead.onRun(serialReadCall);
  threadRead.setInterval(1);

  threadLight.onRun(lightCall);
  threadLight.setInterval(2);

  controll.add(&threadRead);
  controll.add(&threadLight);

}

void loop() {
  controll.run();
}

//SHOW SPECIFC STRIP
void showStrip() {
  FastLED.show();

}

//SET A SPECIFC PIXEL INTO A SPECIFC STRIP
void setPixel(int Pixel, byte red, byte green, byte blue, int stripID) {

  if (stripID == 1) {
    strip1[Pixel].r = red;
    strip1[Pixel].g = green;
    strip1[Pixel].b = blue;
  }
  else if (stripID == 2) {
    strip2[Pixel].r = red;
    strip2[Pixel].g = green;
    strip2[Pixel].b = blue;
  }
  else if (stripID == 3) {
    strip3[Pixel].r = red;
    strip3[Pixel].g = green;
    strip3[Pixel].b = blue;
  }
}

//SET ALL PIXELS TO A SPECIFC STRIP
void setAll(byte red, byte green, byte blue, int stripID) {

  int numberLeds;
  if (stripID == 1) {
    numberLeds = NUM_LEDS;
  }
  else if (stripID == 2) {
    numberLeds = NUM_LEDS2;
  }
  else if (stripID == 3) {
    numberLeds = NUM_LEDS3;
  }
  for (int i = 0; i < numberLeds; i++ ) {
    setPixel(i, red, green, blue, stripID);
  }
  showStrip();
}

//LED EFFECTS ------------------------------------------------------------------------------------------------- x x x x x x x x x x x x ---------------------------------------------------------------------------------------------------------------

//WHEEL FOR RAINBOW
byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }
  delay(0);
  return c;
}


void simpleRainbow(int SpeedDelay, int stripID) {
  byte *c;
  int contTempo;
  int numberLeds;
  int cont;

  if (stripID == 1) {
    contTempo = strip1Count;
    strip1Count++;
    numberLeds = NUM_LEDS;
    cont = strip1CountJ;
  }
  else if (stripID == 2) {
    contTempo = strip2Count;
    strip2Count++;
    numberLeds = NUM_LEDS2;
    cont = strip2CountJ;
  }
  else if (stripID == 3) {
    contTempo = strip3Count;
    strip3Count++;
    numberLeds = NUM_LEDS3;
    cont = strip3CountJ;
  }

  if (contTempo >= SpeedDelay) {
    for (int i = 0; i < numberLeds; i++) {
      c = Wheel(((i * 256 / numberLeds) + cont) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2), stripID);
    }

    contTempo = 0;
    if (stripID == 1) {
      strip1Count = contTempo;
      strip1CountJ++;
    }
    else if (stripID == 2) {
      strip2Count = contTempo;
      strip2CountJ++;
    }
    else if (stripID == 3) {
      strip3Count = contTempo;
      strip3CountJ++;
    }
    showStrip();
  }


}

//SINCRO RAINBOW
void sincroRainbow(int SpeedDelay) {
  byte *c;
  int contTempo;
  int numberLeds;
  int cont;

  numberLeds = NUM_LEDS + NUM_LEDS2 + NUM_LEDS3;
  contTempo = strip1Count;
  strip1Count++;
  cont = strip1CountJ;
  if (contTempo >= SpeedDelay) {

    for (int i = 0; i <= numberLeds; i++) {
      c = Wheel(((i * 256 / numberLeds) + cont) & 255);
      if (i < NUM_LEDS) {
        setPixel(i, *c, *(c + 1), *(c + 2), 1);
      }
      else if (i >= NUM_LEDS && i <= (NUM_LEDS + NUM_LEDS2)) {
        setPixel(i - NUM_LEDS, *c, *(c + 1), *(c + 2), 2);
      }
      else if (i > NUM_LEDS2 && i <= numberLeds) {
        setPixel(NUM_LEDS3 - (i - (NUM_LEDS + NUM_LEDS2)), *c, *(c + 1), *(c + 2), 3);
      }
      delay(0);
    }

    contTempo = 0;

    strip1Count = contTempo;
    strip1CountJ++;

    showStrip();
  }
}
