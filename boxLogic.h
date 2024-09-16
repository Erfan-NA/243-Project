#ifndef BOXLOGIC_H
#define BOXLOGIC_H

#include "userOutput.h"
#include "magicNumbers.h"


void printBox(struct Box* box);
void removeBox(struct Box** boxPtr, int index);
void setBoxValues(struct Box* box, int direction);
void updatePosition(struct Box* box);
void addBox(struct Box** boxPtr, int direction);
int checkHitbox(struct Box* boxPtr);
void updateAllBoxes(struct Box* boxPtr);
void checkAllBoxes(struct Box* boxPtr);

// EXTERNAL VARIABLES WITH TESTING SHOWN IN BOX LOGIC C FILE
extern int numBoxes; //INITIALIZE AS 0
extern int directionFacing; //INITIALIZE AS 2
extern struct Box* boxPtr; //INITIALIZE AS NULL
extern int mrAndersonIndex; //HANDLED IN AUDIO




volatile int* pixel_ctrl_ptr = (int*)0xFF203020; //INITIALIZE WITH PIXEL_CONTROL_ADDRESS



extern int health;
extern bool hit_flag; //INITIALIZE AS FALSE
extern bool hit_reset; //INITIALIZE AS FALSE
extern bool block_flag; //INITIALIZE AS FALSE
extern bool block_reset; //INITIALIZE AS FALSE

extern int difficultySpeed; //INITIALIZED IN MENU


void printBox(struct Box* box) {
    printf("xPos: %d, yPos: %d, xDir: %d, yDir: %d\n", 
    box->xPos, box->yPos, box->xDir, box->yDir);
}

// Function to remove the box at index
void removeBox(struct Box** boxPtr, int index) {
    // Check if the index is valid
    if (index < 0 || index >= numBoxes) {
        printf("Invalid index\n");
        return;
    }

    // Shift the memory to the left to remove the box
    for (int i = index; i < numBoxes - 1; i++) {
        (*boxPtr)[i] = (*boxPtr)[i + 1];
    }

    // Reallocate memory to decrease the size of the box array
    *boxPtr = (struct Box*)realloc(*boxPtr, (numBoxes - 1) * sizeof(struct Box));

    // Decrement the number of boxes
    numBoxes--;
}

// Function to initialize the attributes of the newest box
void setBoxValues(struct Box* box, int direction) {
    // Set position and direction values based on the direction parameter
    if (direction == LEFT_DIRECTION) { //Laft
        box->xPos = 0;
        box->yPos = YHEIGHT/2;
        box->xDir = difficultySpeed;
        box->yDir = OFF;
        box->direction = LEFT_DIRECTION;
    }
    if (direction == RIGHT_DIRECTION) { //Right
        box->xPos = XWIDTH;
        box->yPos = YHEIGHT/2;
        box->xDir = -difficultySpeed;
        box->yDir = OFF;
        box->direction = RIGHT_DIRECTION;
    }
    if (direction == UP_DIRECTION) { //Up
        box->xPos = XWIDTH/2;
        box->yPos = 0;
        box->xDir = OFF;
        box->yDir = difficultySpeed;
        box->direction = UP_DIRECTION;
    }
    if (direction == DOWN_DIRECTION) { //Down
        box->xPos = XWIDTH/2;
        box->yPos = YHEIGHT;
        box->xDir = OFF;
        box->yDir = -difficultySpeed;
        box->direction = DOWN_DIRECTION;
    }
    // Add other conditions as needed
}

void updatePosition(struct Box* box) {
    box->xPos += box->xDir;
    box->yPos += box->yDir;
}

int checkHitbox(struct Box* boxPtr) {
    if (boxPtr->direction == 0 && 159 - boxPtr->xPos < 30) { //Left arrow hitboxes
        if (159 - boxPtr->xPos < 9) {
            return GOT_HIT;
        }
        else if (directionFacing == 0) {
            return BLOCKED_HIT;
        }
        else {
            return 0;
        }
    }

    if (boxPtr->direction == 1 && boxPtr->xPos - 159 < 30) { //Right arrow hitboxes
        if (boxPtr->xPos - 159 < 9) {
            return GOT_HIT;
        }
        else if (directionFacing == 1) {
            return BLOCKED_HIT;
        }
        else {
            return 0;
        }
    }

    if (boxPtr->direction == 2 && 119 - boxPtr->yPos < 30) { //Up arrow hitboxes
        if (119 - boxPtr->yPos < 9) {
            return GOT_HIT;
        }
        else if (directionFacing == 2) {
            return BLOCKED_HIT;
        }
        else {
            return 0;
        }
    }

    if (boxPtr->direction == 3 && boxPtr->yPos - 119 < 30) { //Down arrow hitboxes
        if (boxPtr->yPos - 119 < 9) {
            return GOT_HIT;
        }
        else if (directionFacing == 3) {
            return BLOCKED_HIT;
        }
        else {
            return 0;
        }
    }

    return 0;



}

// Function to add a box
void addBox(struct Box** boxPtr, int direction) {
    // Reallocate memory to increase the size of the box array
    *boxPtr = (struct Box*)realloc(*boxPtr, (numBoxes + 1) * sizeof(struct Box));

    // Check if memory reallocation was successful
    if (*boxPtr == NULL) {
        printf("Memory reallocation failed\n");
        return;
    }

    // Increment the number of boxes
    numBoxes++;

    // Call setBoxValues to initialize the attributes of the newest box
    setBoxValues(&(*boxPtr)[numBoxes - 1], direction);
}

void updateAllBoxes(struct Box* boxPtr) {
    for (int i = 0; i < numBoxes; i++) {
        updatePosition(&boxPtr[i]);
    }
}


void checkAllBoxes(struct Box* boxPtr) {
    for (int i = numBoxes - 1; i >= 0; i--) {
        for (int i = numBoxes - 1; i >= 0; i--) {
            int hit_type = checkHitbox(&boxPtr[i]);
            if (hit_type == GOT_HIT) {
                pixel_buffer_start = *(pixel_ctrl_ptr);
                eraseSmartArrow(&boxPtr[i]);
                pixel_buffer_start = *(pixel_ctrl_ptr + 1);
                removeBox(&boxPtr, i);
                hit_flag = true;
                hit_reset = true;
                mrAndersonIndex = 0;
                health--;
            }
            else if (hit_type == BLOCKED_HIT){
                pixel_buffer_start = *(pixel_ctrl_ptr);
                eraseSmartArrow(&boxPtr[i]);
                pixel_buffer_start = *(pixel_ctrl_ptr + 1);
                removeBox(&boxPtr, i);
                block_flag = true;
                block_reset = true;
            }
        }
    }
}

#endif