
#include "Common.h"
#include "CardSlot.h"
#include "CardDeck.h"

#include <stdio.h>

static const int BJ_SCORE = 21;

static const int ScoreTable[14] = {0, 11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

int getScoreOfCard(int card_id)
{
    int card_num = getCardNumber(card_id);

    return ScoreTable[card_num];
}

void initCardSlot(struct CardSlot* cardSlot)
{
    int i;
    for(i = 0; i < MAX_PLAYER_CARD_NUM; i++) {
        cardSlot->cards[i] = INVALID_CARD_ID;
    }
    cardSlot->score = 0;
    cardSlot->isBust = FALSE;
    cardSlot->isBlackJack = FALSE;
    cardSlot->isEndDraw = FALSE;
}

int countCardInSlot(const struct CardSlot* slot)
{
    int i;
    for(i = 0; i < MAX_PLAYER_CARD_NUM; i++) {
        if(slot->cards[i] == INVALID_CARD_ID) break;
    }
    return i;
}

int getCardInSlot(const struct CardSlot* slot, int index)
{
    if(index < 0 || MAX_PLAYER_CARD_NUM <= index) {
        return INVALID_CARD_ID;
    }

    return slot->cards[index];
}

int findCardInSlot(const struct CardSlot* slot, int cardId)
{
    int ret_index = -1;
    int i;

    if(cardId != INVALID_CARD_ID) {
        for(i = 0; i < MAX_PLAYER_CARD_NUM; i++) {
            if(slot->cards[i] == cardId) ret_index = i;
        }
    }

    return ret_index;
}

void pushCardToSlot(struct CardSlot* cardSlot, int newCard)
{
    int pushIdx = countCardInSlot(cardSlot);

    cardSlot->cards[pushIdx] = newCard;
}

int popCardFromSlot(struct CardSlot* cardSlot)
{
    int popIdx = countCardInSlot(cardSlot) - 1;
    int popCard = cardSlot->cards[popIdx];
    cardSlot->cards[popIdx] = INVALID_CARD_ID;

    return popCard;
}

int countSameNumberCard(const struct CardSlot* slot, int targetCardNum)
{
    int sum = 0;
    int i = 0;
    for(i = 0; i < MAX_PLAYER_CARD_NUM; i++) {
        if(slot->cards[i] != INVALID_CARD_ID) {
            int number = getCardNumber(slot->cards[i]);
            if(number == targetCardNum) sum++;
        }
    }
    return sum;
}

void calcScoreState(struct CardSlot* slot, int isDealer, int isSplit)
{

    int sum = 0;
    int i;
	int aceNum;

    for(i = 0; i < MAX_PLAYER_CARD_NUM; i++) {
        sum += getScoreOfCard(slot->cards[i]);
    }

    // decide whether regard Ace card score  as "1". 
    aceNum = countSameNumberCard(slot, 1);
    for(i = 0; i < aceNum; i++) {
        if(sum > BJ_SCORE) {
            sum -= 10;  // change Ace card socre 11 -> 1
        }
    }

    slot->score = sum;

    if(countCardInSlot(slot) == 2 && slot->score == BJ_SCORE)
        slot->isBlackJack = isSplit ? FALSE : TRUE;

    if(slot->score > BJ_SCORE) slot->isBust = TRUE;

    if(slot->isBlackJack || slot->isBust) slot->isEndDraw = TRUE;

    // In case of Ace split, player can add only one card for each slot.
    if(isSplit && countCardInSlot(slot) == 2) {
        int firstCard = getCardInSlot(slot, 0);
        int splitedCardNumber = getCardNumber(firstCard);
        if(splitedCardNumber == 1) slot->isEndDraw = TRUE;
    }

    // Dealer can't draw card when score is more than 17.
    if(isDealer && slot->score >= 17) slot->isEndDraw = TRUE;
}

int compareSlot(const struct CardSlot* lSlot, const struct CardSlot* rSlot)
{
    int result;

    if(lSlot->isBlackJack) {
        result =  rSlot->isBlackJack ? 0 : 1;
    }
    else if(lSlot->isBust) {
        result = -1;
    }
    else {
        if(rSlot->isBlackJack) {
            result = -1;
        }
        else if(rSlot->isBust) {
            result = 1;
        }
        else {
            int d = lSlot->score - rSlot->score;
            if(d > 0) result = 1;
            else if(d == 0) result = 0;
            else result = -1;
        }
    }

    return result;
}

void printSlotScore(const struct CardSlot* slot, const char* prefix, int printStatus, int hasLineFeed)
{
    printf("%s%2d", prefix, slot->score);

    if(printStatus) {
        if(slot->isBust)
            printf(" (Bust)");
    
        if(slot->isBlackJack)
            printf(" (Black Jack)");
    }

    if(hasLineFeed) {
        printf("\n");
    }
}

