#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <Adafruit_NeoPixel.h>
#include <ezButton.h>

ezButton buttonUp(7);
ezButton buttonDown(8);
ezButton buttonMode(9);
unsigned long M_button_pressed;
unsigned long M_button_released;

#define Button0  0xFF6897
#define Button1  0xFF30CF
#define Button2  0xFF18E7
#define Button3  0xFF7A85
#define Button4  0xFF10EF
#define Button5  0xFF38C7
#define Button6  0xFF5AA5
#define Button7  0xFF42BD
#define Button8  0xFF4AB5
#define Button9  0xFF52AD
#define Bhun  0xFF9867
#define B200  0xFFB04F
#define Bplus  0xFFA85F
#define Bminus  0xFFE01F
#define Bchplus 0xFFE21D
#define Beq 0xFF906F
#define Bright 0xFF02FD
#define Bleft 0xFF22DD
#define Bplay 0xFFC23D
#define Bchminus 0xFFA25D
#define Bch 0xFF629D

#define PIN 6
#define NUMPIXELS 30 // updateLength() --> Change the length of a previously-declared 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;

long int CODES[21] = {Button0, Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8, Button9, Bhun, B200, Bplus, Bminus, Bchplus, Beq, Bright, Bleft, Bplay, Bchminus, Bch};
long int NewCodes[21];
long int NewCode;
int N = 0;
String data;
char d;
int buttonshift = 0;

bool modeloop = true; // sine8(), gamma8()
int R = 0;
int G = 0;
int B = 255;

int sensorPin = A0;
int Compare[2];
int i = 0;
int SoundInBoosted;
int sensitivity = 1;
int SoundInterval = 50;
bool soundmode = true;
int soundmodecall = 0;

int interval = 100;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

struct color {
  int red;
  int blue;
  int green;
};

uint32_t serialColorInput = 0x0000ff;

void setSerialColorInput(uint32_t newColor) {
  serialColorInput = newColor;
}

void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  buttonUp.setDebounceTime(50);
  buttonDown.setDebounceTime(50);
  buttonMode.setDebounceTime(50);
}


void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 100) {
    previousMillis = currentMillis;
    findCode();
  }
}
//==================================================================================== COLORS ==============================================================================================


void ColorFadeFunction(bool r, bool g, bool b, bool UpOrDown, int L_1, int L_2, int counter) { // only one at time
  if (UpOrDown == true) {
    if (r == true) {
      if (R < L_2) {
        R += counter;
      }
      else {
        R = L_1;
      }
    } else if (g == true) {
      if (G < L_2) {
        G += counter;
      }
      else {
        G = L_1;
      }
    } else if (b == true) {
      if (B < L_2) {
        B += counter;
      }
      else {
        B = L_1;
      }
    }
  }
  if (UpOrDown == false) {
    if (r == true) {
      if (R >= L_1) {
        R -= counter;
      }
      else {
        R = L_2;
      }
    } else if (g == true) {
      if (G >= L_1) {
        G -= counter;
      }
      else {
        G = L_2;
      }
    } else if (b == true) {
      if (B >= L_1) {
        B -= counter;
      }
      else {
        B = L_2;
      }
    }
  }

}



//=================================================================================LED SOUND FUNCTIONS======================================================================================

void SoundMode() {

  soundmode = true;
  while (soundmode) {
    buttonUp.loop();
    buttonDown.loop();
    buttonMode.loop();
    int inputValue = analogRead(sensorPin);
    Compare[i] = inputValue;
    i++;

    if (i > 1)
    {
      SoundInBoosted = abs(Compare[0] - Compare[1]) * sensitivity;
      Serial.print("Input Modified difference: ");
      Serial.print(SoundInBoosted);
      Serial.print(", ");
      Serial.print("Sensitivity: ");
      Serial.print(sensitivity);
      Serial.print(", ");
      Serial.print("count: ");
      Serial.print(soundmodecall);
      Serial.print(", ");
      Serial.print("Threshold: ");
      Serial.println(inputValue);
      SoundModeList(soundmodecall, SoundInBoosted);
      i = 0;
    }

    if (buttonUp.isPressed()) {
      Serial.println("The button Up is pressed");
      if (sensitivity <= 201) {
        sensitivity += 5;
      }
    }

    if (buttonDown.isPressed()) {
      Serial.println("The button Down is pressed");
      if (sensitivity > 1) {
        sensitivity -= 5;
      }
    }

    if (buttonMode.isPressed()) {
      Serial.println("mode button pressed");
      M_button_pressed = millis();
    } else if (buttonMode.isReleased()) {
      M_button_released = millis();
      if (M_button_released - M_button_pressed >= 3000)
      {
        Serial.println("Go to Remote mode");
        soundmode = false;
      } else {
        if (soundmodecall < 6) {
          soundmodecall++;
        } else {
          soundmodecall = 0;
        }
      }
    }
    
   if (Serial.available() > 0){
      char value = Serial.read();
      switch(value){
        case  'Z': soundmode = false;
          break;
        case  'A': soundmodecall = 0;
          break;
        case  'B': soundmodecall = 1;
          break;
        case  'C': soundmodecall = 2;
          break;
        case  'D': soundmodecall = 3;
          break;
        case  'E': soundmodecall = 4;
          break;
        case  'F': soundmodecall = 5;
          break;
        case  'G': soundmodecall = 6;
          break;
      }
   }

    delay(SoundInterval);
  }
}


