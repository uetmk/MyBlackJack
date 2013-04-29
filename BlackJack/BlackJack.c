
/*
- BlackJackルール
    - ホールカードあり。ディーラーは配られた2枚のカードの1枚目を
	  表向きにし、2枚目はショーダウンまで伏せる
	- Aのスコアは11としても1としてもよい
	- ディーラーはスコアが17以上になるまでカードを引き続け、17以上
	  になったら追加カードは引けず、スコアを確定する
	- スプリットあり。プレイヤーは最初に配られたカードが同じ数字
	  だった場合、2つのハンドとしてそれぞれ勝負できる
    - スプリットできるのは1回だけ
	- Aをスプリットした場合、それぞれのハンドで引けるカードは1枚だけ
*/

// コメントはずすとデバッグモード。配られるカードを指定できる
// #define BJDEBUG (1)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Common.h"
#include "CardDeck.h"
#include "CardSlot.h"
#include "ConsoleUtil.h"

const char* ResultStr[3] = {"Win", "Lose", "Draw"};

enum PlayResult
{
    WIN,
    LOSE,
    DRAW,
};

enum SlotID
{
    BASE_SLOT_ID = 1,
    SPLIT_SLOT_ID,
};

struct Player
{
    struct CardSlot slot;
    struct CardSlot spSlot;
    int isDealer;
    int isStand;
    int isSplit;
    int chip;         // not implimented
    int isSurrender;  // not implimented
    int isDoubleDown; // not implimented
    int hasInsurance; // not implimented
};

static struct CardDeck s_cardDeck;

#ifdef BJDEBUG

int drawCard_d()
{
	int drawCard = INVALID_CARD_ID;
	printAvailableCardInfo(&s_cardDeck);
    printf("[DEBUG] Input Card ID manualy\n");
	while(drawCard == INVALID_CARD_ID) {
		drawCard = getUserInputNum("[DEBUG] Input card ID", 0, 51);
		drawCard = serveSpecificCard(&s_cardDeck, drawCard);
	}
    printf("[DEBUG] Serve ");
    printCardInfo(drawCard, 0);
    printf("\n\n");

    return drawCard;
}

#endif

void initPlayer(struct Player* player, int isDealer)
{
    initCardSlot(&player->slot);
    initCardSlot(&player->spSlot);
    player->isDealer = isDealer;
    player->isSplit = FALSE;
    player->isStand = FALSE;
}

struct CardSlot* getCurrentSlot(struct Player* player)
{
    if(player->isSplit)
        if(player->slot.isEndDraw)
            return &player->spSlot;

    return &player->slot;
}

enum SlotID getCurrentSlotID(struct Player* player)
{
    enum SlotID curSlotID = BASE_SLOT_ID;
    if(getCurrentSlot(player) == &player->spSlot) {
        curSlotID = SPLIT_SLOT_ID;
    }
    return curSlotID;
}

void showPlayersCard(const struct Player* player)
{
	int indent = 0;
    int i;

	player->isDealer ? printf("Dealer's card") : printf("Your card ");
    if(player->isSplit) printf("(Splited)");
    printf("\n");

    for(i = 0; i < MAX_PLAYER_CARD_NUM; i++) {
        int card_id = getCardInSlot(&player->slot, i);
        int spCardId = getCardInSlot(&player->spSlot, i);
        if(card_id != INVALID_CARD_ID) {
            if(player->isDealer && i == 1 && !player->isStand) {
                printf("%7s [%2s] (Hole Card)", "XXXXX", "X");
            }
            else {
                indent = 0;
                printCardInfo(card_id, indent);
            }
            if(spCardId != INVALID_CARD_ID) {
                indent = 4;
                printCardInfo(spCardId, indent);
            }
            printf("\n");
        }
        else if(spCardId != INVALID_CARD_ID) {
            indent = 16;
            printCardInfo(spCardId, indent);
            printf("\n");
        }
    }
    printf("\n");
}

void drawCard(struct Player* player)
{
    struct CardSlot* slot = getCurrentSlot(player);
    int newCard;

#ifdef BJDEBUG
    const char* drawer = player->isDealer ? "Dealer" : "Player";
    printf("[DEBUG] %s draw card\n", drawer);
    newCard = drawCard_d();
#else
    newCard = serveCard(&s_cardDeck);
#endif

    pushCardToSlot(slot, newCard);

    calcScoreState(slot, player->isDealer, player->isSplit);
}

