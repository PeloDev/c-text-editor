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
     * Notes:
     * - c_iflag is for input flags
     * 
     * (IXON) Disable signals which stop and start data transmission to the terminal
     * 
     * (ICRNL) prevent translation of carriage returns (CR) (13, '\r') into newlines (10, '\n')
     * - This is applicable to both ENTER/RETURN and Ctrl+M curiosly enough.
     * - (10, '\n') is what Ctrl+J will output.
    */
    raw.c_iflag &= ~(ICRNL | IXON);


    /**
     * Notes:
     * - c_oflag is for output flags
     * 
     * (OPOST) Disable translation of "\r" and "\n" to "\r\n"
     * - Interstingly these are both distinct commands,
     *   - "\r" is a "carriage return" which moves the cursor to the beggining of the line,
     *   - "\n" is the very familiar newline command which moves the cursor down one line,
     *   - the distinction of both these commands were relevant in the days of typewriters and "teletypes".
    */
    raw.c_oflag &= ~(OPOST);

    /**
     * Notes:
     *  - c_iflag is for local flags (“dumping ground for other state”)
     * 
     * Turn off echoing (ECHO)
     * - You won't see your input in the terminal anymore.
     * - Disabling the ECHO flag is how password inputs are protected in the terminal (e.g. `sudo` password input).
     * - Note: "q to quit" won't set your terminal back to ECHO inputs unless we revert terminal attributes back
     * to their original values. (If all else fails Ctrl+C will reset your terminal in most cases) 
     *
     * Turn off canonical mode (ICANON)
     * - Inputs will be read byte-by-byte instead of after pressing ENTER.
     * - "q to quit" will enact immediately
     * 
     * Disable signals which wait for other characters to be typed before sending to terminal (IEXTEN)
     * 
     * Disable SIGINT (which terminates the program: Ctrl+C) and SIGSTP (which suspends the program)
     * - disabling ISIG does the job
    */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); // `~` is bitwise NOT operator

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
            printf("%d\r\n", c); // %d formats to decimal
        } else {
            printf("%d ('%c')\r\n", c, c); // %c outputs the byte directly as a character
        }
    }
    return 0;
}