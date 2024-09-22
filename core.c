/*
Core Module
- User interface functions
- User input functions
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "core.h"

//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void)
{
    // Discard all remaining char's from the standard input buffer:
    while (getchar() != '\n')
    {
        ; // do nothing!
    }
}

// Wait for user to input the "enter" key to continue
void suspend(void)
{
    printf("<ENTER> to continue...");
    clearInputBuffer();
    putchar('\n');
}


//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get a valid integer from the keyboard
int inputInt(void)
{
    char newLine = { '\0' };
    int value = 0, scanReturn = 0;

    while (newLine != '\n' && scanReturn != 2)
    {
        scanReturn = scanf("%d%c", &value, &newLine);
        if (scanReturn != 2 || newLine != '\n')
        {
            clearInputBuffer();
            printf("Error! Input a whole number: ");
        }
    }

    return value;
}

// Get a valid integer from the keyboard and validate if the value entered is greater than 0
int inputIntPositive(void)
{
    int value = 0;
    while (value <= 0)
    {
        value = inputInt();
        if (value <= 0)
            printf("ERROR! Value must be > 0: ");
    }

    return value;
}

// Get a valid integer from the keyboard and validate if its between lower-bound and upper-bound
int inputIntRange(int lowerBound, int upperBound)
{
    int value = 0;
    do
    {
        value = inputInt();
        if (value < lowerBound || value > upperBound)
            printf("ERROR! Value must be between %d and %d inclusive: ", lowerBound, upperBound);
    } while (value < lowerBound || value > upperBound);

    return value;
}

// Get a single character from the keyboard and validate it against an array of chars
char inputCharOption(const char stringArray[])
{
    char inputChar = '\0', newLine = '\0';
    int scanReturn = 0;
    char* isInStringArray = NULL;

    while (newLine != '\n')
    {
        scanReturn = scanf("%c%c", &inputChar, &newLine);
        if (newLine != '\n' || scanReturn != 2)
        {
            clearInputBuffer();
            printf("ERROR: Character must be one of [%s]: ", stringArray);
        }
        else
        {
            isInStringArray = strchr(stringArray, inputChar);
            if (!isInStringArray)
            {
                clearInputBuffer();
                printf("ERROR: Character must be one of [%s]: ", stringArray);
                newLine = '\0';
            }
        }
    }

    return inputChar;
}

// Get a string and valid if it's length is between minChar and maxChar
void inputCString(char* cString, int minChar, int maxChar)
{
    int cStringLength = 0;

    while (cStringLength < minChar || cStringLength > maxChar)
    {
        scanf("%[^\n]", cString);

        cStringLength = strlen(cString);
        clearInputBuffer();

        if (minChar == maxChar && cStringLength != maxChar)
        {
            printf("ERROR: String length must be exactly %d chars: ", maxChar);
        }
        else
        {
            if (cStringLength < minChar)
                printf("ERROR: String length must be between %d and %d chars: ", minChar, maxChar);

            if (cStringLength > maxChar)
            {
                printf("ERROR: String length must be no more than %d chars: ", maxChar);
            }
        }
    }
}

// Display an array of 10-character digits as a formatted phone number
void displayFormattedPhone(const char* cString)
{
    int i = 0, correctInput = 1, minAscii = 48, maxAscii = 57;

    if (cString != NULL)
    {
        while (cString[i] != '\0' || i < 10)
        {
            if (i >= 10 || cString[i] < minAscii || cString[i] > maxAscii)
                correctInput = 0;
            i++;
        }
    }

    if (correctInput && cString != NULL)
    {
        printf("(");
        for (i = 0; i < 3; i++)
            printf("%c", cString[i]);
        printf(")");
        for (i = 3; i < 6; i++)
            printf("%c", cString[i]);
        printf("-");
        for (i = 6; i < 10; i++)
            printf("%c", cString[i]);
    }
    else
        printf("(___)___-____");

}
