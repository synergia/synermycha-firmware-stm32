#pragma once

#include "GFX_BW.h"
#include "OLED_SSD1306.h"
#include "i2c.h"
#include "static/fonts.h"
#include "static/synermycha-logo.h"
#include "utils/AllSignals.hh"
#include "utils/Observer.hh"

namespace display
{

// max 4 rows on display (to adjust)
// and max 20 characters
constexpr int maxRows    = 4;
constexpr int maxColumns = 20;

extern char displayBuff[maxRows][maxColumns];

class Display : public utils::Observer
{
  public:
    Display(utils::AllSignals& signals, I2C_HandleTypeDef* hi2c, const uint8_t* logo, const uint8_t* font,
            int fontSize);

    void clear();
    void show();
    void writeLine(int nr, char* line);
    void ShowDisplayBuffor();

  private:
    utils::AllSignals& mAllSignals;
    I2C_HandleTypeDef* mHi2c;
};

}  // namespace display