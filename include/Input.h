#ifndef INPUT_H
#define INPUT_H

#include <string>
using namespace std;

//Key codes
enum Key {
    KEY_UNKNOWN,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_D,
    KEY_R,
    KEY_Q
};

int readKey();
void clearScreen();

#endif
