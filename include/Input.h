#ifndef INPUT_H
#define INPUT_H

#include <string>
using namespace std;

enum Key {
    KEY_LEFT = 1000,
    KEY_RIGHT,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_UNKNOWN,
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
void moveCursor(int row, int col);

#endif
