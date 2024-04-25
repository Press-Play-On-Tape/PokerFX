#include <ArduboyFX.h>

#include "fxdata/fxdata.h"
#include "src/entities/Entities.h"
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "src/utils/SpritesU.hpp"


void play_Init() {

    gameState = GameState::Play;
    
    for (Particle &particle : particles) {

        particle.setCounter(0);

    }

    startGame();

}

void render(uint8_t currentPlane) {

    SpritesU::drawOverwriteFX(0, 0, Images::HUD, currentPlane);
    updateDisplay(currentPlane);

}

void play_Update() {

    frameCount++;

    updateInput();
    updateRound();

}

void play(ArduboyGBase_Config<ABG_Mode::L4_Triplane>& a) {

    if (a.needsUpdate()) play_Update();

    uint8_t currentPlane = a.currentPlane();
    render(currentPlane);


    // Render particles and scores ..

    updateAndRenderParticles(frameCount);

}

void updateInput() {

    uint8_t justPressed = getJustPressedButtons();
    uint8_t pressed = getPressedButtons();
    uint8_t oldBet = bet;

    // Button Up - Increase Bet
    if (currentRound == Rounds::Bet && (justPressed & UP_BUTTON)) {

        winTimer = 0;
        handResult = HandResult::NoHand;
        cardSelect = 0;
        
        switch (bet) {

            case 5:
                if (bet + 5 <= cookie.bank) {
                    bet = 10;  
                    playSFX(MusicSFX::SFX_Bet2);
                }
                break;

            case 10:
                if (bet + 15 <= cookie.bank) {
                    bet = 25;  
                    playSFX(MusicSFX::SFX_Bet3);
                }
                break;

            case 25:
                if (bet + 25 <= cookie.bank) {
                    bet = 50;  
                    playSFX(MusicSFX::SFX_Bet4);
                }
                break;

            case 50:
                if (bet + 50 <= cookie.bank) {
                    bet = 100;  
                    playSFX(MusicSFX::SFX_Bet5);
                }
                break;

        }

    }

    // Button Down - Decrease Bet

    if (currentRound == Rounds::Bet && (justPressed & DOWN_BUTTON)) {

        winTimer = 0;
        handResult = HandResult::NoHand;
        cardSelect = 0;

        switch (bet) {

            case 10:
                bet = 5;  
                playSFX(MusicSFX::SFX_Bet1);
                break;

            case 25:
                bet = 10; 
                playSFX(MusicSFX::SFX_Bet2);
                break;

            case 50:
                bet = 25;  
                playSFX(MusicSFX::SFX_Bet3);
                break;

            case 100:
                bet = 50;  
                playSFX(MusicSFX::SFX_Bet4);
                break;

        }

    }


    // Button Left - Card select left

    if (currentRound == Rounds::Deal && (justPressed & LEFT_BUTTON)) {

        cardSelect--;
        if (cardSelect < 1) {
            cardSelect = 1;
        }

    }


    // Button Right - Card select right

    if (currentRound == Rounds::Deal && (justPressed & RIGHT_BUTTON)) {
    
        cardSelect++;
        if (cardSelect > 5) {
            cardSelect = 5;
        }

    }


    // Button A - Hold / Discard Card

    if (currentRound == Rounds::Deal && (justPressed & A_BUTTON)) {

        if (cardSelect > 0 && Hand[cardSelect - 1] != NULL) {

            if (Hand[cardSelect - 1]->state == HELD) {

                Hand[cardSelect - 1]->state = DRAWN;
                playSFX(MusicSFX::SFX_Bet1);

            } 
            else {

                Hand[cardSelect - 1]->state = HELD;
                playSFX(MusicSFX::SFX_Bet2);

            }

        }

    }

    // Button Up - Discard Card

    if (currentRound == Rounds::Deal && (justPressed & UP_BUTTON)) {

        if (cardSelect > 0 && Hand[cardSelect - 1] != NULL) {

            Hand[cardSelect - 1]->state = HELD;
            playSFX(MusicSFX::SFX_Bet2);

        }

    }

    // Button Up - Hold Card

    if (currentRound == Rounds::Deal && (justPressed & DOWN_BUTTON)) {

        if (cardSelect > 0 && Hand[cardSelect - 1] != NULL) {

            Hand[cardSelect - 1]->state = DRAWN;
            playSFX(MusicSFX::SFX_Bet1);

        }

    }

    // Button B - Deal / Draw
    if (justPressed & B_BUTTON) {

        hintTimer = 0;
        winTimer = 0;
        
        if (currentRound == Rounds::Bet) {

            clearHand();
            cookie.bank -= bet;
            lastBet = bet;
            dealCards();
            currentRound = Rounds::Deal;
            playSFX(MusicSFX::SFX_DoubleBeep);

        } 
        else if (currentRound == Rounds::Deal) {

            currentRound = Rounds::Draw;
            cardSelect = 1;
            drawCards();
            playSFX(MusicSFX::SFX_DoubleBeepDown);

        }

    }

    if (currentRound == Rounds::Bet && justPressed & A_BUTTON) {

        gameState = GameState::Pause;
        pauseMenu = 0;
        
    }

}