//=========================================================================LED REMOTE FUNCTIONS=================================================================================

void findCode() {
  buttonMode.loop();
  buttonUp.loop();
  buttonDown.loop();
  if (irrecv.decode(&results))
  {
    if (exist(results.value, CODES, 21))
    {
      LEDMODE(results.value); // calls mode
    }
    else if (exist(results.value, NewCodes, 21))
    {
      NewInBtn(results.value, NewCodes, 21);
    }
    irrecv.resume();

  } else if (Serial.available() > 0) {
    SerialIn();
  }

  if (buttonMode.isPressed()) {
    Serial.println("mode button pressed");
    M_button_pressed = millis();
  } else if (buttonMode.isReleased()) {
    M_button_released = millis();
    if (M_button_released - M_button_pressed >= 3000) {
      Serial.println("Go to sound mode");
      SoundMode();
    } else {
      if (buttonshift < 18) {
        buttonshift++;
      } else {
        buttonshift = 0;
      }
      ButtonSwitch(buttonshift);
    }
  }

  if (buttonUp.isPressed()) {
    if (interval <= 300) {
      Serial.println("Interval up");
      interval += 50;
    }
  } else if (buttonDown.isPressed()) {
    if (interval > 50) {
      Serial.println("Interval down");
      interval -= 50;
    }
  }
}

int exist(long int compareValue, long int arrayName[], int arraySize) {
  irrecv.resume();
  for (int x = 0; x < arraySize; x++) {
    if (arrayName[x] == compareValue) {
      return true;
    }
  }
  return false;
}

void upinterval()
{
  if (interval < 300) {
    interval += 50;
  }
}

void downinterval()
{
  if (interval > 100)
    interval -= 100;
}

//=========================================================================LED SERIAL FUNCTIONS==================================================================================

void IR_NewBtn(int N) //
{
  bool newloop = true;
  while (newloop)
  {
    if (irrecv.decode(&results))
    {
      NewCode = results.value;
      NewCodes[N] = NewCode;
      newloop = false;
    }
    delay(100);
  }

  irrecv.resume();
  for (int x = 0; x <= 10; x++)
  {
    Serial.println("Z"); // not working
    delay(200);
  }

}

//=========================================================================LED MODE LISTS========================================================================================

void SoundModeList(int count, int soundinput)
{
  switch (count) {

    case 0: LEDSoundResponse_blue(soundinput);
      break;

    case 1: LEDSoundResponse_red(soundinput);
      break;

    case 2: LedMidSoundResponse_red(soundinput);
      break;

    case 3: LedMidSoundResponse_blue(soundinput);
      break;

    case 4: SoundLEDFlash(soundinput);
      break;

    case 5: SoundRedFade(soundinput);
      break;

    case 6: Sound_blinkingstar(soundinput);
      break;

  }
}



void LEDMODE(long int code)
{
  irrecv.resume();
  switch (code) {
    case Button0: R = 255;
      G = 255;
      B = 255;
      whitejump();
      break;

    case Button1: redneonmove();
      break;

    case Button2: blueneonmove();
      break;

    case Button3: greenmove();
      break;

    case Button4: redflash();
      break;

    case Button5: B = 255;
      blueflash();
      break;

    case Button6: redsplitmove();
      break;

    case Button7: bluesplitmove();
      break;

    case Button8: B = 255;
      redyellowsplitmove();
      break;

    case Button9: redblueflash();
      break;

    case Bhun: chasing();
      break;

    case B200: beam();
      break;

    case Beq: beamV2();
      break;

    case Bchplus: kill();
      break;

    case Bplus: upinterval();
      break;

    case Bminus: downinterval();
      break;

    case Bright: purpledot();
      break;

    case Bplay: splitpaintmove();
      break;

    case Bchminus: Colorfade();
      break;

    case Bch: Fadingblueneon();
      break;

    case Bleft: blinkingstar();
      break;

  }
}


