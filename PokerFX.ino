#define ABG_IMPLEMENTATION
#define ABG_TIMER1
#define ABG_SYNC_PARK_ROW
#define SPRITESU_IMPLEMENTATION

#include <ArduboyFX.h>  
#include <FixedPointsCommon.h>
#include "src/utils/ArduboyG.h"
#include "src/utils/Enums.h"
#include "src/utils/Constants.h"
#include "fxdata/fxdata.h"
#include "fxdata/images/Images.h"
#include "src/entities/Entities.h"
#include "src/utils/Random.h"

#define SPRITESU_OVERWRITE
#define SPRITESU_PLUSMASK
#define SPRITESU_RECT
#define SPRITESU_FX

#include "src/utils/SpritesU.hpp"

extern ArduboyGBase_Config<ABG_Mode::L4_Triplane> a;
decltype(a) a;

#define SYNTHU_IMPLEMENTATION
#define SYNTHU_NUM_CHANNELS 4
#define SYNTHU_UPDATE_EVERY_N_FRAMES 3
#define SYNTHU_ENABLE_SFX 1
#define SYNTHU_FX_READDATABYTES_FUNC FX::readDataBytes
#include "src/utils/SynthU.hpp"
#define ABG_TIMER1

#include "src/fonts/Font4x6.h"

Cookie cookie;
SoundSettings &soundSettings = cookie.soundSettings;

Particle particles[Constants::ParticlesMax];
bool particlesNeedRendering = false;
GameState gameState = GameState::SplashScreen_Start;
uint16_t frameCount = 0;
uint8_t titleCounter = 0;


Card CardDeck[52]; // global array of cards
Card *Hand[5]; // current cards in play
uint8_t HandValue[5]; // int array of hand values

int cardSelect;
int bet;
int lastBet;

uint16_t winTimer;
uint8_t pauseMenu = 0;
Rounds currentRound;
boolean lockInput;

Font4x6 font4x6 = Font4x6();
HandResult handResult = HandResult::NoHand;
uint8_t hintTimer = 0;

void setup() {

    a.boot();
    // abg_detail::send_cmds_prog<0xDB, 0x20>();
    a.startGray();
    
    FX::begin(FX_DATA_PAGE, FX_SAVE_PAGE);
    FX::loadGameState((uint8_t*)&cookie, sizeof(cookie));

    #ifndef DEBUG_SOUNDS
    audioInit();
    setAudioOn();
    #endif

}


void loop() {

    FX::enableOLED();
    a.waitForNextPlane(BLACK);
    FX::disableOLED();
    
    switch (gameState) {

        case GameState::SplashScreen_Start ... GameState::SplashScreen_End:
            splashScreen(a);
            break;

        case GameState::Title_Init:
            title_Init();
            [[fallthrough]];

        case GameState::Title_Start ... GameState::Title_End:
            title(a);
            break;

        case GameState::Play_Init:
            play_Init();
            [[fallthrough]];

        case GameState::Play_Start ... GameState::Play_End:
            play(a);
            break;

        case GameState::Pause:
            pause(a);
            break;

        case GameState::GameOver:
            gameOver(a);
            break;
    }

    #ifndef DEBUG_SOUNDS
    audioUpdate();
    #endif

}

