
#include "Common.h"
#include "ConsoleUtil.h"

#if defined(WIN32)
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getUserDecision(const char* message)
{
    int isOK = FALSE;
    char str[80] = {0};

    while(str[0] != 'y' && str[0] != 'n') {
        printf("%s [y/n]: ", message);
        fgets(str, sizeof(str), stdin);
    }

    if(str[0] == 'y') isOK = TRUE;
    printf("\n");

    return isOK;
}

int getUserInputNum(const char* message, int min, int max)
{
    int ret_val;
    char str[80] = {0};

    do {
        printf("%s [%d - %d]: ", message, min, max);
        fgets(str, sizeof(str), stdin);
        ret_val = atoi(str);
    } while(ret_val < min || max < ret_val);
    printf("\n");

    return ret_val;
}

void printFramedText(const char* message)
{
    int barLength = strlen(message) + 2;

    int i;
    printf("+");
    for(i = 0; i < barLength; i++) printf("-");
    printf("+\n| ");
    printf("%s", message);
    printf(" |\n+");
    for(i = 0; i < barLength; i++) printf("-");
    printf("+\n");
}

void printEndAnimation()
{
#if defined(WIN32)
	int i, j;

	for(i = 0; i < 16; i++ ) {
		for(j = 0; j < i; j++) printf(" ");
		printf("„¡(„¡ ^o^)„¢");
		Sleep(60);
		printf("\r");
	}
    for(j = 0; j < i; j++) printf(" ");
    printf("„¡(„¡ ^o^)„¢ƒzƒ‚ƒH");
	Sleep(800);
    printf("\r");
	printf("                                        \n");
#endif
}
