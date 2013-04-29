
#include "Common.h"
#include "CardDeck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CARD_SWAP(a, b) {int tmp = *a; *a = *b; *b = tmp;}

const char* CardNumStr[14] = {NULL, "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

const char* CardTypeStr[5] = {"SPADE", "HEART", "DIAMOND", "CLOVER", "JOKER"};

const int NormalCardNum = 52;

void shuffleCards(struct CardDeck* cardDeck)
{
    int i = 0;
    int swapCount = 100;

    cardDeck->index = 0;
    for(i = 0; i < MAX_CARD_NUM; i++) {
        cardDeck->cards[i] = i;
    }

    srand((unsigned int)time(NULL));

    for(i = 0; i < swapCount; i++) {
        int ind_a = rand() % MAX_CARD_NUM;
        int ind_b = rand() % MAX_CARD_NUM;
        CARD_SWAP(&cardDeck->cards[ind_a], &cardDeck->cards[ind_b]);
    }
}

int serveCard(struct CardDeck* cardDeck)
{
    int card = INVALID_CARD_ID;
    if(cardDeck->index < MAX_CARD_NUM) {
        card = cardDeck->cards[cardDeck->index++];
    }
    return card;
}

int serveSpecificCard(struct CardDeck* cardDeck, int card_id)
{
    int card = INVALID_CARD_ID;
    int i;

    if(!isUsedCard(cardDeck, card_id)) {
        for(i = cardDeck->index; i < MAX_CARD_NUM; i++)
            if(cardDeck->cards[i] == card_id) break;
        CARD_SWAP(&cardDeck->cards[i], &cardDeck->cards[cardDeck->index]);
        cardDeck->index++;
        card = card_id;
    } else {
        printf("Error: Card ID %d is already used.\n", card_id);
    }

    return card;
}

int getCardType(int card_id)
{
    if(card_id == INVALID_CARD_ID) printf("Error: getCardType\n");

    return card_id / 13;
}

int getCardNumber(int card_id)
{
    int number = 0;
    if(INVALID_CARD_ID < card_id && card_id < NormalCardNum)
        number = card_id % 13 + 1;

    return number;
}

void getCardInfoStr(int card_id, char* str)
{
    char buffer[14];
    const int type = getCardType(card_id);
    const int number = getCardNumber(card_id);

    safe_sprintf(buffer, sizeof(buffer), "%7s [%2s]", CardTypeStr[type], CardNumStr[number]);
    safe_strcpy(str,  buffer, strlen(buffer) + 1);
}

int getUsedCardNum(const struct CardDeck* cardDeck)
{
    return cardDeck->index;
}

int isUsedCard(const struct CardDeck* cardDeck, int card_id)
{
    int isUsed = FALSE;
    int i;

    for(i = 0; i < cardDeck->index; i++)
        if(cardDeck->cards[i] == card_id) isUsed = TRUE;

    return isUsed;
}

void printCardInfo(int card_id, int indent)
{
    char buffer[16];
    int i;

    getCardInfoStr(card_id, buffer);
    for(i = 0; i < indent; i++) printf(" ");
    printf("%s", buffer);
}

void printAvailableCardInfo(const struct CardDeck* cardDeck)
{
    int i, j, cardNum;

    printf("\n");
    for(i = 0; i < 4; i++) printf("%-12s", CardTypeStr[i]);
    printf("\n");

    for(i = 0; i < MAX_CARD_NUM / 4; i++) {
        for(j = i; j < MAX_CARD_NUM; j+=13) {
            cardNum = getCardNumber(j);
            if(!isUsedCard(cardDeck, j))
                printf("%2d: [%2s]    ", j, CardNumStr[cardNum]);
            else
                printf("%12s", " ");
        }
        printf("\n");
    }
    printf("\n");
}
