#include "Display.hh"

namespace display
{

char displayBuff[maxRows][maxColumns];

Display::Display(utils::AllSignals& signals, I2C_HandleTypeDef* hi2c, const uint8_t* logo, const uint8_t* font,
                 int fontSize)
    : mAllSignals(signals)
    , mHi2c(hi2c)
{
    SSD1306_I2cInit(hi2c);
    SSD1306_Bitmap(const_cast<uint8_t*>(logo));
    GFX_SetFont(font);
    GFX_SetFontSize(1);

    mAllSignals.displayBuffor.connect<Display, &Display::ShowDisplayBuffor>(*this);
}

void Display::clear()
{
    SSD1306_Clear(BLACK);
}

void Display::show()
{
    SSD1306_Display();
}

void Display::writeLine(int nr, char* line)
{
    while (mHi2c->hdmatx->State != HAL_DMA_STATE_READY)
        ;
    GFX_DrawString(0, (nr + 2) * 9 - 1, const_cast<char*>(line), WHITE, BLACK);
}

void Display::ShowDisplayBuffor()
{
    clear();
    for (int i = 0; i < maxRows; i++)
    {
        writeLine(i, displayBuff[i]);
    }
    show();
}

}  // namespace display