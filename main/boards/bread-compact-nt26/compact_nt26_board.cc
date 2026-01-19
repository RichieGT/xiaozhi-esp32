#include "board.h"
#include "codecs/no_audio_codec.h"
#include "display/st7789_display.h" // Usaremos este driver que es compatible con la redonda
#include "application.h"
#include "button.h"
#include "config.h"
#include "assets/lang_config.h"
#include <esp_log.h>

#define TAG "BotanicoBoard"

class CompactNt26Board : public Board {
private:
    Display* display_ = nullptr;
    Button boot_button_;

public:
    CompactNt26Board() : boot_button_(BOOT_BUTTON_GPIO) {
        // --- 1. CONFIGURACIÓN DE PANTALLA REDONDA (SPI) ---
        // Pines para XIAO S3: SCLK=D8(7), MOSI=D10(9), CS=D2(3), DC=D3(4), RES=D1(2)
        display_ = new St7789Display(7, 9, 3, 4, 2, 240, 240, false, true); 

        // --- 2. BOTÓN DE INTERACCIÓN (El de la placa XIAO) ---
        boot_button_.OnClick([]() {
            Application::GetInstance().ToggleChatState();
        });
    }

    virtual Led* GetLed() override {
        static SingleLed led(BUILTIN_LED_GPIO);
        return &led;
    }

    virtual AudioCodec* GetAudioCodec() override {
        // Pines Audio XIAO S3: BCLK=D9(8), WS=D0(1), DOUT=D7(44)
        // Micrófono Sense Interno: SCK=42, WS=41, DIN=38
        static NoAudioCodecSimplex audio_codec(16000, 16000, 8, 1, 44, 42, 41, 38);
        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        return display_;
    }
};

DECLARE_BOARD(CompactNt26Board);
