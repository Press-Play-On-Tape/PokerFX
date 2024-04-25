#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/entities/Entities.h"
#include "src/utils/SpritesU.hpp"


void gameOver_Update() {

    uint8_t justPressed = getJustPressedButtons();

    if (justPressed & A_BUTTON) {

        gameState = GameState::Title_Init;

    }
      
}

void gameOver(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {
    
    if (a.needsUpdate()) gameOver_Update();

    uint8_t currentPlane = a.currentPlane();
    render(currentPlane);
    SpritesU::drawOverwriteFX(32, 16, Images::GameOver, currentPlane);
    
}
