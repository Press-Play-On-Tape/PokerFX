#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/entities/Entities.h"
#include "src/utils/SpritesU.hpp"

uint8_t getJustPressedButtons() {

    a.pollButtons();

    return a.justPressedButtons();

}

uint8_t getPressedButtons() {

    return a.pressedButtons();

}

void saveCookie(bool enableLEDs) {

    #ifdef USE_LED
    if (enableLEDs) {
        #ifndef MICROCADE
            a.setRGBled(RED_LED, 32);
        #else
            a.setRGBledGreenOff();
            a.setRGBledBlueOff();
        #endif
    }
    #endif

    FX::saveGameState(cookie);

    #ifdef USE_LED
    if (enableLEDs) {
        #ifndef MICROCADE
            a.setRGBled(RED_LED, 0);
            a.setRGBled(GREEN_LED, 32);
        #else
            a.setRGBledRedOff();
            a.setRGBledGreenOn();
        #endif
    }
    #endif

}

void deactivateLEDs() {

    #ifdef USE_LED
        #ifndef MICROCADE
            a.setRGBled(0, 0, 0);
        #else
            a.setRGBledRedOff();
            a.setRGBledGreenOff();
            a.setRGBledBlueOff();
        #endif
    #endif

}


HandResult checkHand() {

    for (int i = 0; i < 5; i++) {
        HandValue[i] = Hand[i]->value;
    }

    #if defined(DEBUG) && defined(DEBUG_RENDER_HAND)
    DEBUG_PRINT(Hand[i]->suit);
    DEBUG_PRINT(" ");
    DEBUG_PRINT(Hand[i]->value);
    DEBUG_PRINT("=");
    DEBUG_PRINT(((static_cast<uint8_t>(Hand[i]->suit) * 13) + (Hand[i]->value - 1)) * 3);
    DEBUG_PRINT(", ");
    #endif

    sortCards();

    if (isRoyalFlush()) {
        cookie.bank += (250 * lastBet);
        return HandResult::RoyalFlush;
    }

    if (isStraightFlush()) {
        cookie.bank += (50 * lastBet);
        return HandResult::StraightFlush;
    }

    if (isFourOfAKind()) {
        cookie.bank += (25 * lastBet);
        return HandResult::FourOfAKind;
    }

    if (isFullHouse()) {
        cookie.bank += (9 * lastBet);
        return HandResult::FullHouse;
    }

    if (isFlush()) {
        cookie.bank += (6 * lastBet);
        return HandResult::Flush;
    }

    if (isStraight()) {
        cookie.bank += (4 * lastBet);
        return HandResult::Straight;
    }

    if (isThreeOfAKind()) {
        cookie.bank += (3 * lastBet);
        return HandResult::ThreeOfAKind;
    }

    if (isTwoPair()) {
        cookie.bank += (2 * lastBet);
        return HandResult::TwoPair;
    }

    if (isJacksOrBetter()) {
        cookie.bank += lastBet;
        return HandResult::JacksOrBetter;
    }

    winTimer = 0;
    return HandResult::Loser;

}

bool isJacksOrBetter() {

    for (int i = 0; i < 4; i++) {
        if (HandValue[i] == HandValue[i + 1] && HandValue[i] >= JACK) return true;
    }

    return false;
}

bool isTwoPair() {

    if ((HandValue[0] == HandValue[1] && HandValue[2] == HandValue[3]) ||
        (HandValue[1] == HandValue[2] && HandValue[3] == HandValue[4]) ||
        (HandValue[0] == HandValue[1] && HandValue[3] == HandValue[4]))
        return true;

    return false;
}

bool isThreeOfAKind() {

    for (int i = 0; i < 3; ++i) {
        
        if ((HandValue[i] == HandValue[i + 1]) && 
            (HandValue[i + 1] == HandValue[i + 2])) {
            return true;
        }

    }

    return false;

}

bool isStraight() {

    if (HandValue[4] == HandValue[3] + 1 &&
        HandValue[3] + 1 == HandValue[2] + 2 &&
        HandValue[2] + 2 == HandValue[1] + 3 &&
        HandValue[1] + 3 == HandValue[0] + 4)
        return true;

    if (HandValue[4] == ACE && HandValue[0] == 2 && HandValue[1] == 3 &&
        HandValue[2] == 4 && HandValue[3] == 5)
        return true;

    return false;

}

bool isFlush() {

    if (Hand[0]->suit == Hand[1]->suit && Hand[1]->suit == Hand[2]->suit &&
        Hand[2]->suit == Hand[3]->suit && Hand[3]->suit == Hand[4]->suit)
        return true;

    return false;

}

bool isFullHouse() {

    if ((HandValue[0] == HandValue[1] && HandValue[1] == HandValue[2] &&
         HandValue[3] == HandValue[4]) ||
        (HandValue[0] == HandValue[1] && HandValue[2] == HandValue[3] &&
         HandValue[3] == HandValue[4]))
        return true;

    return false;

}

bool isFourOfAKind() {

    if ((HandValue[0] == HandValue[1] && HandValue[1] == HandValue[2] &&
         HandValue[2] == HandValue[3]) ||
        (HandValue[1] == HandValue[2] && HandValue[2] == HandValue[3] &&
         HandValue[3] == HandValue[4]))
        return true;

    return false;

}

bool isStraightFlush() {

    if (isStraight() && isFlush()) return true;
    
    return false;

}

bool isRoyalFlush() {

    if (isFlush() && HandValue[0] == 10 && HandValue[1] == JACK &&
        HandValue[2] == QUEEN && HandValue[3] == KING && HandValue[4] == ACE) {
        return true;
    }

    return false;

}
