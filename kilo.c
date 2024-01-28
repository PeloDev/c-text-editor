# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <termios.h>
# include <unistd.h>

struct termios orig_termios;

/**
 * Restore original terminal attributes
*/
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/**
 * Set the terminals attributes so that it behaves more like a Text Editor by:
 * - Turning off echoing
 * - Turning off canonical mode
*/
void enableRawMode() {
    // get terminal attributes and assign them to the original termios struct
    tcgetattr(STDIN_FILENO, &orig_termios);

    // revert to original at exit
    atexit(disableRawMode);

    // create raw struct from original for updated terminal attributes
    struct termios raw = orig_termios;

    /**
     * Turn off echoing (ECHO)
     * - You won't see your input in the terminal anymore.
     * - Disabling the ECHO flag is how password inputs are protected in the terminal (e.g. `sudo` password input).
     * - Note: "q to quit" won't set your terminal back to ECHO inputs unless we revert terminal attributes back
     * to their original values. (If all else fails Ctrl+C will reset your terminal in most cases) 
     *
     * Turn off canonical mode (ICANON)
     * - Inputs will be read byte-by-byte instead of after pressing ENTER.
     * - "q to quit" will enact immediately
    */
    raw.c_lflag &= ~(ECHO | ICANON); // `~` is bitwise NOT operator

    // set the terminal attributes with the new value (TCSAFLUSH -> apply this after all pending outputs are written to the terminal, and discard unread inputs)
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    /**
     * Listen for key press events from the user.
     * Read 1 byte from the standard input into char c until the end of the standard input (file)?
     * Keyboard inputs will get read into char c in "canonical/cooked mode" 
     * (ie: they will only be read after pressing ENTER, like terminal commands)
    */
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') { // q to quit
        /**
         * Display ASCII values and character representations (if printable) for key presses.
        */
        if (iscntrl(c)) { // test if `c` is a control character (ie: non-printable characters like ESC, BACKSPACE, TAB, ENTER/RETURN)
            printf("%d\n", c); // %d formats to decimal
        } else {
            printf("%d ('%c')\n", c, c); // %c outputs the byte directly as a character
        }
    }
    return 0;
}