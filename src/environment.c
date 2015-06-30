#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void setEnvironment(void);
void checkTerminal(void);
void saveAttributes(void);
void setMenuMode(void);
void setRaceMode(void);
void setNoncanonicalMode(int vmin, int vtime);
void registerSigIntCatcher(void);
void sigIntCatcher(int signum);
void disableRepeatAndCursor(void);
void resetEnvironment(void);
void resetInputMode(void);
void enableRepeatAndCursor(void);
void resetConsole(void);

//////////////////////////////

int const DISABLE_REPEAT = 0;

struct termios saved_attributes;
volatile sig_atomic_t pleaseExit = 0;

////////// AT START //////////

void setEnvironment() {
	checkTerminal();
	saveAttributes();
	setMenuMode();
	registerSigIntCatcher();
	disableRepeatAndCursor();
}

void checkTerminal() {
	if (!~isatty(STDIN_FILENO)) {
		printf("Not a terminal: %d.\n", STDIN_FILENO);
		exit(EXIT_FAILURE);
	}
}

void saveAttributes() {
	tcgetattr(STDIN_FILENO, &saved_attributes);
}

/* 
 * Sets blocking mode (getc waits for input).
 */
void setMenuMode() {
	setNoncanonicalMode(1, 0);
}

/*
 * Sets nonblocking mode (getc does not wait for input, it returns every 0.1 s).
 */
void setRaceMode() {
	setNoncanonicalMode(0, 1);
}

void setNoncanonicalMode(int vmin, int vtime) {
	struct termios tattr;
	// Set noncanonical mode and disable echo.
	atexit(resetEnvironment);
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO);
	tattr.c_cc[VMIN] = vmin;
	tattr.c_cc[VTIME] = vtime;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

void registerSigIntCatcher() {
	struct sigaction action;
	// Reset all members.
	memset(&action, 0, sizeof(action));
	action.sa_handler = sigIntCatcher;
	sigaction(SIGINT, &action, NULL);
}

/*
 * Gets executed when ctrl-c is pressed. It necesary so that atexit method 
 * gets executed. atexit sets terminal back to the original state.
 */
void sigIntCatcher(int signum) {
	pleaseExit = 1;
}

void checkRetVal(int retVal, char const errMsg[]) {
	if (retVal == -1) {
		fprintf(stderr, "%s", errMsg);
	}
}

void disableRepeatAndCursor() {
	if (DISABLE_REPEAT) {
		// Disable repeat in xwindow console.
		int retVal = system("xset -r");
		checkRetVal(retVal, "Could disable key repeat.");
		// Disable repeat in Linux console.
		retVal = system("setterm --repeat off");
		checkRetVal(retVal, "Could disable key repeat.");
	}
	// Set cursor off. Could also probably use system("setterm -cursor off).
	printf("\e[?25l");
	fflush(stdout);
}

/////////// AT END ///////////

void resetEnvironment() {
	resetInputMode();
	enableRepeatAndCursor();
	resetConsole();
}

void resetConsole() {
	int retVal = system("reset");
	checkRetVal(retVal, "Could not reset the screen.");
}

/*
 * Brings back canonical mode.
 */
void resetInputMode() {
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}

void enableRepeatAndCursor() {
	if (DISABLE_REPEAT) {
		// Enables repeat in Xwindow console.
		int retVal = system("xset r");
		checkRetVal(retVal, "Could not enable key repeat.");
		// Disables repeat in Linux console.
		retVal = system("setterm --repeat on");
		checkRetVal(retVal, "Could not enable key repeat.");
	}
	int retVal = system("clear");
	checkRetVal(retVal, "Could not clear the screen.");
	// Brings back cursor.
	printf("\e[?25h");
	fflush(stdout) ;
}

