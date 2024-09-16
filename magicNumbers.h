#ifndef MAJIC_NUMBERS_C
#define MAJIC_NUMBERS_C

#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

#define EMPTY 0
#define XWIDTH 320
#define YHEIGHT 240
#define ARROWBOXWIDTH 16
#define LEFT_ARROW 0x6B
#define RIGHT_ARROW 0x74
#define UP_ARROW 0x75
#define DOWN_ARROW 0x72
#define W_KEY 0x1D
#define A_KEY 0x1C
#define S_KEY 0x1B
#define D_KEY 0x23
#define SPACE_KEY 0x29

#define LEFT_DIRECTION 0
#define RIGHT_DIRECTION 1
#define UP_DIRECTION 2
#define DOWN_DIRECTION 3
#define SHIELD_SIZE 26

#define BLACK 0x0000

#define MENU_START 1
#define HIGH_HEALTH 4
#define LOW_HEALTH 2
#define SMALL_HEART_MEASURE 5
#define HEALTH_POSITION 200
#define SMALL_HEART_MOVEMENT 5
#define SMALL_HEART_CYCLE 7

#define MRANDERSON_LENGTH 21343

#define OFF 0
#define ON 1

#define COOLDOWN_TIMER_EASY 20000
#define COOLDOWN_TIMER_HARD 100
#define DIFFICULTY_SPEED_SLOW 2
#define DIFFICULTY_SPEED_FAST 5

#define ATTACKER 0
#define DEFENDER 1

#define GOT_HIT -2
#define BLOCKED_HIT -1

struct Box {
    int xPos;
    int yPos;
    int xDir;
    int yDir;
    int direction;
};



#endif