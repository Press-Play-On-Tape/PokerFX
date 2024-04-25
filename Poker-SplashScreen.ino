#include <Arduboy2.h>


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void splashScreen_Update() { 

    frameCount++; 
    if (titleCounter < 32) titleCounter++;   

    uint8_t justPressed = getJustPressedButtons();

    if (justPressed > 0 && titleCounter == 32) {
        
        Arduboy2::initRandomSeed();
        gameState = GameState::Title_Init; 

    }

}


void splashScreen(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {
    
    uint8_t currentPlane = a.currentPlane();
    uint8_t idx = static_cast<uint8_t>(gameState) - static_cast<uint8_t>(GameState::SplashScreen_Start);

    SpritesU::drawOverwriteFX(0, 0, Images::PPOT, (3 * idx) + currentPlane);

    if (frameCount == 12) {

        idx = (idx + 1) % 4;
        frameCount = 0;
        gameState = static_cast<GameState>(static_cast<uint8_t>(GameState::SplashScreen_Start) + idx);

    }

    if (a.needsUpdate()) splashScreen_Update();

}
