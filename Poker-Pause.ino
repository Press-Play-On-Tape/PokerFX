#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/entities/Entities.h"
#include "src/utils/SpritesU.hpp"


void pause_Update() {

    uint8_t justPressed = getJustPressedButtons();

    if (justPressed & UP_BUTTON) {

        pauseMenu = 0;

    }

    if (justPressed & DOWN_BUTTON) {

        pauseMenu = 1;

    }

    if (justPressed & A_BUTTON) {

        if (pauseMenu == 0) {
            gameState = GameState::Title_Init;
        }
        else {
            gameState = GameState::Play;
        }

    }

    if (justPressed & B_BUTTON) {

        gameState = GameState::Play;

    }
      
}

void pause(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {
    
    if (a.needsUpdate()) pause_Update();

    uint8_t currentPlane = a.currentPlane();
    render(currentPlane);
    SpritesU::drawOverwriteFX(32, 16, Images::Pause, (3 * pauseMenu) + currentPlane);
    
}
