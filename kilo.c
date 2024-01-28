# include <termios.h>
# include <unistd.h>

/**
 * Set the terminals attributes so that it behaves more like a Text Editor by:
 * - Turning off echoing
*/
void enableRawMode() {
    /**
     * Turn off echoing - you won't see your input in the terminal anymore.
     * Disabling ECHO is how password inputs are protected in the terminal (e.g. `sudo` password input).
     * At this point "q to quit" won't set your terminal back to ECHO inputs, 
     * Ctrl+C will reset your terminal though (this is true for most cases)
    */
    struct termios raw;
    // get terminal attributes and assign them to the raw termios struct
    tcgetattr(STDIN_FILENO, &raw);
    // update the terminal attributes in your struct to disable (`~` is bitwsie-NOT operator) the ECHO flag
    raw.c_lflag &= ~(ECHO);
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
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'); // q to quit
    return 0;
}