void SerialIn()
{
  String sdata = "";
  byte ch;
  while (Serial.available() > 0) {
    ch = Serial.read();
    sdata += (char)ch;
    if (ch == '\r') { // Command recevied and ready.
      sdata.trim();
    }
  }

  Serial.println(sdata);

  switch (sdata.charAt(0)) {
    case 'R':   setSerialColorInput(strtol(sdata.substring(1).c_str(), NULL, 16));
      break;

    case '0': kill();
      break;

    case '1': Colorfade();
      break;

    case '2': Fadingblueneon();
      break;

    case '3': splitpaintmove();
      break;

    case '4': blinkingstar();
      break;

    case '5': purpledot();
      break;

    case '6': beamV2();
      break;

    case '7': chasing();
      break;

    case '8': beam();
      break;

    case '9': whitejump();
      break;

    case 'A': redneonmove();
      break;

    case 'B': blueneonmove();
      break;

    case 'C': greenmove();
      break;

    case 'D': redflash();
      break;

    case 'E': blueflash();
      break;

    case 'F': redsplitmove();
      break;

    case 'G': bluesplitmove();
      break;

    case 'H': redyellowsplitmove();
      break;

    case 'I': redblueflash();
      break;

    case 'J': splitmove();
      break;

    case 'K': keyFlash();
      break;

    case 'L': keyRunFlash();
      break;

    case 'M': keyHalfFlash();
      break;

    case 'N':  IR_NewBtn(sdata.substring(1).toInt());
      break;

    case 'O': keySpiltFlash();
      break;

    case 'P': keySpiltFlash2();
      break;

    case 'Q': keyRunRight();
      break;

    case 'T': keyRunLeft();
      break;

    case 'S': interval = sdata.substring(1).toInt();
      break;

    case 'Z': SoundMode();
      break;

    default: break;
  }
}

void NewInBtn(long int compareValue, long int arrayName[], int arraySize)
{
  int z;
  for (int x = 0; x < arraySize; x++) {
    if (arrayName[x] == compareValue) {
      z = x;
    }
  }

  switch (z) {
    
    case 0: kill();
      break;

    case 1: Colorfade();
      break;

    case 2: Fadingblueneon();
      break;

    case 3: splitpaintmove();
      break;

    case 4: blinkingstar();
      break;

    case 5: purpledot();
      break;

    case 6: beamV2();
      break;

    case 7: chasing();
      break;

    case 8: beam();
      break;

    case 9: whitejump();
      break;

    case 10: redneonmove();
      break;

    case 11: blueneonmove();
      break;

    case 12: greenmove();
      break;

    case 13: redflash();
      break;

    case 14: blueflash();
      break;

    case 15: redsplitmove();
      break;

    case 16: bluesplitmove();
      break;

    case 17: redyellowsplitmove();
      break;

    case 18: redblueflash();
      break;

  }

}

void ButtonSwitch(int z)
{
  switch (z) {

    case 0: kill();
      break;

    case 1: Colorfade();
      break;

    case 2: Fadingblueneon();
      break;

    case 3: splitpaintmove();
      break;

    case 4: blinkingstar();
      break;

    case 5: purpledot();
      break;

    case 6: beamV2();
      break;

    case 7: chasing();
      break;

    case 8: beam();
      break;

    case 9: whitejump();
      break;

    case 10: redneonmove();
      break;

    case 11: blueneonmove();
      break;

    case 12: greenmove();
      break;

    case 13: redflash();
      break;

    case 14: blueflash();
      break;

    case 15: redsplitmove();
      break;

    case 16: bluesplitmove();
      break;

    case 17: redyellowsplitmove();
      break;

    case 18: redblueflash();
      break;


  }

}

//=========================================================================LED SOUND MODES========================================================================================

void LEDSoundResponse_red(int x) {
  pixels.clear();
  int limit = x / (1000 / NUMPIXELS);
  B = 0;
  G = 0;
  R = 1;
  for (int i = 0; i <= limit; i++) {
    if (R >= 100) {
      G += 10;
    }
    pixels.setPixelColor(i, pixels.Color(R, G, B));
    pixels.show();
    R += 8;
  }
}

void LEDSoundResponse_blue(int x) {
  pixels.clear();
  int limit = x / (1000 / NUMPIXELS);
  G = 0;
  B = 1;
  R = 0;
  for (int i = 0; i <= limit; i++) {
    if (B >= 100) {
      R += 10;
    }
    pixels.setPixelColor(i, pixels.Color(R, G, B));
    pixels.show();
    B += 8;
  }
}

void LedMidSoundResponse_red(int x) {
  pixels.clear();
  int limit = x / (500 / NUMPIXELS);
  G = 0;
  R = 1;
  for (int i = 0; i <= limit; i++) {
    if (R >= 100) {
      G += 10;
    }
    pixels.setPixelColor((NUMPIXELS / 2) + i, pixels.Color(R, G, B));
    pixels.setPixelColor((NUMPIXELS / 2) - i, pixels.Color(R, G, B));
    pixels.show();
    R += 16;
  }
}

void LedMidSoundResponse_blue(int x) {
  pixels.clear();
  int limit = x / (500 / NUMPIXELS);
  G = 0;
  B = 1;
  R = 0;
  for (int i = 0; i <= limit; i++) {
    if (B >= 100) {
      R += 10;
    }
    pixels.setPixelColor((NUMPIXELS / 2) + i, pixels.Color(R, G, B));
    pixels.setPixelColor((NUMPIXELS / 2) - i, pixels.Color(R, G, B));
    pixels.show();
    B += 16;
  }
}


