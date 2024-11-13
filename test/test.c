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
    int jumping;
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
    player.velocity.x = 0;
    player.velocity.y = 0;
    player.jumping = 0;
    memset(keyPressed, 0, sizeof(keyPressed));
    DrawScreen();
}

int IntersectCourse(VEC2 _v) {
    int x = (int) _v.x;
    int y = (int) _v.y;
    switch(course_1_1[y][x]) {
        case 'b':
        case 'p':
        case 'q':
            return 1;
    }

    return 0;
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
            player.velocity.y += 0.005f;    // gravity
            
            player.position.x += player.velocity.x;
            player.position.y += player.velocity.y;

            VEC2 left = {
                player.position.x,
                player.position.y + 0.5f
            };
            if(IntersectCourse(left)) {
                player.position.x = floorf(player.position.x) + 1.0f;
                player.velocity.x = 0;
            }
            VEC2 right = {
                player.position.x + 1.0f,
                player.position.y + 0.5f
            };
            if(IntersectCourse(right)) {
                player.position.x = floorf(player.position.x);
                player.velocity.x = 0;
            }

            VEC2 up = {
                player.position.x + 0.5f,
                player.position.y
            };
            if(IntersectCourse(up)) {
                player.position.y = floorf(player.position.y) + 1.0f;
                player.velocity.y = 0;
            }

            VEC2 down = {
                player.position.x + 0.5f,
                player.position.y + 1.0f
            };
            if(IntersectCourse(down)) {
                player.position.y = floorf(player.position.y);
                player.velocity.y = 0;
                player.jumping = 0;
            }else{
                player.jumping = 1;
            }
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
                default:
                    if(!player.jumping) player.velocity.y = -0.15f; // jump power
                    break;
            }
        }
    }
}