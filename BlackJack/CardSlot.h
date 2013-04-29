
#ifndef CARD_SLOT_H_
#define CARD_SLOT_H_

#define MAX_PLAYER_CARD_NUM 12

struct CardSlot
{
    int cards[MAX_PLAYER_CARD_NUM];
    int score;
    int isBust;
    int isBlackJack;
    int isEndDraw;
};

void initCardSlot(struct CardSlot* cardSlot);

int countCardInSlot(const struct CardSlot* slot);

int getCardInSlot(const struct CardSlot* slot, int index);

int findCardInSlot(const struct CardSlot* slot, int cardId);

void pushCardToSlot(struct CardSlot* cardSlot, int newCard);

int popCardFromSlot(struct CardSlot* cardSlot);

int countSameNumberCard(const struct CardSlot* slot, int targetCardNum);

void calcScoreState(struct CardSlot* slot, int isDealer, int isSplit);

int compareSlot(const struct CardSlot* lSlot, const struct CardSlot* rSlot);

void printSlotScore(const struct CardSlot* slot, const char* prefix, int printStatus, int hasLineFeed);

#endif // ! CARD_SLOT_H_