void SoundLEDFlash(int x) {
  if (x <= 10) {
    R = 0;
    B = 0;
    G = 255;
  } else if (x > 10 && x <= 75) {
    R = 0;
    G = 255;
    B = 255;
  } else if (x > 75 && x <= 200) {
    B = 0;
    R = 255;
    G = 255;
  } else if (x > 200 && x < 300) {
    R = 0;
    G = 0;
    B = 255;
  } else if (x > 300 && x <= 400) {
    G = 0;
    R = 255;
    B = 255;
  } else if (x > 400) {
    G = 0;
    B = 0;
    R = 255;
  }

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(R, G, B));
  }
  pixels.show();
}

void SoundRedFade(int x) {
  B = 0;
  G = 0;
  if (x <= 10) {
    R = 5;
  } else if (x > 10 && x <= 75) {
    R = 50;
  } else if (x > 75 && x <= 200) {
    R = 100;
  } else if (x > 200 && x < 300) {
    R = 150;
  } else if (x > 300 && x <= 400) {
    R = 200;
  } else if (x > 400) {
    R = 255;
  }
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(R, G, B));
  }
  pixels.show();

}

void Sound_blinkingstar(int x) {
  int divident;
  if (x <= 10) {
    divident = 15;
  } else if (x > 10 && x <= 75) {
    divident = 10;
  } else if (x > 75 && x <= 200) {
    divident = 5;
  } else if (x > 200 && x < 400) {
    divident = 3;
  } else if (x > 400 && x <= 500) {
    divident = 2;
  } else if (x > 500) {
    divident = 1;
  }
  for (int x = 0; x < (NUMPIXELS / divident); x++) {
    int i = rand() % NUMPIXELS;
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
  }
  pixels.show();
  findCode();
  pixels.clear();

}


//========================================================================= LED REMOTE MODES ======================================================================================
class bounceBeam {
    int currentPosition;
    int homePosition;
    uint32_t color;
    int len;
    bool dirc;
    bool beamTail;
    int RED, GREEN, BLUE;
    int Brightness = 255;

  public:
    bounceBeam(int beamLen, bool beamDirc, uint32_t beamColor, int startpos, bool tail = false) {
      len = beamLen;
      dirc = beamDirc;
      color = beamColor;
      currentPosition = homePosition = startpos;
      beamTail = tail;
    }

    bounceBeam(int beamLen, bool beamDirc, int R, int G, int B, int startpos, bool tail = true) {
      len = beamLen;
      dirc = beamDirc;
      currentPosition = homePosition = startpos;
      beamTail = tail;
      RED = R;
      GREEN = G;
      BLUE = B;
    }

    void shift(int j = 1) {
      if (dirc) {
        currentPosition += j;
        for (int i = 0; i < len; i++) {
          pixels.setPixelColor(currentPosition + i, color);
        }
      } else {
        currentPosition -= j;
        for (int i = 0; i < len; i++) {
          pixels.setPixelColor(currentPosition + i, color);
        }
      }
    }

    int getPosition() {
      return currentPosition;
    }

    bool getDirc() {
      return dirc;
    }

    int getLength() {
      return len;
    }

    void switchDirc() {
      if (dirc) {
        dirc = false;
      } else {
        dirc = true;
      }
    }

    void setColor(uint32_t beamColor) {
      color = beamColor;
    }

    uint32_t getColor() {
      return color;
    }

    void switchColor(uint32_t colorA, uint32_t colorB) {
      if (getColor() == colorA) {
        setColor(colorB);
      } else {
        setColor(colorA);
      }
    }

    void reborn() {
      currentPosition = homePosition;
    }

};


void kill() {
  buttonshift = 0;
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 100)
    {
      previousMillis = currentMillis;
      pixels.fill(0x000000, 0, NUMPIXELS);
      pixels.show();
      findCode();
    }
  }
}

//==========================================================================Key Modes==================================================================================


void keyFlash() {
  bool flag = true;
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      if (flag) {
        pixels.fill(0xffffff, 0, NUMPIXELS);
        flag = false;
      } else {
        pixels.clear();
      }
      pixels.show();
    }
    findCode();
  }
}

void keyRunFlash() {
  bounceBeam A(3, true, 0xffffff, 0);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 20)
    {
      previousMillis = currentMillis;
      if (A.getPosition() < NUMPIXELS) {
        pixels.clear();
        A.shift();
      }
      pixels.show();
    }
    findCode();
  }
}

void keyHalfFlash() {
  bool flag = true;
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      pixels.clear();
      previousMillis = currentMillis;
      if (flag) {
        for (int i = 0; i < NUMPIXELS; i += 2) {
          pixels.setPixelColor(i, 0xffffff);
        }
        flag = false;
      } else {
        pixels.clear();
      }
      pixels.show();
    }
    findCode();
  }
}

