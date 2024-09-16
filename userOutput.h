#ifndef USER_OUTPUT_C
#define USER_OUTPUT_C


#include "audioSpriteLists.h"
#include "magicNumbers.h"

extern double globalTime; //INITIALIZE AS 0
extern int numBoxes; //INITIALIZE AS 0
extern short int *backGround; //GETS SET DEPENDING ON MENU / GAME MODE
extern int mrAndersonIndex; //HANDLED IN AUDIO

volatile int* LEDaddress = 0xFF200040;
volatile int* AudioBase = 0xFF203040;
extern bool hit_flag;
extern int health;




// FIGURE OUT PIXEL BUFFER VISUALS LATER
volatile int pixel_buffer_start; // VALUE DEPENDANT ON VSYNC BEHAVIOR




bool inBounds(int x, int y);
void eraseSmartArrow(struct Box* a);
void eraseAllArrows(struct Box* boxes);
void drawSmartArrow(struct Box* a);
void drawAllArrows(struct Box* boxes);
void writeToAudio(int audioSignal);
int getSWValue();
int decodeHex(int value);
void setSevenSegment(volatile long* segAddress, int value);
void draw_line(int x0, int y0, int x1, int y1, short int line_color);
void plot_pixel(int x, int y, short int line_color);
void draw_background();
void swap(int* a, int* b);
int abs(int a);
void drawMiniHeart(int x, int y);
int fastCos(int angle);
int fastSin(int angle);
int rotateX(int x, int y, int angle);
int rotateY(int x, int y, int angle);
void drawShield(double direction);
void wait_for_vsync();
void writeToAudio(int value);
int getSound(const int array[], int* index, int size, bool* soundcondition);
void drawHealth(int offset);
void drawMiniHeart(int x, int y) ;


//ADD VSYNC FUNCTIONS LATER

bool inBounds(int x, int y) {
    return (x < XWIDTH) && (x >= 0) && (y < YHEIGHT) && (y >= 0);
}


int fastCos(int angle) {
    if (angle == 0) {
        return 1;
    }
    else if (angle == 90 || angle == 270) {
        return 0;
    }
    else if (angle == 180) {
        return -1;
    }

    return 1;
}

int fastSin(int angle) {
    if (angle == 90) {
        return 1;
    }
    else if (angle == 270) {
        return -1;
    }
    else if (angle == 0 || angle == 180) {
        return 0;
    }

    return 0;
}


int rotateX(int x, int y, int angle) {
    //return x;
    return (int)(x * fastCos(angle)) - (y * fastSin(angle));
}

int rotateY(int x, int y, int angle) {
    //return y;
    return (int)(x * fastSin(angle)) + (y * fastCos(angle));
}

void drawShield(double direction) {
    double angle = 0;
    if (direction == LEFT_DIRECTION) {
        angle = 0;
    }
    else if (direction == RIGHT_DIRECTION) {
        angle = 180;
    }
    else if (direction == UP_DIRECTION) {
        angle = 90;
    }
    else if (direction == DOWN_DIRECTION) {
        angle = 270;
    }

    static int centerX = XWIDTH / 2;
    static int centerY = YHEIGHT / 2;
    for (int x = 0; x < SHIELD_SIZE; x++) {
        for (int y = 0; y < SHIELD_SIZE; y++) {
            plot_pixel(rotateX(x - (SHIELD_SIZE / 2), y - (SHIELD_SIZE / 2), angle) + centerX,
                rotateY(x - (SHIELD_SIZE / 2), y - (SHIELD_SIZE / 2), angle) + centerY,
                shieldSprite[x + (SHIELD_SIZE * y)]);
        }
    }
}


