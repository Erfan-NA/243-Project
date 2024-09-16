#include "gameLogic.h"
#include "userOutput.h"




void reset();
void intializations();


int menuPosition;
int defenderInput;
int mrAndersonIndex = OFF;
char key = OFF;
int coolDown_time = COOLDOWN_TIMER_EASY;
int menuPosition = MENU_START;
int directionFacing = UP_DIRECTION;
int numBoxes = 0;
struct Box* boxPtr = NULL;
double globalTime = 0;
int difficultySpeed = DIFFICULTY_SPEED_SLOW;
bool hit_flag = false;
bool hit_reset = false;
bool block_flag = false;
bool block_reset = false;
int health = HIGH_HEALTH;
short int* backGround;

int main() {

    intializations();

    //WHERE LEGENDS COOK
    while (1) {
        

        while (1) { // MENU LOOP

            if (menuPosition == MENU_START) {
                backGround = easyBG;
                difficultySpeed = DIFFICULTY_SPEED_SLOW;
                coolDown_time = COOLDOWN_TIMER_EASY;
                health = HIGH_HEALTH;
            }
            else {
                backGround = hardBG;
                difficultySpeed = DIFFICULTY_SPEED_FAST;
                coolDown_time = COOLDOWN_TIMER_HARD;
                health = LOW_HEALTH;
            }

            // WAIT FOR V_SYNC
            draw_background();

            defenderInput = checkUserInput();
            if (defenderInput >> 8 != OFF && defenderInput >> 16 == DEFENDER) {
                if (defenderInput && 0xFF == UP_ARROW || defenderInput & 0xFF == DOWN_ARROW) {
                    menuPosition = -menuPosition;
                }
                else if (defenderInput && 0xFF == SPACE_KEY) {
                    break;
                }
            }

        }



        while (1) { // GAME LOOP


            GameLogic();
            if (health == 0) {
                break;
            }

        }

        // WAIT FOR VSYNC


        reset();
    }


    return 0;
}


void reset() {
    free(boxPtr);
    intializations;
}

void intializations() {
    mrAndersonIndex = OFF;
    key = OFF;
    coolDown_time = COOLDOWN_TIMER_EASY;
    menuPosition = MENU_START;
    directionFacing = UP_DIRECTION;
    numBoxes = 0;
    boxPtr = NULL;
    globalTime = 0;
    difficultySpeed = DIFFICULTY_SPEED_SLOW;
    hit_flag = false;
    hit_reset = false;
    block_flag = false;
    block_reset = false;
    health = HIGH_HEALTH;
}