void updateRound() {

    if (cookie.bank < 5 && currentRound == Rounds::Bet) {

        gameState = GameState::GameOver;
        playSFX(MusicSFX::SFX_Death, true);

    }

    if (bet > cookie.bank) {
        bet = 5;
    }

    if (currentRound == Rounds::Draw) {
        lockInput = true;

        if (Hand[0]->flipTimer == 0 && Hand[1]->flipTimer == 0 &&
            Hand[2]->flipTimer == 0 && Hand[3]->flipTimer == 0 &&
            Hand[4]->flipTimer == 0) {
            lockInput = false;
            currentRound = Rounds::Bet;
        }
    }

    if (currentRound == Rounds::Deal) {

        lockInput = true;

        if (Hand[0]->flipTimer == 0 && Hand[1]->flipTimer == 0 &&
            Hand[2]->flipTimer == 0 && Hand[3]->flipTimer == 0 &&
            Hand[4]->flipTimer == 0) {
            lockInput = false;
        }
    }
}

void displayCards(uint8_t currentPlane) {

    #if defined(DEBUG) && defined(DEBUG_RENDER_HAND)
    DEBUG_PRINT("displayCards ");
    #endif

    bool hasFlippedCard = false;

    for (int i = 0; i < 5; i++) {

        if (Hand[i] != NULL) {

            if (Hand[i]->flipTimer > 0) {

                Hand[i]->flipTimer--;
                SpritesU::drawOverwriteFX(Constants::CardXPosNormal + (i * Constants::CardSpacing), Constants::CardYPosNormal, Images::Cards_Back, ((i == 4) * 3) + currentPlane);

                if (Hand[i]->flipTimer == 1) { 
                    // playSFX(MusicSFX::SFX_EnemyBlip, true);
                } 

                hasFlippedCard = true;
                continue;

            }

            #if defined(DEBUG) && defined(DEBUG_RENDER_HAND)
            DEBUG_PRINT(Hand[i]->suit);
            DEBUG_PRINT(" ");
            DEBUG_PRINT(Hand[i]->value);
            DEBUG_PRINT("=");
            DEBUG_PRINT(((static_cast<uint8_t>(Hand[i]->suit) * 13) + (Hand[i]->value - 1)) * 3);
            DEBUG_PRINT(", ");
            #endif

            if (Hand[i]->state == HELD) {
                SpritesU::drawOverwriteFX(Constants::CardXPosNormal + (i * Constants::CardSpacing), Constants::CardYPosNormal - Constants::CardYPosHeld, Images::Cards, (((static_cast<uint8_t>(Hand[i]->suit) * 13) + (Hand[i]->value - 2)) * 3) + currentPlane);
            }
            else {
                SpritesU::drawOverwriteFX(Constants::CardXPosNormal + (i * Constants::CardSpacing), Constants::CardYPosNormal, Images::Cards, (((static_cast<uint8_t>(Hand[i]->suit) * 13) + (Hand[i]->value - 2)) * 3) + currentPlane);
            }


            if (cardSelect - 1 == i && frameCount % 32 < 16) {
                SpritesU::drawPlusMaskFX(Constants::CardXPosNormal + (i * Constants::CardSpacing), Constants::CardYPosNormal + (Hand[i]->state == HELD ? -Constants::CardYPosHeld : 0), Images::Cards_Highlight, currentPlane);
            }

        } 
        else {

            SpritesU::drawOverwriteFX(Constants::CardXPosNormal + (i * Constants::CardSpacing), Constants::CardYPosNormal, Images::Cards_Back, ((i == 4) * 3) + currentPlane);

        }

    }

    if (!hasFlippedCard && cardSelect == 0 && currentRound != Rounds::Bet) {
        cardSelect = 1;
    }

    #if defined(DEBUG) && defined(DEBUG_RENDER_HAND)
    DEBUG_PRINTLN("");
    #endif

}

