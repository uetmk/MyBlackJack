
#ifndef USER_INPUT_H_
#define USER_INPUT_H_

int getUserDecision(const char* message);

int getUserInputNum(const char* message, int min, int max);

void printFramedText(const char* message);

void printEndAnimation();

#endif

