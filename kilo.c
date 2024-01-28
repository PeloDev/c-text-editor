# include <unistd.h>

int main() {
    /**
     * Listen for key press events from the user.
     * Read 1 byte from the standard input into char c until the end of the standard input (file)?
     * Keyboard inputs will get read into char c in "canonical/cooked mode" 
     * (ie: they will only be read after pressing ENTER, like terminal commands)
    */
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1);
    return 0;
}