//----------------------------------------------
//#include <windows.h>
//----------------------------------------------
// mimic Sleep() in windows.h in Windows
void Sleep (int msec) {
    int usec = msec*1000;
    usleep(usec);
}
//----------------------------------------------

int execLoop = 1;

void keyEventProc(void)
{
    keyData = getch();
    if (keyData == 27) {
        printf("\rquit by [Esc]");
        fflush(stdout);
        execLoop = 0;
    } else if (32 <= keyData && keyData <= 126) {
        printf("\r[%c]-key was hit",keyData);
        fflush(stdout);
	Sleep(1000);
    }
}

void idleProc(void)
{
    printf("\ridle process   ");
    fflush(stdout);
}

int main(void)
{
    while (execLoop) {
        if (kbhit()) {
            keyEventProc();
        } else {
            idleProc();
        }
    }
    return 0;
}