void eraseSmartArrow(struct Box* a) {
    for (int x = 0 - abs(a->xDir); x < ARROWBOXWIDTH - abs(a->xDir); x++) {
        for (int y = 0 - abs(a->yDir); y < ARROWBOXWIDTH + abs(a->yDir); y++) {
            if (inBounds(a->xPos + (x - (ARROWBOXWIDTH / 2)) * (a->xDir / abs(a->xDir)),
                a->yPos + (y - (ARROWBOXWIDTH / 2) * (a->xDir / abs(a->xDir))))) {
                //if (arrowSprite[x + (y * ARROWBOXWIDTH)] != 0x0000) {
                plot_pixel(a->xPos + (x - (ARROWBOXWIDTH / 2)) * (a->xDir / abs(a->xDir)),
                    a->yPos + (y - (ARROWBOXWIDTH / 2)) * (a->yDir / abs(a->yDir)),
                    0x0000);
                //}
            }
        }
    }
}


void eraseAllArrows(struct Box* boxes) {
    if (numBoxes <= 0) return;
    for (int i = 0; i < numBoxes; i++) {
        //if ((&boxes[i])->direction < 2) {
        eraseSmartArrow(&boxes[i]);
        //}

    }
}


void drawSmartArrow(struct Box* a) { // draw the arrows but put a little thought into it
    double angle = 0;

    int centerX = a->xPos;
    int centerY = a->yPos;

    if (a->direction == LEFT_DIRECTION) {
        angle = 0;
    }
    else if (a->direction == RIGHT_DIRECTION) {
        angle = 180;
    }
    else if (a->direction == UP_DIRECTION) {
        angle = 90;
    }
    else if (a->direction == DOWN_DIRECTION) {
        angle = 270;
    }
    for (int x = 0; x < ARROWBOXWIDTH; x++) {
        for (int y = 0; y < ARROWBOXWIDTH; y++) {
            if (inBounds(a->xPos + (x - (ARROWBOXWIDTH / 2)) * (a->xDir / abs(a->xDir)), a->yPos + y)) {
                if (arrowSprite[x + (y * ARROWBOXWIDTH)] != BLACK) {
                    plot_pixel(rotateX(x - (ARROWBOXWIDTH / 2), y - (ARROWBOXWIDTH / 2), angle) + centerX,
                        rotateY(x - (ARROWBOXWIDTH / 2), y - (ARROWBOXWIDTH / 2), angle) + centerY,
                        arrowSprite[x + (y * ARROWBOXWIDTH)]);
                }
            }
        }
    }
    //plot_pixel(a->xPos, a->yPos, 0xff00);
    //plot_pixel(a->xPos + 1, a->yPos, 0x0000);

}

void drawAllArrows(struct Box* boxes) {
    if (numBoxes <= 0) return;
    for (int i = 0; i < numBoxes; i++) {
        drawSmartArrow(&boxes[i]);
    }
}


void wait_for_vsync() {
    volatile int* pixel_ctrl_ptr = (int*)0xFF203020;
    *pixel_ctrl_ptr = 1;
    int status, currentSound;
    while (1) {
        status = *(pixel_ctrl_ptr + 3);
        currentSound = 0;
        currentSound += getSound(mrAndersonSound, &mrAndersonIndex, MRANDERSON_LENGTH, &hit_flag);
        writeToAudio(currentSound);

        //printf(" %d \n", currentSound);

        if ((status & 1) == 0) {
            globalTime += 1.0 / 60.0;
            pixel_buffer_start = *(pixel_ctrl_ptr + 1);
            return;
        }

    }
}


void writeToAudio(int value) {
    int fifoSpace = *(AudioBase + 1) & 0xFF000000;

    if ((fifoSpace > 32) || (fifoSpace == 0x80000000)) {
        *(AudioBase + 2) = value;
        *(AudioBase + 3) = value;
        //*(AudioBase + 2) = value;
        //*(AudioBase + 3) = value;
    }

}


int getSound(const int array[], int* index, int size, bool* soundcondition) {
    if (!(*soundcondition)) return 0;

    int fifoSpace = *(AudioBase + 1) & 0xFF000000;
    if (!((fifoSpace > 32) || (fifoSpace == 0x80000000))) return 0;

    int value;

    value = array[*index];

    (*index)++;
    //(*index)++;
    if (*index >= size) {
        *index = 0;
        *soundcondition = false;
    }
    return value;
}


