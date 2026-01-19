#include "board.h"
#include "codecs/no_audio_codec.h"
#include "display/oled_display.h" 
#include "application.h"
#include "button.h"
#include "config.h"
#include "assets/lang_config.h"
#include <esp_log.h>
#include <driver/i2c_master.h>

class CompactNt26Board : public Board {
private:
    Display* display_ = nullptr;
    Button boot_button_;
    i2c_master_bus_handle_t i2c_bus_;

public:
    CompactNt26Board() : boot_button_(BOOT_BUTTON_GPIO) {
        // --- 1. I2C ESTÁNDAR (D4 y D5) ---
        i2c_master_bus_config_t bus_config = {
            .i2c_port = (i2c_port_t)0,
            .sda_io_num = (gpio_num_t)5, // Pin D4
            .scl_io_num = (gpio_num_t)6, // Pin D5
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
        };
        ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus_));

        // --- 2. DISPLAY OLED 0.96 ---
        display_ = new OledDisplay(i2c_bus_, 128, 64, false, false);

        // --- 3. BOTÓN ---
        boot_button_.OnClick([]() {
            Application::GetInstance().ToggleChatState();
        });
    }

    virtual AudioCodec* GetAudioCodec() override {
        // Audio I2S en D7, D8, D9 | Micrófono Sense en 42, 41, 38
        static NoAudioCodecSimplex audio_codec(16000, 16000, 7, 8, 44, 42, 41, 38);
        return &audio_codec;
    }

    virtual Display* GetDisplay() override { return display_; }
};

DECLARE_BOARD(CompactNt26Board);
