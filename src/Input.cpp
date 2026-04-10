#include "Input.h"
#include <iostream>
#include <conio.h>
using namespace std;

int readKey() {
    int c = _getch();

    if (c == '\r' || c == '\n') return KEY_ENTER;
    if (c == 27) return KEY_ESCAPE;
    if (c == '1') return KEY_1;
    if (c == '2') return KEY_2;
    if (c == '3') return KEY_3;
    if (c == '4') return KEY_4;
    if (c == 'd' || c == 'D') return KEY_D;
    if (c == 'r' || c == 'R') return KEY_R;
    if (c == 'q' || c == 'Q') return KEY_Q;

    // arrow keys on windows 
    if (c == 0 || c == 224) {
        int ext = _getch();
        if (ext == 75) return KEY_LEFT;
        if (ext == 77) return KEY_RIGHT;
        return KEY_UNKNOWN;
    }

    return KEY_UNKNOWN;
}

void clearScreen() {
    system("cls");
}

void moveCursor(int row, int col) {
    cout << "\033[" << row << ";" << col << "H" << flush;
}
