#pragma once

enum class MusicSong : uint8_t {
    MainTheme
};

enum class MusicSFX : uint8_t {
    SFX_Death,
    SFX_Victory,
    SFX_XPGain,
    SFX_Three_Inc,
    SFX_Three_Dec,
    SFX_PlayerBlip,
    SFX_EnemyBlip,
    SFX_Explosion,
    SFX_DoubleBeep,
    SFX_DoubleBeepDown,
    SFX_Bet1,
    SFX_Bet2,
    SFX_Bet3,
    SFX_Bet4,
    SFX_Bet5,
    SFX_Bet6,
};

enum class GameState : uint8_t {

    SplashScreen_Start,
        SplashScreen_00 = SplashScreen_Start,
        SplashScreen_01,
        SplashScreen_02,
        SplashScreen_03,
    SplashScreen_End,

    Title_Init,
    Title_Start,
        Title_OptPlay = Title_Start,
        Title_OptSound,
        Title_OptCredits,
        Title_OptResume,
        Title_OptPlay2,
        Title_OptSound2,
        Title_OptSound_Music,
        Title_OptSound_SFX,
        Title_OptSound_Volume,
        Title_OptSound_Music2,
        Title_OptSound_SFX2,
        Title_OptSound_Volume2,
        Title_ShowCredits,
    Title_End,

    Play_Init,
    Play_Start,
        Play = Play_Start,
    Play_End,
    Pause,
    GameOver,

};


enum class CardSuits : uint8_t { 
    Clubs = 0, 
    Diamonds = 1, 
    Hearts = 2, 
    Spades = 3, 
};

enum class Rounds : uint8_t { 
    Bet = 0, 
    Deal = 1, 
    Draw = 3,
};

enum CARDSTATES { INDECK = 0, DRAWN = 1, HELD = 2, DISCARDED = 3 };
enum FACECARDS { JACK = 11, QUEEN = 12, KING = 13, ACE = 14 };

enum class HandResult : uint8_t {
    JacksOrBetter,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush,
    RoyalFlush,
    NoHand,
    Loser
};