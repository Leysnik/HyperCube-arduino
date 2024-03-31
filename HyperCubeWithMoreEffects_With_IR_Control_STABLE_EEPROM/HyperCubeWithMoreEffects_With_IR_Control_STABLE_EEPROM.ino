//================ОБЬЯВЛЕНИЕ БИЛБИОТЕК====================
#include <FastLED.h>

#include <IRremote.h>

#include <EEPROM.h>
 // ==========НАСТРОЙКА КУБА=============
#define EdgeLeds 6
#define EdgeCount 24
#define AmpMax 1800
#define LedDI 8
#define IRPin 13
#define LedType WS2812

//================ОБЬЯВЛЕНИЕ ПЕРЕМЕННЫХ=====================
const int NumLeds = EdgeLeds * EdgeCount;
CRGB leds[NumLeds];
IRrecv irrecv(IRPin);
decode_results results;
byte Bright = EEPROM.read(6);
bool Power = EEPROM.read(5);
bool DrawMode = EEPROM.read(3);
bool MaxPoint = false;
byte Mode = EEPROM.read(0);
byte MaxMode = 5;
byte eMode = EEPROM.read(1);
byte Spd = EEPROM.read(2);
long unsigned int c;
byte counter;

// =========ВВОД ДАННЫХ===========
void Input() {
  while (!irrecv.isIdle()); // ожидаем возможности считать значение
  if (irrecv.decode( & results)) //ПРОВЕРКА НА НАЛИЧИЕ ЗНАЧЕНИЙ
  {
    irrecv.resume(); //ПРИЕМ СЛЕДУЮЩЕГО НАБОРА КОМАНД
    c = results.value;
    Serial.println(c);
    switch (c) {
    case 33456255: // off/on
      if (Power) {
        Power = !Power;
      } else {
        Power = !Power;
      }
      EEPROM.put(5, Power);
      break;

    case 33441975: // bright+
      if (Bright < 244 and!Power) {
        Bright += 10;

      }
      EEPROM.put(6, Bright);
      break;

    case 33454215: // bright-
      if (Bright > 1 and!Power) {
        Bright -= 10;

      }
      EEPROM.put(6, Bright);
      break;
    case 33448095: // ChangeMode
      if (!DrawMode) {
        Mode--;
      }
      if (!Power) {
        DrawMode = true;
        Mode++;
        counter = 0;
      }

      if (Mode > MaxMode) {
        Mode = 1;
      }
      delay(15);
      EEPROM.put(0, Mode);
      EEPROM.put(3, DrawMode);
      break;
    case 33472575: // speed-
      if (Spd > 1 and !Power) {
        Spd--;
      }
      EEPROM.put(2, Spd);
      break;
    case 33439935: // speed+
      if (Spd < 30 and !Power) {
        Spd++;
      }
      EEPROM.put(2, Spd);
      break;
    case 33424125: // snow
      if (!Power) {
        DrawMode = false;
        eMode = 1;
      }
      EEPROM.put(1, eMode);
      EEPROM.put(3, DrawMode);
      break;
    case 33428205: // sun
      if (!Power) {
        DrawMode = false;
        eMode = 2;
      }
      EEPROM.put(1, eMode);
      EEPROM.put(3, DrawMode);
      break;
    case 33446055: // mixed
      if (!Power) {
        DrawMode = false;
        eMode = 3;
        counter = 0;
      }
      EEPROM.put(1, eMode);
      EEPROM.put(3, DrawMode);
      break;
    case 33464415: // moon
      if (!Power) {
        DrawMode = false;
        FastLED.clear();
        eMode = 4;
      }
      EEPROM.put(1, eMode);
      EEPROM.put(3, DrawMode);
      break;
    case 33427695: // timer_off
      if (!Power) {
        DrawMode = false;
        eMode = 5;
      }
      EEPROM.put(1, eMode);
      EEPROM.put(3, DrawMode);
      break;
    }
  }
}

void delayBreak(int a) {
  delay(a / Spd);
}

