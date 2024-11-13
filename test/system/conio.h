#include <stdio.h>
#include <stdlib.h>
char keyData = '\0';

//----------------------------------------------
// #include <conio.h>
//----------------------------------------------
// mimic kbhit(), getch() in conio.h in Windows
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
char kbhit(void)
{
    struct termios old, new;
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    int byteswaiting = 0;
    ioctl(STDIN_FILENO, FIONREAD, &byteswaiting);
    keyData = '\0';
    if ( byteswaiting > 0 ) {
        keyData = getchar();
        fflush(stdout);
        printf("\b");
    }
      
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return keyData;
}
char getch (void)
{
    return keyData;
}