void keySpiltFlash() {
  bounceBeam A(3, true, 0xffffff, 0);
  bounceBeam B(3, false, 0xffffff, NUMPIXELS);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 20)
    {
      previousMillis = currentMillis;
      if (A.getPosition() < NUMPIXELS + B.getLength()) {
        pixels.clear();
        A.shift();
        B.shift();
      }
      pixels.show();

    }
    findCode();
  }
}

void keySpiltFlash2() {
  bounceBeam A(3, true, 0xffffff, 0);
  bounceBeam B(3, false, 0xffffff, NUMPIXELS);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 20)
    {
      previousMillis = currentMillis;

      if (A.getPosition() == B.getPosition()) {
        A.switchDirc();
        B.switchDirc();
        A.setColor(0x000000);
        B.setColor(0x000000);
      }

      if (A.getPosition() >= 0 && A.getPosition() <= NUMPIXELS / 2) {
        A.shift();
        B.shift();
      }

      pixels.show();

    }
    findCode();
  }
}

void keyRunRight() {
  bounceBeam A(1, true, 0xffffff, 0);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 10)
    {
      previousMillis = currentMillis;
      if (A.getPosition() < NUMPIXELS) {
        A.shift();
      } else {
        pixels.clear();
      }
      pixels.show();
    }
    findCode();
  }
}

void keyRunLeft() {
  bounceBeam A(1, false, 0xffffff, NUMPIXELS);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 10)
    {
      previousMillis = currentMillis;
      if (A.getPosition() > 0) {
        A.shift();
      } else {
        pixels.clear();
      }
      pixels.show();
    }
    findCode();
  }
}

//===================================================================================================================================================================================

void greenmove() // change name
{
  buttonshift = 12;
  bounceBeam A(5, true, pixels.Color(0, 255, 150), 0);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      pixels.clear();
      A.shift();

      if (A.getPosition() == 0 || A.getPosition() == NUMPIXELS - A.getLength())
      {
        A.switchDirc();
      }

      if (A.getPosition() % 2 == 0)
      {
        A.setColor(pixels.Color(255, 50, 0));
      } else {
        A.setColor(pixels.Color(0, 255, 255));
      }

      pixels.show();
      findCode();
    }
  }
}



void redneonmove()
{
  buttonshift = 10;
  bounceBeam A(5, true, 255, 0, 0, 0);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      pixels.clear();
      A.shift();

      if (A.getPosition() == 0 || A.getPosition() == NUMPIXELS - A.getLength())
      {
        A.switchDirc();
      }

      pixels.show();
      findCode();
    }
  }
}



void blueneonmove()
{
  buttonshift = 11;
  while (true) {

    int i = 0;
    while (i <= NUMPIXELS - 4)
    {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.clear(); // Set all pixel colors to 'off'
        pixels.setPixelColor(i, pixels.Color(50, 0, 50));
        pixels.setPixelColor(i + 1, pixels.Color(100, 0, 50));
        pixels.setPixelColor(i + 2, pixels.Color(150, 0, 50));
        pixels.setPixelColor(i + 3, pixels.Color(200, 0, 50));
        pixels.setPixelColor(i + 4, pixels.Color(255, 0, 50));
        pixels.show();   // Send the updated pixel colors to the hardware.
        i++;
        findCode();
      }
    }

    previousMillis = 0;
    while (true) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.setPixelColor(NUMPIXELS - 5, pixels.Color(100, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 4, pixels.Color(150, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 3, pixels.Color(200, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 2, pixels.Color(255, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 1, pixels.Color(200, 0, 50));
        pixels.show();
        break;
      }
    }
    while (true) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.setPixelColor(NUMPIXELS - 5, pixels.Color(150, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 4, pixels.Color(200, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 3, pixels.Color(255, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 2, pixels.Color(200, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 1, pixels.Color(150, 0, 50));
        pixels.show();
        break;
      }
    }
    while (true) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.setPixelColor(NUMPIXELS - 5, pixels.Color(200, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 4, pixels.Color(255, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 3, pixels.Color(200, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 2, pixels.Color(150, 0, 50));
        pixels.setPixelColor(NUMPIXELS - 1, pixels.Color(100, 0, 50));
        pixels.show();
        break;
      }
    }




    while (i >= 4)
    { // For each pixel..
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.clear(); // Set all pixel colors to 'off'
        pixels.setPixelColor(i, pixels.Color(50, 0, 50));
        pixels.setPixelColor(i - 1, pixels.Color(100, 0, 50));
        pixels.setPixelColor(i - 2, pixels.Color(150, 0, 50));
        pixels.setPixelColor(i - 3, pixels.Color(200, 0, 50));
        pixels.setPixelColor(i - 4, pixels.Color(255, 0, 50));
        pixels.show();   // Send the updated pixel colors to the hardware.
        i--;
        findCode();
      }
    }


    previousMillis = 0;
    while (true) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.setPixelColor(4, pixels.Color(100, 0, 50));
        pixels.setPixelColor(3, pixels.Color(150, 0, 50));
        pixels.setPixelColor(2, pixels.Color(200, 0, 50));
        pixels.setPixelColor(1, pixels.Color(255, 0, 50));
        pixels.setPixelColor(0, pixels.Color(200, 0, 50));
        pixels.show();
        break;
      }
    }
    while (true) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.setPixelColor(4, pixels.Color(150, 0, 50));
        pixels.setPixelColor(3, pixels.Color(200, 0, 50));
        pixels.setPixelColor(2, pixels.Color(255, 0, 50));
        pixels.setPixelColor(1, pixels.Color(200, 0, 50));
        pixels.setPixelColor(0, pixels.Color(150, 0, 50));
        pixels.show();
        break;
      }
    }
    while (true) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.setPixelColor(4, pixels.Color(200, 0, 50));
        pixels.setPixelColor(3, pixels.Color(255, 0, 50));
        pixels.setPixelColor(2, pixels.Color(200, 0, 50));
        pixels.setPixelColor(1, pixels.Color(150, 0, 50));
        pixels.setPixelColor(0, pixels.Color(100, 0, 50));
        pixels.show();
        break;
      }
    }

  }


}



