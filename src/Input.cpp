#include "Input.h"
#include <iostream>

//determine which operating system we are compiling on
#ifdef _WIN32 //windows
    #include <conio.h>
    #include <cstdlib>
#else //Max/Linux
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace std;

#ifndef _WIN32 //mimic -getch() for Mac and Linux
// Mac/Linux replacement for _getch()
int getch() {
    termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt); //get current terminal settings
    newt = oldt; //copy settings to modify
    newt.c_lflag &= ~(ICANON | ECHO); //disable buffering and stop characters from printing
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); //apply modified settings

    ch = getchar(); //read in input as a character

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //restore original terminal settings
    return ch;
}//end of getch()
#endif

int readKey() { //read in input
#ifdef _WIN32 //windows using getch
    int c = _getch();

    //basic keys
    if (c == '\r') return KEY_ENTER;
    if (c == 27) return KEY_ESCAPE;

    //number keys
    if (c == '1') return KEY_1;
    if (c == '2') return KEY_2;
    if (c == '3') return KEY_3;
    if (c == '4') return KEY_4;
    //letter keys with both cases
    if (c == 'd' || c == 'D') return KEY_D;
    if (c == 'r' || c == 'R') return KEY_R;
    if (c == 'q' || c == 'Q') return KEY_Q;

    if (c == 0 || c == 224) {
        int ext = _getch();
        if (ext == 75) return KEY_LEFT;
        if (ext == 77) return KEY_RIGHT;
        if (ext == 72) return KEY_UP;
        if (ext == 80) return KEY_DOWN;
    }

#else //Mac/Linux version
    int c = getch();

    if (c == '\n') return KEY_ENTER;

    if (c == 27) { //ESC or arrow keys
        int c1 = getch();
        if (c1 == 91) {
            int c2 = getch();
            switch (c2) {
                case 65: return KEY_UP;
                case 66: return KEY_DOWN;
                case 67: return KEY_RIGHT;
                case 68: return KEY_LEFT;
            }
        }
        return KEY_ESCAPE; //if not arrow, treat as escape
    }

    if (c == '1') return KEY_1;
    if (c == '2') return KEY_2;
    if (c == '3') return KEY_3;
    if (c == '4') return KEY_4;
    if (c == 'd' || c == 'D') return KEY_D;
    if (c == 'r' || c == 'R') return KEY_R;
    if (c == 'q' || c == 'Q') return KEY_Q;

#endif

    return KEY_UNKNOWN; //return unknown if nothing matches
}//end of readKey

//clear console
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
} //end of clearScreen
