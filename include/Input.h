#ifndef INPUT_H
#define INPUT_H

#include <string>
using namespace std;

//Key codes for user input
enum Key {
    KEY_UNKNOWN,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_SPACE,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
    KEY_B,
    KEY_D,
	KEY_F,
    KEY_R,
    KEY_Q,
    KEY_S
};

int readKey(); //take in user input
void clearScreen(); //wipe all console text and start the game over

#endif
