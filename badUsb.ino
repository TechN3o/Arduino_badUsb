/* goals:
*      make classes and executing code easy, with no addition to write executed code from zero and specific to one task, but rather from some base - classes, pointers, functions - in a short keyword: variable
*      to do:
*        shortcuts (alt+f, ctrl+f, win+f...) >>realisticly>> shortcut(CTRL, SHIFT, N); outputs ctrl+shift+letterN
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
*then pins.ioInit()
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
  int arrayLength = sizeof(array) / sizeof(array[0]);
  return arrayLength;
}
void comment(String input) {
  if (lastTime.read() > 2000) {
    Serial.println("--------------------------");
  }
  Serial.println(input);
  lastTime.stop();
  lastTime.start();
}

const int nullI = 0;       // null integer definition
const String nullS = "0";  // null String definition



class pins {
public:
  bool armed;
  int input[1] = { 11 };                      //{ 11, 9, 7, 5, 3, 2 };  // + PIN 2 sensing GND
  int output[1] = { 10 };                     //{ 12, 10, 8, 6, 4 };
  void ioInit(const bool out_state = true) {  //out_state sets logical value for output shortening pins. Keep this on true,
                                              //  but can be switched to false to make negative current from out pins
    for (int i; i < arraySize(input); i++) {
      pinMode(input[i], INPUT_PULLUP);
    }
    for (int i; i < arraySize(output); i++) {
      pinMode(i, OUTPUT);
      comment("Init. output pins " + String(i));
      digitalWrite(output[i], out_state);
    }
  }
  ////
  void checkCombination() {  // selecting the script based on return from pins.read()
    // switch (pins.read()) {
    //  case 1      //// assign script ordinal number, like script_1() is as no. 1
    //   break;
  }

  ////
  int read(bool read_state = true) {  //checking shortening combination of pins 
    int found;
    for (int i; i != arraySize(input); i++) {
      if (digitalRead(input[i]) == read_state) {
        found = i;
        comment("i found it.. i is this: " + String(i) + "..the state of pin is " + String(digitalRead(i)));
        break;
        comment("Found read and end..");
      } else {
        found = 999;
        comment("Didnt found anything, keep going");
      }
      comment("welp, it maybe skipped if i can see this message...");
    }
    return input[found];  //combination number that is connected
  }
  bool isArmed() {
    return read() != 0;
  }
};
pins pins;

class prg {
public:
};
prg prg;

class let {
public:
  //make array of integer HEX referencing letters on keyboard - 7 bits = around 128 progmem extern in program flash or smth like that
  int l(char input) {  //return HEX numero after letter input
    // for(int i; ){

    // }
    return 0;
  }

  void init() {
  }
};
class kbd {  //just wrapping keyboard lib functions
public:

  String write(String input) {
  }

  int shortcut(int i1, int i2, int i3 = nullI, int i4 = nullI, int i5 = nullI) {
    Keyboard.press(i1);
    Keyboard.press(i2);
    if (i3 != nullI) {
      Keyboard.press(i3);
    }
    if (i4 != nullI) {
      Keyboard.press(i4);
    }
    if (i5 != nullI) {
      Keyboard.press(i5);
    }
    delay(100);
    Keyboard.releaseAll();
    //add return input i1, i2...;
  }
  // String shortcut(String i1, String i2, String i3 = nullS, String i4 = nullS, String i5 = nullS) { //if on L80 will be array of integers, String shortcut wont be used, bcs integer will
};

// void fuse() {
//   for (int i; i < arraySize(pins.input); i++) {
//     pinMode(i, INPUT_PULLUP);
//   }
//   for (int i; i < arraySize(pins.output); i++) {
//     pinMode(i, OUTPUT);
//     comment("Init. output pins" + String(i));
//     comment("fuse: digitalWrite")
//     digitalWrite(i, true);
//   }
// }

void setup() {
  Serial.begin(9600);
  pins.ioInit();

  // digitalWrite(10, true);
  /////////////
}

void loop() {
  if (Serial.available() > 0) {
    pins.ioInit();
    Serial.println("found on: " + String(pins.read()));
    Serial.read();
    comment(String(digitalRead(11)) + " and output is " + String(digitalRead(10)));
  }
}

void script_unArmed() {
  if (pins.isArmed()) {



  } else {
  }
}
void script_1() {
}
void script_2() {
}
void script_3() {
}
void script_4() {
}
void script_5() {
}
void script_6() {
}
