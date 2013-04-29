
#ifndef CARD_DECK_H_
#define CARD_DECK_H_

#define JOKER_CARD_NUM (0)

#define MAX_CARD_NUM (52 + JOKER_CARD_NUM)

#define INVALID_CARD_ID (-1)

//  1 - 13 spade
// 14 - 26 heart
// 27 - 39 diamond
// 40 - 52 clover
// 53 - 54 jorker

struct CardDeck
{
    int cards[MAX_CARD_NUM];
    int index;
};

void shuffleCards(struct CardDeck* cardDeck);

int serveCard(struct CardDeck* cardDeck);

int serveSpecificCard(struct CardDeck* cardDeck, int card_id);

int getCardType(int card_id);

int getCardNumber(int card_id);

int getUsedCardNum(const struct CardDeck* cardDeck);

void printCardInfo(int card_id, int indent);

void printAvailableCardInfo(const struct CardDeck* cardDeck);

#endif // ! CARD_DECK_H_

