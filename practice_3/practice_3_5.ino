#include <Arduino.h>
#include <Keypad.h>

uint8_t red = 13;
uint8_t yellow = 12;
uint8_t green = 11;
uint8_t try_counter = 3;
uint8_t j = 0; // счетчик для элементов массива input
char code[] = {'1', '1', '1', '1'};
char input[] = {' ', ' ', ' ', ' '};
bool equal = true;

const uint8_t ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};
uint8_t rowPins[ROWS] = {7, 8, 9, 10};
uint8_t colPins[COLS] = {6, 5, 4, 3};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void time_delay(int b) {
    auto time_ = millis();
    while (millis() - time_ < b) {
        char key = keypad.getKey();
        if (key != NO_KEY) break;
    }
}

void check_input(char b = ' ') {
    equal = true;
    if (b == '#') {
        for (uint8_t i = 0; i < j; i++) {if (code[i] != input[i]) {equal = false;}}
        if (equal) {
            digitalWrite(green, HIGH);
            delay(3000);
            digitalWrite(green, LOW);
        } else {
            try_counter--;
            for (uint8_t i = 0; i < 3; i++) {
                digitalWrite(red, HIGH);
                delay(500);
                digitalWrite(red, LOW);
            }
            input[0] = ' '; input[1] = ' '; input[2] = ' '; input[3] = ' ';
            j = 0;
        }
    } else {
        for (uint8_t i = 0; i < 4; i++) {
            if (code[i] != input[i]) {
                equal = false;
            }
        }
        if (equal) {
            try_counter = 3;
            digitalWrite(green, HIGH);
            delay(3000);
            digitalWrite(green, LOW);
            input[0] = ' '; input[1] = ' '; input[2] = ' '; input[3] = ' ';
        } else {
            try_counter--;
            for (uint8_t i = 0; i < 3; i++) {
                digitalWrite(red, HIGH);
                delay(500);
                digitalWrite(red, LOW);
            }
            input[0] = ' '; input[1] = ' '; input[2] = ' '; input[3] = ' ';
        }
        j = 0;
    }
}

void setup() {
    pinMode(red, OUTPUT);
    pinMode(yellow, OUTPUT);
    pinMode(green, OUTPUT);
}

void loop() {
    if (try_counter == 0) {
        delay(10000);
        try_counter = 3;
    }
    
    char key = keypad.getKey();
    if (key != NO_KEY) {
        digitalWrite(yellow, HIGH);
        time_delay(500);
        digitalWrite(yellow, LOW);
        
        if (key == '*') { // * - сброс кода try_counter остается тем же
            input[0] = ' '; input[1] = ' '; input[2] = ' '; input[3] = ' ';
            j = 0;
        } else if (key == '#') { // досрочная проверка нажатых символов: если все верно, то программа продолжает свою работу, если нет - сбрасываются инпут и счетчик заполненных элементов, try_counter -1
            check_input('#');
        } else if (key != '*' && key != '#') {
            input[j] = key;
            j++;
        }
        if (j >= 4) {
            check_input();
        } 
    }
}