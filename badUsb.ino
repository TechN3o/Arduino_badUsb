/* goals:
      make classes and executing code easy, with no addition to write executed code from zero and specific to one task, but rather from base - classes, pointers, functions - in a short keyword: variable
      to do:
        shortcuts (alt+f, ctrl+f, win+f...) >> shortcut(CTRL, SHIFT, N); outputs ctrl+shift+letterN

wiring diagram for Arduino Nano 33 IoT, where shortening two pins is used to tell which assigned opration(script) to use, but you can ignore this if you 
pins  12 10 8  6  4   GND    OUTPUT  \
pins  11 9  7  5  3   2      INPUT   /
      |  |  |  |  |   |
  ----+--+--+--+--+---+---------------
      |  |  |  |  |   |
      1  2  3  4  5   6      <<options for shortening pins/choosing action (shortening between input and output)
then pins.ioInit()

*/

#include <Keyboard.h>
// #include <Scheduler.h>
Keyboard_ keyboard;
//// shortcut

template<typename T, size_t N>
int arraySize(T (&array)[N]) {
  int arrayLength = sizeof(array) / sizeof(array[0]);
  return arrayLength;
}

const int nullI = 0;       // null integer def
const String nullS = "0";  // null String def



class pins {
public:
  bool armed;
  int input[6] = { 11, 9, 7, 5, 3, 2 };  // + PIN 2 sensing GND
  int output[5] = { 12, 10, 8, 6, 4 };
  void ioInit(bool out_state = true) {  //out_state sets logical value for output shortening pins. Keep this on true,
                                        //  but can be switched to false to make negative current from out pins
    for (int i; i != arraySize(input); i++) {
      pinMode(i, INPUT);
    }
    for (int i; i != arraySize(output); i++) {
      pinMode(i, INPUT);
      digitalWrite(i, out_state);
    }
  }
  ////
  void checkCombination() {  // selecting the script based on return from pins.read()
    // switch (pins.read()) {
    //  case 1      //// assign script ordinal number, like script_1() is as no. 1
    //   break;
  }

  ////
  int read() {  //checking shortening combination of pins
    int found;
    for (int i; i != arraySize(input); i++) {
      if ((digitalRead(input[i]) == HIGH) || (digitalRead(input[5]) == LOW)) {
        found = i;
        break;
      } else {
        found = 0;
      }
    }
    return found;  //combination number that is connected
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
  int letters[128]; //make array of integer HEX referencing letters on keyboard
  void init() {
  }
};
class kbd {  //just wrapping keyboard lib functions
public:

  String write(String input) {
  }

  int shortcut(int i1, int i2, int i3 = nullI, int i4 = nullI, int i5 = nullI) {
    keyboard.press(i1);
    keyboard.press(i2);
    if (i3 != nullI) {
      keyboard.press(i3);
    }
    if (i4 != nullI) {
      keyboard.press(i4);
    }
    if (i5 != nullI) {
      keyboard.press(i5);
    }
    delay(100);
    keyboard.releaseAll();
  }
  // String shortcut(String i1, String i2, String i3 = nullS, String i4 = nullS, String i5 = nullS) { //if on L80 will be array of integers, String shortcut wont be used, bcs integer will 
  // }
};


void setup() {
  Serial.begin(9600);
  Keyboard.begin();




  /////////////
}

void loop() {
  Serial.println("found combination on pin: " + String(pins.read()));
  delay(2000);
}
void script_unArmed() {
  if (pins.isArmed()) {



  } else {
  }
}
void script_1() {
  // demo
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
