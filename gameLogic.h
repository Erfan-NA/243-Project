#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "boxLogic.h"
#include "userInput.h"

//COOLDOWN SHOULD BE INITIALIZED AS NON ZERO
extern int coolDown_time; // INITIALIZE AS COOLDOWN_TIMER
void GameLogic() {

    // INPUT FORMATTED AS: BYTE1, BYTE2, BYTE3
    // BYTE1 is either 0 or 1 to indicate if player 1 uses WASD or player 2 uses arrow keys
    // BYTE2 indicated if the key is held down at that instant
    // BYTE3 is the key value
    // Example 00016B: player 2 is using arrow keys, currently held down arrow key, and the key is left (6B)
    int inputs = checkUserInput();
    int direction;
    int coolDown;
    if (coolDown > 0) {
        coolDown--;
    }
    direction = inputs & 0xFF;
    if (inputs >> 8 != OFF && !coolDown) {
        coolDown = coolDown_time;
        if (inputs >> 16 == ATTACKER) {
            if (direction == A_KEY)
                addBox(&boxPtr, LEFT_DIRECTION);
            if (direction == D_KEY)
                addBox(&boxPtr, RIGHT_DIRECTION);
            if (direction == W_KEY)
                addBox(&boxPtr, UP_DIRECTION);
            if (direction == S_KEY)
                addBox(&boxPtr, DOWN_DIRECTION);
        }
    }
    else if (inputs >> 8 != OFF && inputs >> 16 == DEFENDER) {
        if (direction == LEFT_ARROW)
            directionFacing = LEFT_DIRECTION;
        if (direction == RIGHT_ARROW)
            directionFacing = RIGHT_DIRECTION;
        if (direction == UP_ARROW)
            directionFacing = UP_DIRECTION;
        if (direction == DOWN_ARROW)
            directionFacing = DOWN_DIRECTION;
    }
    updateAllBoxes(boxPtr);
    checkAllBoxes(boxPtr);
  
}

#endif