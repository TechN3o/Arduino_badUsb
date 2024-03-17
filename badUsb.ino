// https://github.com/TechN3o/Arduino_badUsb
/*
* ..you wont get the bitches either by doing this, but still its pretty cool shit to do
*
* I had also idea for nano33 iot.... to counter executing code, when i want to upload new one, I would just shorten 2 pins via jumper cable and that will disable the executing process and make space to upload 
* and also I get to choose which script I want to use 
*
*wiring diagram for Arduino Nano 33 IoT, where shortening two pins is used to tell which assigned opration(script) to use, but you can ignore this if you dont want to
*pins  12 10 8  6  4   GND    OUTPUT  \
*pins  11 9  7  5  3   2      INPUT   /
*      |  |  |  |  |   |
*  ----+--+--+--+--+---+---------------
*      |  |  |  |  |   |
*      1  2  3  4  5   6      <<options for shortening pins/choosing action (shortening between input and output)
*then pins.startPing()
*
*just keep in mind, that SHORTENING pins to do actions is absolutely OPTIONAL and you dont have to do it
*/


#include "Keyboard.h"
#include <Timer.h>
// #include <Scheduler.h>
Timer timer;
Timer lastTime;
//// shortcut

template<typename T, size_t N>
int arraySize(T (&array)[N]) {
  return sizeof(array) / sizeof(array[0]);
}



class tls {
public:
  bool isInRange(uint8_t input, uint8_t from, uint8_t to) {
    //if after constrain is the value same, then it fits and it is in range
    return (constrain(input, from, to) == input);
  }
  void comment(String input) {
    if (lastTime.read() > 2000) {
      Serial.println(">--------------------------<");
    }
    Serial.println(input);
    blink();
    lastTime.stop();
    lastTime.start();
  }
  void blink(uint8_t i = 2) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(i);
    digitalWrite(LED_BUILTIN, LOW);
  }
};
tls tls;
#define nullI 0    // null integer definition
#define nullS "0"  // null String definition
#define nullC '0'
class pins {
public:
  int8_t input[1] = { 11 };  //{For Nano33IoT: 11, 9, 7, 5, 3, 2 };   //KEEP AN EYE TO ALSO SET ARRAY SIZE WHEN CHANGING PINS              ...wonder how if i wanna change pins in mid of the code...
  int8_t output[1] = { 9 };
  //                                                                                 int8_t input[1] = { 11 };
  //                                                                                 ~~~~~~~~~~~~~^
  uint8_t pwmPing;
  uint32_t pulseTimeoutMs = 80;
  uint8_t readError = 4;
#define valuesSize 8  // dunno, values[] was giving errors about un-static size declaration, so just fixed it with #define
  uint8_t values[valuesSize] = {};

  void startPing(uint8_t in = sqrt(32 / 2)) {
    pwmPing = sq(in) * 2;
    for (int i; i < arraySize(input); i++) {
      pinMode(input[i], INPUT);
      tls.comment("[" + String(i) + "] pin[" + String(input[i]) + "] is set on INPUT");
    }
    for (int i; i < arraySize(output); i++) {
      pinMode(output[i], OUTPUT);
      analogWrite(output[i], in);
      tls.comment("[" + String(i) + "] pin[" + String(output[i]) + "] is set on OUTPUT with pwmPing[" + String(pwmPing) + "]");
    }
  }
  ////
  int read(uint8_t inputPin) {  //  because input reading for positive current is unstable and reading nothing with pullup resistor aint work, measuring pwm may work, but to get solid yes or no, we gotta scan multiple times to make average              //checking shortening combination of pins

    tls.comment("reading pin[" + String(inputPin) + "]");
    clearValues();
    for (int i = 0; i < valuesSize; i++) {  // pinging pin with pwm value
      onEnd(constrain(pulseIn(inputPin, HIGH, pulseTimeoutMs * 1000), 0, 255));
    }
    return decide();
  }
  void endPing() {
    for (int i; i < arraySize(input); i++) {
      pinMode(input[i], OUTPUT);
      digitalWrite(input[i], LOW);
      tls.comment("[" + String(i) + "] pinging on pin[" + String(input[i]) + "] ended");
    }
    for (int i; i < arraySize(output); i++) {
      pinMode(output[i], OUTPUT);
      digitalWrite(output[i], LOW);
      tls.comment("[" + String(i) + "] pinging from pin[" + String(output[i]) + "] ended");
    }
  }
private:
  int8_t valuesCounter = 0;
  bool decide() {
    uint8_t maxVal = pwmPing + abs(readError);
    uint8_t minVal = pwmPing + abs(readError) * -1;
    int8_t ones = 0;
    int8_t zeros = 0;  //inglisch weri gut

    for (int8_t i = 0; i < valuesSize; i++) {
      if (tls.isInRange(values[i], minVal, maxVal)) {
        Serial.println("[" + String(i) + "] - value [" + String(values[i]) + "] is in range readError[" + String(readError) + " +- from " + String(pwmPing) + "] ");
        ones++;
      } else {
        Serial.println("[" + String(i) + "] - value [" + String(values[i]) + "] is NOT in range readError[" + String(readError) + " +- from " + String(pwmPing) + "] ");
        zeros++;
      }
    }
    if (ones > zeros) {  // could just write return (ones > zeros);  but I used if statement also to get feedback info
      Serial.println("this pin is Enabled, ones[" + String(ones) + "] and zeros[" + String(zeros) + "]");
      return true;
    } else {
      Serial.println("this pin is Disabled, ones[" + String(ones) + "] and zeros[" + String(zeros) + "]");
      return false;
    }
  }
  void clearValues() {
    for (int i = 0; i < valuesSize; i++) {
      values[i] = 0;  // Nastaví každou hodnotu v poli na 0
    }
    valuesCounter = 0;
  }

