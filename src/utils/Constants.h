#pragma once
#include "../../fxdata/fxdata.h"
#include "../../fxdata/images/Images.h"

#define DEBUG_PRINT    Serial.print
#define DEBUG_PRINTLN  Serial.println

#define _DEBUG
#define _DEBUG_RENDER_HAND


namespace Music {

    constexpr uint24_t Songs[] = { Music::Song_MainTheme };

    constexpr uint24_t SFXs[] =  { Music::SFX_Death, 
                                   Music::SFX_Victory,
                                   Music::SFX_XPGain,
                                   Music::SFX_Three_Inc,
                                   Music::SFX_Three_Dec,
                                   Music::SFX_PlayerBlip,
                                   Music::SFX_EnemyBlip,
                                   Music::SFX_Explosion,
                                   Music::SFX_DoubleBeep,
                                   Music::SFX_DoubleBeepDown,
                                   Music::SFX_Bet1,
                                   Music::SFX_Bet2,
                                   Music::SFX_Bet3,
                                   Music::SFX_Bet4,
                                   Music::SFX_Bet5,
                                   Music::SFX_Bet6,
                                };

}

namespace Constants {

    constexpr uint8_t CardSpacing = 22;
    constexpr uint8_t CardXPosNormal = 2;
    constexpr uint8_t CardYPosNormal = 13;
    constexpr uint8_t CardYPosHeld = 4;
    constexpr uint8_t ParticlesMax = 25;
    constexpr uint8_t FlipTime = 25;
    constexpr uint8_t WinFlashTime_Start = 120;
    constexpr uint16_t WinFlashTime_End = 800;

// #define FLIPTIME 25
// #define WINFLASHTIME_START 120
// #define WINFLASHTIME_END 800

};

typedef struct CARD_S {

  CardSuits suit;       // CLUBS, DIAMONDS, HEARTS, or SPADES
  uint8_t value;        // 2-10, J (11), Q (12), K (13), or A (14)
  uint8_t state;        // INDECK, DRAWN, HELD, or DISCARDED
  uint8_t flipTimer;    // Timer for flipping card

} Card;