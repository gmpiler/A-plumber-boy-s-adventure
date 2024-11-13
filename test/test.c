#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "system/conio.h"
#include "course/course.h"
#include "course/1-1.h"

#define UPDATE_FPS  (60)
#define UPDATE_INTERVAL (10000 / UPDATE_FPS)
#define DRAW_FPS    (10)
#define DRAW_INTERVAL   (1000000/ DRAW_FPS)

typedef struct {
    float x, y;
} VEC2;

typedef struct {
    VEC2 position;
    VEC2 velocity;
} PLAYER;

PLAYER player;

int keyPressed[256];

void DrawScreen()
{
    int scrollX = (int)roundf(player.position.x) - SCREEN_WIDTH/2;
    if (scrollX < 0) scrollX = 0;
    for(int y = 0; y < SCREEN_HEIGHT; y++)
        for(int x = 0; x < SCREEN_WIDTH; x++)
            screen_buf[y][x] = course_1_1[y][scrollX + x];

    {
        int x = (int)roundf(player.position.x) - scrollX;
        int y = (int)roundf(player.position.y);
        if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
        screen_buf[y][x] = '@';
    }

    system("clear");
    for(int y = 0; y < SCREEN_HEIGHT; y++){
        for(int x = 0; x < SCREEN_WIDTH; x++){
            printf("%s", aa[screen_buf[y][x]]);
        }
        printf("\n");
    }
}

void  Init() {
    player.position.x = SCREEN_WIDTH/2;
    player.position.y = 13;
    memset(keyPressed, 0, sizeof(keyPressed));
    DrawScreen();
}

int main(){
    sprintf(aa[0], "x");
    sprintf(aa[' '], " ");
    sprintf(aa['b'], "#");
    sprintf(aa['m'], "^");
    sprintf(aa['p'], "+");
    sprintf(aa['t'], "Y");
    sprintf(aa['q'], "?");
    sprintf(aa['c'], "~");
    sprintf(aa['g'], "|");
    sprintf(aa['f'], "F");
    sprintf(aa['@'], "*");

    Init();

    clock_t lastUpdateClock = clock();
    clock_t lastDrawClock   = clock();
    while(1) {
        clock_t nowClock = clock();
        if (nowClock >= lastUpdateClock + UPDATE_INTERVAL) {
            lastUpdateClock = nowClock;
            float acceleration = 0.005f;
            if (keyPressed['d'])
                player.velocity.x += acceleration;
            if (keyPressed['a'])
                player.velocity.x -= acceleration;

            player.velocity.x *= 0.95f;
            
            player.position.x += player.velocity.x;
            player.position.y += player.velocity.y;
        }

        if (nowClock >= lastDrawClock + DRAW_INTERVAL) {
            lastDrawClock = nowClock;
            DrawScreen();
        }

        if (kbhit()) {
            switch(getch()) {
                case 'a':
                    if (keyPressed['d'])
                        keyPressed['d'] = 0;
                    else
                        keyPressed['a'] = 1;
                    break;
                case 'd':
                    if (keyPressed['a'])
                        keyPressed['a'] = 0;
                    else
                        keyPressed['d'] = 1;
                    break;
            }
        }
    }
}