  bool onEnd(int input) {  //inputing the number i wanna add
    if (valuesCounter < valuesSize) {
      values[valuesCounter] = input;
      valuesCounter++;
      tls.comment("Last Added [" + String(input) + "]");
      delay(100);

      return true;  //returning if can or cannot intercept another value
    } else {
      // Serial.println("\nToo much elements in array [" + String(valuesCounter) + "], heres the whole array:  ");
      // for (int i; i < valuesCounter; i++) {
      //   Serial.println(values[i]);
      // }
      // Serial.println("--Thats all \n");
      return false;
    }
  }
};
pins pins;

class kbd {  //just wrapping keyboard lib functions
public:
  void wr(uint8_t k) {  // WRITE
    Keyboard.write(k);
  }

  bool isPressed(uint8_t k) {
    //keeps logs from prs()
    //returns bool on
  }
  void prs(uint8_t k) {
    Keyboard.press(k);
  }
  void press(uint8_t k) {
    Keyboard.press(k);
  }
  void prt(String w) {
    Keyboard.print(w);
  }
  void print(String w) {
    Keyboard.print(w);
  }
  void prtln(String w) {
    Keyboard.println(w);
  }
  void println(String w) {
    Keyboard.println(w);
  }
  void rls(uint8_t k) {
    Keyboard.release(k);
  }
  void rlsA() {
    Keyboard.releaseAll();
  }
  void dl(uint16_t i) {
    delay(i);
  }

  void sht(uint8_t i1, uint8_t i2, uint8_t i3 = nullI, uint8_t i4 = nullI, uint8_t i5 = nullI) {  // SHORTCUT
    int counter = 0;

    tls.comment("i1 pressed[" + String(i1) + "]");
    if (i1 >= 128) {
      Keyboard.press(i1);  // is modifier
      pressed[counter] = i1;
      counter++;
    } else {
      Keyboard.write(i1);  // or else isnt
      pressPressed();
    }

    tls.comment("i2 pressed[" + String(i2) + "]");
    if (i2 >= 128) {
      Keyboard.press(i2);
      pressed[counter] = i2;
      counter++;
    } else {
      Keyboard.write(i2);
      pressPressed();
    }


    if (i3 != nullI) {
      tls.comment("i3 pressed[" + String(i3) + "]");
      if (i3 >= 128) {
        Keyboard.press(i3);
        pressed[counter] = i3;
        counter++;
      } else {
        Keyboard.write(i3);
        pressPressed();
      }
    }
    if (i4 != nullI) {
      tls.comment("i4 pressed[" + String(i4) + "]");
      if (i4 >= 128) {
        Keyboard.press(i4);
        pressed[counter] = i4;
        counter++;
      } else {
        Keyboard.write(i4);
        pressPressed();
      }
    }
    if (i5 != nullI) {
      tls.comment("i5 pressed[" + String(i5) + "]");
      if (i5 >= 128) {
        Keyboard.press(i5);
        pressed[counter] = i5;
        counter++;
      } else {
        Keyboard.write(i5);
        pressPressed();
      }
    }
    delay(100);
    rlsA();
    //add return input i1, i2...;
  }

private:
#define pressedSize 5
  int pressed[pressedSize] = {};  // 0-4