void Draw() {
  if (!Power) {
    if (DrawMode) {
      switch (Mode) {
      case 1:
        Rainbow();
        break;
      case 2:
        CorruptedRainbow();
        break;
      case 3:
        ChaosMod();
        break;
      case 4:
        StepRainbow();
        break;
      case 5:
        Christmas();
        break;
      }

    } else {
      switch (eMode) {
      case 1:
        Snow();
        break;
      case 2:
        Sun();
        break;
      case 3:
        Mixed();
        break;
      case 4:
        Moon();
        break;
      case 5:

        break;
      }
    }
  } else {
    Off();
  }
}
//========ЭФФЕКТЫ==========
void Off() {
  for (int i = 0; i < NumLeds; i++) {
    Input();
    if (!Power) {
      break;
    }
    leds[i] = CHSV(0, 0, 0);
    FastLED.show();
    delayBreak(300);

  }
}
void Christmas() {
  if (MaxPoint == false) {
    if (counter == Bright) {
      MaxPoint = true;
    }

    for (int i = 0; i < NumLeds; i++) {
      Input();

      leds[i] = CHSV(1, 240, Bright - counter);
      if (i % 2 == 0) {
        leds[i] = CHSV(1, 0, Bright - counter);
      }
      if (i % 3 == 0) {
        leds[i] = CHSV(240, 240, Bright - counter);
      }

    }
    FastLED.show();
    delayBreak(200);
    counter++;
    if (counter > Bright) {
      counter = Bright;
    }
  }
  if (MaxPoint == true) {
    if (counter == 0) {
      MaxPoint = false;
    }
    for (int i = 0; i < NumLeds; i++) {
      Input();

      leds[i] = CHSV(0, 240, Bright - counter);
      if (i % 2 == 0) {
        leds[i] = CHSV(0, 0, Bright - counter);
      }
      if (i % 3 == 0) {
        leds[i] = CHSV(240, 240, Bright - counter);
      }

    }
    FastLED.show();
    delayBreak(200);
    counter--;
  }

}
void Mixed() {
  for (int i = 0; i < NumLeds / 3; i++) {
    Input();
    if (eMode != 3 or DrawMode) {
      break;
    }
    leds[i] = CHSV(3 * counter + 10, 250, Bright);
    FastLED.show();
    delayBreak(600);
  }
  for (int i = NumLeds / 3; i < 2 * NumLeds / 3; i++) {
    Input();
    if (eMode != 3 or DrawMode) {
      break;
    }
    leds[i] = CHSV(3 * counter + 90, 250, Bright);
    FastLED.show();
    delayBreak(600);
  }
  for (int i = 2 * NumLeds / 3; i < NumLeds; i++) {
    Input();
    if (eMode != 3 or DrawMode) {
      break;
    }
    leds[i] = CHSV(3 * counter + 130, 250, Bright);
    FastLED.show();
    delayBreak(600);
  }
  counter++;
}
void Moon() {
  for (int i = 0; i < NumLeds; i++) {
    Input();
    if (eMode != 4 or DrawMode) {
      break;
    }
    if (i % 2 == 0) {
      leds[i] = CHSV(2 * i, 250, Bright);
      FastLED.show();
      delayBreak(3000);
    }
  }
  for (int i = 0; i < NumLeds; i++) {
    Input();
    if (eMode != 4 or DrawMode) {
      break;
    }
    if (i % 2 == 0) {
      leds[i] = CHSV(0, 0, 0);
      FastLED.show();
      delayBreak(3000);
    }
  }
}
void Sun() {
  for (int i = 0; i < NumLeds; i++) {

    leds[i] = CHSV(35, 250, Bright);
  }
  FastLED.show();
  delayBreak(1000);
}
void Snow() {
  for (int i = 0; i < NumLeds; i++) {

    leds[i] = CHSV(0, 0, Bright);
  }
  FastLED.show();
  delayBreak(1000);
}
void StepRainbow() {
  for (int i = 0; i < NumLeds; i++) {
    Input();
    if (Mode != 4 or!DrawMode) {
      break;
    }
    leds[i] = CHSV(i * 2, 255, Bright);
    FastLED.show();
    delayBreak(600);
  }
}
void Rainbow() {
  for (int i = 0; i < NumLeds; i++) {
    leds[i] = CHSV(counter + i * 2, 255, Bright);
  }
  counter++;
  FastLED.show();
  delayBreak(1000);
}
void CorruptedRainbow() { //неожиданно, данный режим описывает бегающую змейку
  for (int i = 0; i < NumLeds; i++) {

    leds[i] = CHSV(counter + i * 2, int(255 / abs(counter - i)), int(Bright / abs(i - counter)));

  }
  counter++;
  if (counter > 190) {
    counter = 0;
  }
  FastLED.show();
  delayBreak(600);
}
void GoldMod() {
  for (int i = 0; i < NumLeds; i++) {

    leds[i] = CHSV(90, 255, Bright);
    if (i % EdgeLeds) {
      leds[i] = CHSV(60, 255, Bright);
    }
  }
  counter++;
  FastLED.show();
  delayBreak(1000);

}

void ChaosMod() {
  FastLED.clear();
  FastLED.show();
  delayBreak(1500);
  for (int i = 0; i < NumLeds / 2; i++) {
    Input();
    if (Mode != 3 or!DrawMode or Power) {
      break;
    }
    leds[random(NumLeds)] = CHSV(random(360), random(255), random(Bright));
    FastLED.show();
    delayBreak(2000);
  }

  for (int i = 0; i < NumLeds; i++) {
    Input();
    if (Mode != 3 or!DrawMode) {
      break;
    }
    leds[i] = CHSV(0, 0, 0);
    FastLED.show();
    delayBreak(600);
  }
}

//==========ОСНОВНЫЕ ФУНКЦИИ===============
void setup() {
  if (Mode > MaxMode) {
    Mode = 1;
  }
  if (eMode > 5) eMode = 1;
  if (Spd > 30) Spd = 20;
  if (Bright % 10 != 0) Bright = Bright + Bright % 10;
  irrecv.enableIRIn();
  pinMode(IRPin, INPUT);
  Serial.begin(9600);
  FastLED.addLeds < LedType, LedDI, GRB > (leds, NumLeds).setCorrection(TypicalLEDStrip);
  if (AmpMax > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, AmpMax);
  FastLED.setBrightness(Bright);
  FastLED.clear();
}

void loop() {
  Input();
  Draw();
}