int getSWValue() {
    return *LEDaddress;
}

int decodeHex(int value) {
    int result;
    if (value == 0) {
        result = 0b0111111;
    }
    else if (value == 1) {
        result = 0b0000110;
    }
    else if (value == 2) {
        result = 0b1011011;
    }
    else if (value == 3) {
        result = 0b1001111;
    }
    else if (value == 4) {
        result = 0b1100110;

    }
    else if (value == 5) {
        result = 0b1101101;
    }
    else if (value == 6) {
        result = 0b1111101;
    }
    else if (value == 7) {
        result = 0b0000111;
    }
    else if (value == 8) {
        result = 0b1111111;
    }
    else if (value == 9) {
        result = 0b1100111;
    }
    else { return 0; }

    return result;

}

void setSevenSegment(volatile long* segAddress, int value) {

    int seg7;

    seg7 = decodeHex(value % 10);

    value /= 10;
    seg7 += decodeHex(value % 10) << 8;

    value /= 10;
    seg7 += decodeHex(value % 10) << 16;

    value /= 10;
    seg7 += decodeHex(value % 10) << 24;

    *segAddress = seg7;
}


void draw_line(int x0, int y0, int x1, int y1, short int line_color) {
    bool is_steep = abs(y1 - y0) > abs(x1 - x0);
    if (is_steep) {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if (x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = -(deltax / 2);
    int y = y0;
    int y_step;
    if (y0 < y1) {
        y_step = 1;
    }
    else {
        y_step = -1;
    }

    for (int x = x0; x < x1; x++) {
        if (is_steep) {
            plot_pixel(y, x, line_color);
        }
        else {
            plot_pixel(x, y, line_color);
        }
        error = error + deltay;
        if (error > 0) {
            y = y + y_step;
            error = error - deltax;
        }
    }
}

void plot_pixel(int x, int y, short int line_color) {
    volatile short int* one_pixel_address;
    one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
    *one_pixel_address = line_color;
}

void draw_background() {
    int y, x;
    short int color;
    for (x = 0; x < XWIDTH; x++) {
        for (y = 0; y < YHEIGHT; y++) {
            color = backGround[x + (y * XWIDTH)];
            plot_pixel(x, y, color);
        }
    }
}

void swap(int* a, int* b) {
    int temp = *a;
    (*a) = (*b);
    (*b) = temp;
}

int abs(int a) {
    if (a > 0) {
        return a;
    }
    else {
        return -1 * a;
    }
}


void drawHealth(int offset) {
    for (int i=0; i<health; i++) {
        if (abs(i+offset) < 5) {
            drawMiniHeart(((2*i) + (SMALL_HEART_MEASURE*i)), (HEALTH_POSITION - abs(i + offset)));
        }
        else if (i+offset < -SMALL_HEART_MOVEMENT) {
            drawMiniHeart(((2*i) + (SMALL_HEART_MEASURE*i)), (HEALTH_POSITION - SMALL_HEART_MOVEMENT));
        }
        else if (i+offset > SMALL_HEART_MEASURE) {
            drawMiniHeart(((2*i) + (SMALL_HEART_MEASURE*i)), (HEALTH_POSITION + SMALL_HEART_MOVEMENT));
        }
    }
}

void drawMiniHeart(int x, int y) {
    for (int i = 0; i < SMALL_HEART_MEASURE; i++) {
        for (int j = 0; j < SMALL_HEART_MEASURE; j++) {
            if (miniHeartSprite[x + (y * SMALL_HEART_MEASURE)] != BLACK) {
                plot_pixel((x+i), (y+j), miniHeartSprite[x + (y * SMALL_HEART_MEASURE)]);
            }
        }
    }
}



#endif