void displayWin_RenderBanner(uint8_t currentPlane, uint8_t width) {

    SpritesU::drawPlusMaskFX(64 - (width / 2) - 2, 29, Images::Frame_LH, currentPlane);
    SpritesU::drawPlusMaskFX(64 - 45 + (width / 2) + 3, 29, Images::Frame_RH, currentPlane);

    font4x6.setCursor(67 - (width / 2), 32);

}

void displayWin(uint8_t currentPlane) {

    cardSelect = 0;
    displayCards(currentPlane);

    if (winTimer < Constants::WinFlashTime_End) {

        winTimer++;
        if (winTimer < Constants::WinFlashTime_Start) {
            return;
        }

        if (winTimer == Constants::WinFlashTime_Start) {
            frameCount = 0;
        }

    }

    if (handResult != HandResult::Loser) {

        switch (winTimer) {

            case Constants::WinFlashTime_Start:
                playSFX(MusicSFX::SFX_Three_Inc);
                launchParticles(64, 32);
                break;

            case Constants::WinFlashTime_Start + 75:
            case Constants::WinFlashTime_Start + 150:
            case Constants::WinFlashTime_Start + 225:
            case Constants::WinFlashTime_Start + 300:
                launchParticles(64, 32);
                break;

            default:
                break;
        }
    
    }
    else {
        
        if (winTimer == Constants::WinFlashTime_Start) {
            playSFX(MusicSFX::SFX_Three_Dec);
        } 

    }
    
    if ((frameCount % 72 < 36) && winTimer < Constants::WinFlashTime_End) {

        switch(handResult) {

            case HandResult::JacksOrBetter:
                displayWin_RenderBanner(currentPlane, 79);
                font4x6.println(F("Jacks or better!"));
                break;

            case HandResult::TwoPair:
                displayWin_RenderBanner(currentPlane, 46);
                font4x6.println(F("Two pair!"));
                break;

            case HandResult::ThreeOfAKind:
                displayWin_RenderBanner(currentPlane, 77);
                font4x6.println(F("Three of a kind!"));
                break;

            case HandResult::Straight:
                displayWin_RenderBanner(currentPlane, 46);
                font4x6.println(F("Straight!"));
                break;

            case HandResult::Flush:
                displayWin_RenderBanner(currentPlane, 46);
                font4x6.println(F("  Flush!"));
                break;

            case HandResult::FullHouse:
                displayWin_RenderBanner(currentPlane, 56);
                font4x6.println(F("Full House!"));
                break;

            case HandResult::FourOfAKind:
                displayWin_RenderBanner(currentPlane, 72);
                font4x6.println(F("Four of a kind!"));
                break;

            case HandResult::StraightFlush:
                displayWin_RenderBanner(currentPlane, 75);
                font4x6.println(F("Straight flush!"));
                break;

            case HandResult::RoyalFlush:
                displayWin_RenderBanner(currentPlane, 63);
                font4x6.println(F("Royal flush!"));
                break;

            case HandResult::Loser:
                displayWin_RenderBanner(currentPlane, 46);
                font4x6.println(F("You lose!"));
                break;
        }

    }

}