int canSplitCard(const struct Player* player)
{
    int canSplit = FALSE;

    if(!player->isDealer && countCardInSlot(&player->slot) == 2) {
        int firstCard = getCardInSlot(&player->slot, 0);
        int secondCard = getCardInSlot(&player->slot, 1);
        if(getCardNumber(firstCard) == getCardNumber(secondCard)) {
            canSplit = TRUE;
        }
    }

    return canSplit;
}

void splitCard(struct Player* player)
{
    int moveCard = popCardFromSlot(&player->slot);
    pushCardToSlot(&player->spSlot, moveCard);

#ifdef BJDEBUG
    printf("[DEBUG] Move ");
    printCardInfo(moveCard, 0);
    printf("\n");
#endif
    player->isSplit = TRUE;

    calcScoreState(&player->slot, player->isDealer, player->isSplit);
    calcScoreState(&player->spSlot, player->isDealer, player->isSplit);
}

void showCards(const struct Player* player, const struct Player* dealer)
{
    showPlayersCard(dealer);
    showPlayersCard(player);
}

enum PlayResult getPlayResultBySlot(const struct CardSlot* playerSlot, 
                                            const struct CardSlot* dealerSlot)
{
    int ret =  compareSlot(playerSlot, dealerSlot);

    enum PlayResult result = LOSE;
    switch(ret) {
        case -1:
            result = LOSE;
            break;
        case 0:
            result = DRAW;
            break;
        case 1:
            result = WIN;
            break;
        default:
            printf("Error\n");
    }
    return result;
}

void printPlayResult(struct Player* player, struct Player* dealer)
{
    struct CardSlot* playerSlot = &player->slot;
    struct CardSlot* playerSpSlot = &player->spSlot;
    struct CardSlot* dealerSlot = getCurrentSlot(dealer);

    const char* scorePrefix = player->isSplit ? "    [Slot1] Score: " : "    Score: ";
    enum PlayResult result;

    printf("Dealer\n");
    printSlotScore(dealerSlot, "    Score: ", TRUE, TRUE);

    printf("Player\n");
    printSlotScore(playerSlot, scorePrefix, TRUE, FALSE);

    result = getPlayResultBySlot(playerSlot, dealerSlot);
    printf(" => You %s\n", ResultStr[result]);

    if(player->isSplit) {
        printSlotScore(playerSpSlot, "    [Slot2] Score: ", TRUE, FALSE);
        result = getPlayResultBySlot(playerSpSlot, dealerSlot);
        printf(" => You %s\n", ResultStr[result]);
    }
    printf("\n");
}

int isStand(const struct Player* player)
{
    int isStand = FALSE;
    if(player->isSplit) {
        if(player->slot.isEndDraw && player->spSlot.isEndDraw)
            isStand = TRUE;
    }
    else {
        if(player->slot.isEndDraw)
            isStand = TRUE; 
    }

    return isStand;
}

void onGameEnd()
{
    srand((unsigned int)time(NULL));
    if(!(rand() % 6)) printEndAnimation();
}

void playBlackJack(struct Player* player, struct Player* dealer)
{
    int i;

    printFramedText("Game start");
    printf("\n");

    initPlayer(player, FALSE);
    initPlayer(dealer, TRUE);
    shuffleCards(&s_cardDeck);

    for(i = 0; i < 2; i++) {
        drawCard(player);
    }
    for(i = 0; i < 2; i++) {
        drawCard(dealer);
    }

    showCards(player, dealer);

	// In case of same card, ask if player splits card.
    if(canSplitCard(player)){
        if(getUserDecision("You have two same number cards. Split card ?")) {
            printFramedText("Player split card");
            printf("\n");
            splitCard(player);
            showCards(player, dealer);
        }
    }

	// Player draw card.
    while(player->isStand == FALSE) {
        if(player->isSplit) printf("[Slot %d] ", (int)getCurrentSlotID(player));
        if(getUserDecision("Draw one more card ?")) {
            printFramedText("Player draw card");
            printf("\n");
            drawCard(player);
            showCards(player, dealer);
        }
        else {
            struct CardSlot* slot = getCurrentSlot(player);
            slot->isEndDraw = TRUE;
        }
        player->isStand = isStand(player);
    }

	// Dealer draw card.
    while(isStand(dealer) == FALSE) {
        drawCard(dealer);
    }
    dealer->isStand = TRUE;

	// Show play result.
    printFramedText("Dealer draw card");
    printf("\n");
    showCards(player, dealer);
    printPlayResult(player, dealer);

    onGameEnd();
}

int main(void)
{
    struct Player player;
    struct Player dealer;

    do {
        playBlackJack(&player, &dealer);
    } while(getUserDecision("Replay Game ?"));

    return 0;
}

