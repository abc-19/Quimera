#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void enableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term); // Get current terminal settings
    term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // Apply changes
}

int main() {
    char c;

    printf("Press arrow keys (Press 'q' to quit):\n");
    enableRawMode(); // Enable raw mode

	char buff[4];

    while (1) {
		read(0, buff, 4);
		printf("%d %d %d %d\n", *buff, buff[1], buff[2], buff[3]);

		if (*buff == 0x1b && buff[1] == '[') {
            switch (buff[2]) {
                    case 'A': printf("Up arrow\n"); break;
                    case 'B': printf("Down arrow\n"); break;
                    case 'C': printf("Right arrow\n"); break;
                    case 'D': printf("Left arrow\n"); break;
                }
		}

		if (*buff == '1') break;
    }

    struct termios term;
    tcgetattr(STDIN_FILENO, &term); // Get current terminal settings
    term.c_lflag |= (ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // Apply changes

    return 0;
}