void redflash()
{
  buttonshift = 14;
  while (true) {
    pixels.clear();
    while (true)
    {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.fill(pixels.Color(255, 0, 0), 0, (NUMPIXELS / 3));
        pixels.fill(pixels.Color(255, 0, 0) , (NUMPIXELS / 3) * 2 , (NUMPIXELS / 3));
        findCode();
        pixels.show();
        break;
      }
    }

    pixels.clear();
    while (true) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.fill( pixels.Color(255, 0, 0), (NUMPIXELS / 3), (NUMPIXELS / 3));
        pixels.show();
        findCode();
        break;
      }
    }
  }
}


void blueflash()
{
  while (true) {
    pixels.clear();
    while (true)
    {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.fill(serialColorInput, 0, (NUMPIXELS / 3));
        pixels.fill(serialColorInput , (NUMPIXELS / 3) * 2 , (NUMPIXELS / 3));
        findCode();
        pixels.show();
        break;
      }
    }

    pixels.clear();
    while (true) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.fill( serialColorInput, (NUMPIXELS / 3), (NUMPIXELS / 3));
        pixels.show();
        findCode();
        break;
      }
    }
  }
}



void redsplitmove() // here
{
  buttonshift = 15;
  bounceBeam A(2, true, pixels.Color(50, 0, 255), 0);
  bounceBeam B(2, false, pixels.Color(50, 0, 255), NUMPIXELS);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      pixels.clear();

      A.shift();
      B.shift();

      if (A.getPosition() == B.getPosition() || A.getPosition() == 0 || A.getPosition() == NUMPIXELS)
      {
        A.switchDirc();
        A.switchColor(pixels.Color(50, 0, 255), pixels.Color(0, 255, 50));
      }
      if (A.getPosition() == B.getPosition() || B.getPosition() == NUMPIXELS || B.getPosition() == 0)
      {
        B.switchDirc();
        B.switchColor(pixels.Color(50, 0, 255), pixels.Color(0, 255, 50));
      }

      pixels.show();
      findCode();
    }
  }
}


int positionA, positionB;

void shift(bool dirc) { // (true to right - false to left, beam length, index of loop)
  if (dirc) {
    positionA++;
    pixels.setPixelColor(positionA, pixels.Color(225, 0, 0));
  } else {
    positionB--;
    pixels.setPixelColor(positionB, pixels.Color(225, 0, 0));
  }
}



void bluesplitmove() // to be modified
{
  buttonshift = 16;
  bounceBeam A(2, true, pixels.Color(255, 0, 0), 0);
  bounceBeam B(2, false, pixels.Color(255, 0, 0), NUMPIXELS - 1);

  bounceBeam C(1, true, pixels.Color(0, 0, 255), 10);
  bounceBeam D(1, false, pixels.Color(0, 0, 255), NUMPIXELS - 10);

  bounceBeam E(1, true, pixels.Color(255, 255, 255), 7);

  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      pixels.clear();

      A.shift();
      B.shift();
      C.shift();
      D.shift();
      E.shift();

      if (A.getPosition() == B.getPosition() || A.getPosition() == 0 || A.getPosition() == NUMPIXELS)
      {
        A.switchDirc();
      }
      if (A.getPosition() == B.getPosition() || B.getPosition() == NUMPIXELS || B.getPosition() == 0)
      {
        B.switchDirc();
      }
      if (C.getPosition() == D.getPosition() || C.getPosition() == 0 || C.getPosition() == NUMPIXELS)
      {
        C.switchDirc();
      }
      if (C.getPosition() == D.getPosition() || D.getPosition() == NUMPIXELS || D.getPosition() == 0)
      {
        D.switchDirc();
      }
      if (E.getPosition() == 0 || E.getPosition() == NUMPIXELS)
      {
        E.switchDirc();
      }

      pixels.show();
      findCode();
    }
  }
}