  void pressPressed() {
    for (int i = 0; i < pressedSize; i++) {
      Keyboard.press(pressed[i]);
    }
  }
};
kbd k;
void setup() {
  delay(4000);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  /////////////
  pins.startPing(sqrt(32 / 2));  // pusleIn() will then read 32  //sqrt(32 / 2) = sqrt(16) = 4  = analogWrite(4) //
  for (int i = 0; i < arraySize(pins.input); i++) {
    if (pins.read(pins.input[i])) {
      tls.comment("Ready for an upload");  // UPLOAD
      delay(3000);
      pins.endPing();
      Keyboard.end();
    } else {
      Keyboard.begin(KeyboardLayout_cz_CZ);
      tls.comment("lets hunt");
      pins.endPing();
      k.sht(KEY_LEFT_GUI, 'm');  //minimalize everything
      runScript(2);
      delay(1000);
      runScript(1);

      Keyboard.end();
    }
  }
  /////////////
}

void loop() {
  if (Serial.available() > 0) {
    runScript(4);
    Serial.read();
  }
}

void script_unArmed() {
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}
int runScript(uint8_t s) {
  switch (s) {
    case 1:
      script_1();
      break;
    case 2:
      script_2();
      break;
    case 3:
      script_3();
      break;
    case 4:
      script_4();
      break;
    case 5:
      script_5();
      break;
    case 6:
      script_6();
      break;
    case 7:
      script_7();
      break;
    case 0:
      script_unArmed();
    default:
      script_unArmed();
  }
}
void script_1() {
  k.sht(KEY_LEFT_GUI, 'r');  //mělo by být velké i to B, protože shift se drží v každém případě
  k.prtln("cmd");
  k.dl(600);
  k.sht(KEY_LEFT_GUI, KEY_UP_ARROW);
  k.dl(200);
  k.prtln("color a");
  k.prtln("ipconfig -all");
  k.prt("...pich pich...");
  k.sht(KEY_LEFT_CTRL, '-');
  k.sht(KEY_LEFT_CTRL, '-');
  k.sht(KEY_LEFT_CTRL, '-');
  k.dl(1000);
  k.rlsA();
  script_3();
}
void script_2() {
  k.sht(KEY_LEFT_GUI, 'r');  //mělo by být velké i to B, protože shift se drží v každém případě
  k.prtln("cmd");
  k.dl(600);
  k.sht(KEY_LEFT_GUI, KEY_UP_ARROW);
  k.dl(600);
  k.prtln("color 5");
  k.prtln("curl ascii.live/donut");
  k.sht(KEY_LEFT_CTRL, '+');
  k.sht(KEY_LEFT_CTRL, '+');
  k.sht(KEY_LEFT_CTRL, '+');
}
void script_3() {  // rickroll
  k.sht(KEY_LEFT_GUI, 'r');
  k.dl(1000);
  k.prtln("https://www.youtube.com/watch?v=mx86-rTclzA&ab_channel=AviatorCameron");
  k.dl(5000);
  k.wr('f');
}
void script_4() {  //minonky script
  uint32_t rand = random(32000);
  k.sht(KEY_LEFT_GUI, 'r');
  k.prtln("cmd");
  k.dl(1000);
  k.prtln("cd desktop");
  k.prtln("color a");
  k.prtln("type nul > hello" + String(rand) + ".txt");
  delay(500);
  delay(1000);
  k.sht(KEY_LEFT_CTRL, 's');
  delay(300);
  k.sht(KEY_LEFT_ALT, KEY_TAB);
  delay(150);
  k.prtln("type hello" + String(rand) + ".txt");
}
void script_5() {
}
void script_6() {
}
void script_7() {
}
void script_8() {
}
void script_9() {
}
void script_10() {
}
void script_11() {
}
void script_12() {
}
void script_13() {
}
void script_14() {
}
void script_15() {
}
void script_16() {
}
void script_17() {
}
void script_18() {
}
void script_19() {
}
void script_20() {
}
//....