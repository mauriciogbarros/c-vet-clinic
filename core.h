#ifndef CORE_H
#define CORE_H

//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void);

// Wait for user to input the "enter" key to continue
void suspend(void);


//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get a valid integer from the keyboard
int inputInt(void);

// Get a valid integer from the keyboard and validate if the value entered is greater than 0
int inputIntPositive(void);

// Get a valid integer from the keyboard and validate if its between lower-bound and upper-bound
int inputIntRange(int lowerBound, int upperBound);

// Get a single character from the keyboard and validate it against an array of chars
char inputCharOption(const char stringArray[]);

// Get a string and valid if it's length is between minChar and maxChar
void inputCString(char* cString, int minChar, int maxChar);

// Display an array of 10-character digits as a formatted phone number
void displayFormattedPhone(const char* cString);

#endif // !CORE_H