void redyellowsplitmove()
{
  buttonshift = 17;
  bounceBeam A(2, true, serialColorInput, 0);
  bounceBeam B(2, false, serialColorInput, NUMPIXELS);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      pixels.clear();

      A.shift();
      B.shift();

      if (A.getPosition() == B.getPosition() || A.getPosition() == 0 || A.getPosition() == NUMPIXELS)
      {
        A.switchDirc();
      }
      if (A.getPosition() == B.getPosition() || B.getPosition() == NUMPIXELS || B.getPosition() == 0)
      {
        B.switchDirc();
      }

      pixels.show();
      findCode();
    }
  }
}


void whitejump() // to be modified to suit any strip length
{
  buttonshift = 9;
  bounceBeam A(5, true, serialColorInput, 0);
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      pixels.clear();
      int i = rand() % NUMPIXELS;
      A.shift(5);
      if (A.getPosition() == i || A.getPosition() == 0 || A.getPosition() == NUMPIXELS - A.getLength())
      {
        A.switchDirc();
      }
      pixels.show();
      findCode();
    }
  }
}


void redblueflash()
{
  buttonshift = 18;
  while (true) {
    pixels.clear();
    while (true)
    {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.fill(pixels.Color(255, 0, 0), 0, (NUMPIXELS / 6));
        pixels.fill(pixels.Color(255, 0, 0), (NUMPIXELS / 6) * 2, (NUMPIXELS / 6));
        pixels.fill(pixels.Color(255, 0, 0), (NUMPIXELS / 6) * 4, (NUMPIXELS / 6));
        findCode();
        pixels.show();
        break;
      }
    }

    pixels.clear();
    while (true) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.fill(pixels.Color(0, 0, 255), (NUMPIXELS / 6), (NUMPIXELS / 6));
        pixels.fill(pixels.Color(0, 0, 255), (NUMPIXELS / 6) * 3, (NUMPIXELS / 6));
        pixels.fill(pixels.Color(0, 0, 255), (NUMPIXELS / 6) * 5, (NUMPIXELS / 6));
        pixels.show();
        findCode();
        break;
      }
    }
  }
}


void chasing()
{
  buttonshift = 7;
  while (true) {


    for (int z = 0; z <= 200; z += 50)
    {
      for (int x = 0; x <= 200; x += 50)
      {
        for (int i = 0; i < NUMPIXELS; i++)
        {
          while (true) {
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
              previousMillis = currentMillis;
              pixels.setPixelColor(i, pixels.Color(255 - x, z, 0));
              pixels.show();
              findCode();
              break;
            }
          }
        }

        for (int i = 0; i < NUMPIXELS; i++)
        {
          while (true) {
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
              previousMillis = currentMillis;
              pixels.setPixelColor(i, pixels.Color(z, 0, 50 + x));
              pixels.show();
              findCode();
              break;
            }
          }
        }
      }

    }
  }
}


void beam() // adjust to mode button (beam v2 too)
{
  buttonshift = 8;
  while (true) {
    pixels.fill(serialColorInput, 0, NUMPIXELS);
    int i = 0;
    while (i < NUMPIXELS)
    {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.setPixelColor(i - 1, serialColorInput);
        for (int x = 0; x < (NUMPIXELS / 6); x++)
        {
          pixels.setPixelColor(i + x, 0x000000);
        }
        pixels.show();
        i++;
        findCode();
      }
    }
  }
}



void beamV2()
{
  buttonshift = 6;
  while (true) {
    int i = 0;
    while (i < NUMPIXELS + 1)
    {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.clear();
        pixels.setPixelColor(i, pixels.Color(10, 0, 50));
        pixels.setPixelColor(i + 1, pixels.Color(25, 0, 150));
        pixels.setPixelColor(i + 2, pixels.Color(50, 0, 255));
        pixels.setPixelColor(i + 3, pixels.Color(25, 0, 150));
        pixels.setPixelColor(i + 4, pixels.Color(10, 0, 50));
        pixels.show();
        i++;

        findCode();

        if (!(i >= (NUMPIXELS / 3) && i < ((NUMPIXELS / 3) * 2)))
          interval = 75;
        else
          interval = 150;

      }
    }

  }
}



void purpledot() {
  buttonshift = 5;
  bounceBeam A(1, true, pixels.Color(150, 0, 150), 0);
  int x = 0;
  while (true)
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      pixels.clear();
      A.shift();

      if (x > NUMPIXELS / 2 - 1)
      {
        x = 0;
      }

      if (A.getDirc()) {
        if (A.getPosition() >= NUMPIXELS - x) {
          x++;
          A.switchDirc();
        }
      } else {
        if (A.getPosition() <= 0 + x) {
          x++;
          A.switchDirc();
        }
      }

      if (A.getPosition() % 2 == 0)
      {
        A.setColor(pixels.Color(150, 0, 150));
      } else {
        A.setColor(pixels.Color(255, 150, 0));
      }

      pixels.show();
      findCode();
    }
  }
}