void updateDisplay(uint8_t currentPlane) {


    if (handResult == HandResult::NoHand) {

        displayCards(currentPlane);

    }
    else {

        displayWin(currentPlane);

    }


    if (hintTimer < 254 && currentRound == Rounds::Bet) {
    
        hintTimer++;                        
        font4x6.setCursor(1, 0);
        font4x6.print(F("Up/Down:Bet B:Deal A:Pause"));

    }
    else {

        if (currentRound == Rounds::Bet) {

            uint8_t width = 0;

            switch (cookie.bank) {

                case 0 ... 9:
                    width = 1;
                    break;

                case 10 ... 99:
                    width = 2;
                    break;

                case 100 ... 999:
                    width = 3;
                    break;

                case 1000 ... 9999:
                    width = 4;
                    break;

                case 10000 ... 99999:
                    width = 5;
                    break;

                case 100000 ... 999999:
                    width = 6;
                    break;
                    
            }

            switch (bet) {

                case 0 ... 9:
                    width = width + 1;
                    break;

                case 10 ... 99:
                    width = width + 2;
                    break;

                case 100:
                    width = width + 3;
                    break;
                    
            }

            font4x6.setCursor(1, 0);
            font4x6.print(F("Bank:" ));  
            font4x6.print(cookie.bank);  
            font4x6.print(F(", Bet:" ));  
            font4x6.print(bet);  


            // Render Up / Down arrows ..

            if (frameCount % 64 < 32) {

                SpritesU::drawOverwriteFX(55 + (width * 5), 0, Images::ArrowUpDown, currentPlane);

            }

        } 
        else if (currentRound == Rounds::Deal) {

            font4x6.setCursor(1, 0);
            font4x6.print(F("Bet:" ));  
            font4x6.print(bet);  
            font4x6.print(F("  A:Discard  B:Draw"));

        }

    }

}


// ######################################################
//    Card Functions
// ######################################################

// Initializes the CardDeck[]
void initCardDeck() {

    byte suit = 0;

    for (int i = 0; i < 52; i++) {

        CardDeck[i].suit = static_cast<CardSuits>(suit);
        CardDeck[i].value = 2 + (i % 13);
        CardDeck[i].state = INDECK;
        CardDeck[i].flipTimer = 0;

        if (2 + (i % 13) == 14) {
            
            suit++;

        }

    }

}

// Clears the Hand[] and puts cards back in deck
void clearHand() {

    cardSelect = 0;
    winTimer = 0;
    handResult = HandResult::NoHand;

    for (uint8_t i = 0; i < 5; i++) {
        Hand[i] = NULL;
    }

    for (uint8_t j = 0; j < 52; j++) {
        CardDeck[j].state = INDECK;
        CardDeck[j].flipTimer = 0;
    }

}

Card* pickRandomCard() {

    while (true) {

        uint8_t card = random(52);

        if (CardDeck[card].state == INDECK) {

            CardDeck[card].state = DRAWN;
            return &CardDeck[card];

        } 

    }

    return NULL;
}

void dealCards() {

    cardSelect = 0;

    for (uint8_t i = 0; i < 5; i++) {
        Hand[i] = pickRandomCard();
        Hand[i]->flipTimer = (1 + i) * Constants::FlipTime;
    }

}

void drawCards() {

    uint8_t drawCtr = 1;
    cardSelect = 0; 

    for (uint8_t i = 0; i < 5; i++) {

        if (Hand[i]->state == HELD) {
            Hand[i]->state = DISCARDED;
            Hand[i] = pickRandomCard();
            Hand[i]->flipTimer = drawCtr * Constants::FlipTime;
            drawCtr++;
        }

    }

    uint8_t bank_old = cookie.bank;
    handResult = checkHand();

    if (cookie.bank != bank_old) {
        saveCookie(true);
    }

}

void sortCards() { 
    
    qsort(HandValue, 5, sizeof(uint8_t), valueCompare); 
    
}

int valueCompare(const void* a, const void* b) { 
    
    return (*(uint8_t*)a - *(uint8_t*)b); 
    
}

void startGame() {

    if (cookie.bank < 5) {
        cookie.bank = 100;
        saveCookie(true);
    }


    bet = 5;
    lastBet = 0;
    lockInput = false;
    handResult = HandResult::NoHand;
    winTimer = 0;

    initCardDeck();
    clearHand();
    currentRound = Rounds::Bet;

}
