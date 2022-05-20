#include "Display.hh"

namespace display
{

char displayBuff[maxRows][maxColumns];
char displayMazeBuff[5][maxColumns];

void clearDisplayBuff()
{
    for (int i = 0; i < display::maxRows; i++)
        for (int j = 0; j < display::maxColumns; j++)
            display::displayBuff[i][j] = ' ';
}

void clearDisplayBuffLine(const uint32_t line)
{
    if (line >= display::maxRows)
        return;
    for (int j = 0; j < display::maxColumns; j++)
        display::displayBuff[line][j] = ' ';
}

Display::Display(utils::AllSignals& signals, I2C_HandleTypeDef* hi2c, const uint8_t* logo, const uint8_t* font,
                 int fontSize)
    : mAllSignals(signals)
    , mHi2c(hi2c)
{
    SSD1306_I2cInit(hi2c);
    SSD1306_Bitmap(const_cast<uint8_t*>(logo));
    GFX_SetFont(font);
    GFX_SetFontSize(1);

    mAllSignals.displayBuffor.connect<Display, &Display::showDisplayBuffor>(*this);
    mAllSignals.displayBuffReadyPararell.connect<Display, &Display::displayBuffReadyPararell>(*this);
    mAllSignals.displayBuffPararell.connect<Display, &Display::showDisplayBufforPararell>(*this);
    mAllSignals.displayLogValue.connect<Display, &Display::logValue>(*this);
    mAllSignals.displayTextLine.connect<Display, &Display::displayTextLine>(*this);
    mAllSignals.showMaze.connect<Display, &Display::showMaze>(*this);
}

void Display::clear()
{
    SSD1306_Clear(BLACK);
}

void Display::show()
{
    SSD1306_Display();
}

void Display::writeLine(int nr, const char* line)
{
    while (mHi2c->hdmatx->State != HAL_DMA_STATE_READY)
        ;
    GFX_DrawString(0, (nr + 2) * 9 - 1, const_cast<char*>(line), WHITE, BLACK);
}

void Display::displayTextLine(int nr, const char* text)
{
    writeLine(nr, text);
    show();
}

void Display::showDisplayBuffor()
{
    clear();
    for (int i = 0; i < maxRows; i++)
    {
        writeLine(i, displayBuff[i]);
    }
    show();
}

void Display::showMaze()
{
    clear();
    for (int i = 0; i < 5; i++)
    {
        writeLine(i, displayMazeBuff[i]);
    }
    show();
}

void Display::displayBuffReadyPararell()
{
    if (not mIsDisplayShowing)
    {
        mIsDisplayBuffReady = true;
    }
}

void Display::showDisplayBufforPararell()
{
    if (mIsDisplayBuffReady)
    {
        mIsDisplayShowing = true;
        showDisplayBuffor();
        mIsDisplayBuffReady = false;
        mIsDisplayShowing   = false;
    }
}

void Display::logValue(const char* text, double val, uint8_t line, bool show)
{
    if (line > 3)
        return;
    display::clearDisplayBuffLine(line);
    snprintf(display::displayBuff[line], display::maxColumns, text, val);

    if (show)
        displayBuffReadyPararell();
}

}  // namespace display