void blinkingstar() {
  buttonshift = 4;
  while (true) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      for (int x = 0; x < 2; x++) {
        int i = rand() % NUMPIXELS;
        pixels.setPixelColor(i, serialColorInput);
      }
      pixels.show();
      findCode();
    }
    pixels.clear();
  }
}



void splitpaintmove() // fix mid point
{
  buttonshift = 3;
  bounceBeam A(1, true, serialColorInput, 0);
  bounceBeam B(1, false, serialColorInput, NUMPIXELS);
  int x = 0;
  while (true) {
    if (x > 240) {
      x = 0;
    }
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      A.shift();
      B.shift();
      if (A.getPosition() == B.getPosition() || A.getPosition() < 0)
      {
        A.switchDirc();
        if (A.getDirc()) {
          A.setColor(pixels.Color(255, 0, x));
        } else {
          A.setColor(pixels.Color(0, 255, x));
        }
      }
      if (A.getPosition() == B.getPosition() || B.getPosition() == NUMPIXELS)
      {
        B.switchDirc();
        if (B.getDirc()) {
          B.setColor(pixels.Color(255, 0, x));
        } else {
          B.setColor(pixels.Color(0, 255, x));
        }
      }

      pixels.show();
      findCode();
    }
    x += 30;
  }
}


void Colorfade() { // here
  buttonshift = 1;
  float b = 1;
  R = 255;
  G = 0;
  B = 0;
  while (true)
  {
    pixels.clear();
    for (int h = 0; h <= 5; h++) {
      for (int k = 0; k < 255; k++) {
        modeloop = true;
        while (modeloop) {
          currentMillis = millis();
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;

            switch (h) {
              case 0:   ColorFadeFunction(0, 1, 0, true, 0, 255, 1);
                break;
              case 1:  ColorFadeFunction(1, 0, 0, false, 0, 255, 1);
                break;
              case 2:  ColorFadeFunction(0, 0, 1, true, 0, 255, 1);
                break;
              case 3:  ColorFadeFunction(0, 1, 0, false, 0, 255, 1);
                break;
              case 4:  ColorFadeFunction(1, 0, 0, true, 0, 255, 1);
                break;
              case 5:  ColorFadeFunction(0, 0, 1, false, 0, 255, 1);
                break;
            }

            for (int i = 0; i < NUMPIXELS; i++) {
              pixels.setPixelColor(i, pixels.Color(R, G, B));
            }
            pixels.show();
            findCode();
            modeloop = false;
          }
        }
      }
    }
  }
}



void Fadingblueneon() {
  R = 0;
  buttonshift = 2;
  float b = 1;

  while (true)
  {
    R = 0;
    G = 0;
    B = 0;
    pixels.clear();

    for (int k = 0; k < 255; k += 5) {
      modeloop = true;
      while (modeloop == true) {
        currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          ColorFadeFunction(0, 0, 1, true, 0, 255, 5);
          for (int i = 0; i < NUMPIXELS; i++)
          {
            pixels.setPixelColor(i, pixels.Color(R, G, B));
          }
          pixels.show();
          findCode();
          modeloop = false;
        }
      }
    }

    for (int k = 255; k > 0; k -= 5) {
      modeloop = true;
      while (modeloop == true) {
        currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          ColorFadeFunction(0, 0, 1, false, 0, 255, 5);
          for (int i = 0; i < NUMPIXELS; i++)
          {
            pixels.setPixelColor(i, pixels.Color(R, G, B));
          }
          pixels.show();
          findCode();
          modeloop = false;
        }
      }
    }

    if (b > 0.1)
      b -= 0.1;
    else
      b = 1;
  }
}



void splitmove() // to be modified
{
  while (true) {
    bounceBeam A(2, true, pixels.Color(255, 50, 0), 0);
    bounceBeam B(2, false, pixels.Color(255, 50, 0), NUMPIXELS / 2);
    bounceBeam C(2, true, pixels.Color(255, 50, 0), NUMPIXELS / 2);
    bounceBeam D(2, false, pixels.Color(255, 50, 0), NUMPIXELS);

    while (true)
    {
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        pixels.clear();
        A.shift();
        B.shift();
        C.shift();
        D.shift();

        if (A.getPosition() == 0 || A.getPosition() == NUMPIXELS / 2)
        {
          A.switchDirc();
        }
        if (B.getPosition() == NUMPIXELS / 2 || B.getPosition() == 0)
        {
          B.switchDirc();
        }
        if (C.getPosition() == NUMPIXELS / 2 || C.getPosition() == NUMPIXELS)
        {
          C.switchDirc();
        }
        if (D.getPosition() == NUMPIXELS / 2 || D.getPosition() == NUMPIXELS)
        {
          D.switchDirc();
        }

        pixels.show();
        findCode();
      }
    }
  }
}
