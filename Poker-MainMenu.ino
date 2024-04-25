#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/entities/Entities.h"
#include "src/utils/SpritesU.hpp"

void title_Init() {

    if (cookie.bank == 100 || cookie.bank == 0) {
        gameState = GameState::Title_OptPlay;
    }
    else {
        gameState = GameState::Title_OptResume;
    }

}

void title_Update() {
// Serial.print((uint8_t)bank);
// Serial.print(" ");
// Serial.println((uint8_t)gameState);
    frameCount++;

    uint8_t justPressed = getJustPressedButtons();
    uint8_t pressed = getPressedButtons();

    switch (gameState) {

        #ifndef DEBUG        

            case GameState::Title_ShowCredits:

                if (justPressed & B_BUTTON || justPressed & A_BUTTON) {

                    gameState = GameState::Title_OptCredits;

                }

                break;

        #endif
        
        case GameState::Title_OptPlay:

            if (justPressed & DOWN_BUTTON) {
                gameState = GameState::Title_OptSound;
            }

            if (justPressed & A_BUTTON) {
                
                cookie.bank = 100;
                saveCookie(true);
                gameState = GameState::Play_Init;

            }

            break;
        
        case GameState::Title_OptResume:

            if (justPressed & DOWN_BUTTON) {
                gameState = GameState::Title_OptPlay2;
            }

            if (justPressed & A_BUTTON) {

                gameState = GameState::Play_Init;

            }

            break;
          
        case GameState::Title_OptPlay2:

            if (justPressed & UP_BUTTON) {
                gameState = GameState::Title_OptResume;
            }

            if (justPressed & DOWN_BUTTON) {
                gameState = GameState::Title_OptSound2;
            }

            if (justPressed & A_BUTTON) {
                
                cookie.bank = 100;
                saveCookie(true);
                gameState = GameState::Play_Init;

            }

            break;

        case GameState::Title_OptSound:

            if (justPressed & UP_BUTTON) {
                gameState = GameState::Title_OptPlay;
            }

            if (justPressed & DOWN_BUTTON) {
                gameState = GameState::Title_OptCredits;
            }

            if (justPressed & A_BUTTON) {

                gameState = GameState::Title_OptSound_Music;

            }

            break;

        case GameState::Title_OptSound2:

            if (justPressed & UP_BUTTON) {
                gameState = GameState::Title_OptPlay2;
            }

            if (justPressed & DOWN_BUTTON) {
                gameState = GameState::Title_OptCredits;
            }

            if (justPressed & A_BUTTON) {

                gameState = GameState::Title_OptSound_Music2;

            }

            break;
        
        case GameState::Title_OptCredits:

            if (justPressed & UP_BUTTON) {
                gameState = GameState::Title_OptSound;
            }

            if (justPressed & A_BUTTON) {
                gameState = GameState::Title_ShowCredits;
            }

            break;
                
        case GameState::Title_OptSound_Music:
        case GameState::Title_OptSound_Music2:
            
            if (justPressed & A_BUTTON) {

                soundSettings.setMusic(!soundSettings.getMusic());
                if (soundSettings.getMusic()) {
                    playMusic();
                }
                else {
                    SynthU::stop();
                }

            }
            
            if (justPressed & B_BUTTON) {

                switch (gameState) {

                    case GameState::Title_OptSound_Music:
                        gameState = GameState::Title_OptSound;
                        break;

                    case GameState::Title_OptSound_Music2:
                        gameState = GameState::Title_OptSound2;
                        break;
            
                }

            }
            
            if (justPressed & DOWN_BUTTON) {

                gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) + 1);

            }

            break;
        
        case GameState::Title_OptSound_SFX:
        case GameState::Title_OptSound_SFX2:
            
            if (justPressed & A_BUTTON) {

                soundSettings.setSFX(!soundSettings.getSFX());

            }
            
            if (justPressed & B_BUTTON) {

                switch (gameState) {

                    case GameState::Title_OptSound_SFX:
                        gameState = GameState::Title_OptSound;
                        break;

                    case GameState::Title_OptSound_SFX2:
                        gameState = GameState::Title_OptSound2;
                        break;
            
                }

            }
            
            if (justPressed & UP_BUTTON) {

                gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) - 1);

            }
            
            if (justPressed & DOWN_BUTTON) {

                gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) + 1);

            }

            break;

        case GameState::Title_OptSound_Volume:
        case GameState::Title_OptSound_Volume2:
            
            if (justPressed & LEFT_BUTTON) {

                if (soundSettings.getVolume() > 0) {

                    soundSettings.setVolume(soundSettings.getVolume() - 1);
                    SynthU::setVolume(soundSettings.getVolume());

                }

            }
            
            if (justPressed & RIGHT_BUTTON) {

                if (soundSettings.getVolume() < 7) {

                    soundSettings.setVolume(soundSettings.getVolume() + 1);
                    SynthU::setVolume(soundSettings.getVolume());

                }

            }

            if (justPressed & B_BUTTON) {

                switch (gameState) {

                    case GameState::Title_OptSound_Volume:
                        gameState = GameState::Title_OptSound;
                        break;

                    case GameState::Title_OptSound_Volume2:
                        gameState = GameState::Title_OptSound2;
                        break;
            
                }

                saveCookie(true);

            }
            
            if (justPressed & UP_BUTTON) {

                gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) - 1);

            }

            break;
        
    }

    if (titleCounter < 72) titleCounter++;
      
}

void title(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {
    
    if (a.needsUpdate()) title_Update();

    uint8_t currentPlane = a.currentPlane();

    SpritesU::drawOverwriteFX(0, 0, Images::Title_Background, currentPlane);
    SpritesU::drawPlusMaskFX(80, 23, Images::Title, (3 * (static_cast<uint8_t>(gameState) - 6)) + currentPlane);

    switch (gameState) {
        
        case GameState::Title_OptSound_Music ... GameState::Title_OptSound_Volume:
        case GameState::Title_OptSound_Music2 ... GameState::Title_OptSound_Volume2:
            {

                if (soundSettings.getMusic())    SpritesU::drawPlusMaskFX(88, 26, Images::Sound_Checkbox, currentPlane);
                if (soundSettings.getSFX())      SpritesU::drawPlusMaskFX(88, 36, Images::Sound_Checkbox, currentPlane);

                uint8_t volume = (soundSettings.getMusic() || soundSettings.getSFX()) ? soundSettings.getVolume() : 0;

                if (soundSettings.getMusic() || soundSettings.getSFX()) {
                    SpritesU::drawPlusMaskFX(89, 45, Images::Sound_Volume, (soundSettings.getVolume() * 3) + currentPlane);
                }
                else {
                    SpritesU::drawPlusMaskFX(89, 45, Images::Sound_Volume, currentPlane);
                }

            }

            break;     
  
        default:
            break;    

    }

}
