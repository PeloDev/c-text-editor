# Run this command to allow permissions to run this script: `chmod u+x ./test/errortcgetattr.sh`
# You can make tcgetattr fail by giving the program a text file or pipe as a standard input instead of the terminal

# Uncomment one of the following lines, either should break the program:
# ./kilo <kilo.c # give the program a file
echo test | ./kilo # give the program a pipe