#include "MenuPage.hh"
#include "display/Display.hh"
#include <algorithm>
#include <cstring>

namespace menu
{

void MenuPage::AddOption(MenuOption option)
{
    if (mNumberOfOptions < maxNrOfOptions)
    {
        mOptions[mNumberOfOptions] = option;
        ++mNumberOfOptions;
    }
}

void MenuPage::shiftToNextOption()
{
    if (mChoosenOption + 1 < mNumberOfOptions)
    {
        ++mChoosenOption;
    }
}

void MenuPage::shiftToPreviousOption()
{
    if (mChoosenOption > 0)
    {
        --mChoosenOption;
    }
}

void MenuPage::passEnterToChoosenOption()
{
    mOptions[mChoosenOption].onEnter();
}

void MenuPage::passButtonUpToChoosenOption()
{
    mOptions[mChoosenOption].onButtonUp();
}

void MenuPage::passButtonDownToChoosenOption()
{
    mOptions[mChoosenOption].onButtonDown();
}

OptionType MenuPage::getTypeOfChoosenOption() const
{
    return mOptions[mChoosenOption].getType();
}

MenuPage* MenuPage::getPageOfChoosenOption()
{
    return mOptions[mChoosenOption].getPage();
}

OptionCallbackType MenuPage::getCallbackOfChoosenOption()
{
    return mOptions[mChoosenOption].getCallback();
}

void MenuPage::setDisplayPromt(bool promt)
{
    mDisplayPromt = promt;
}

void MenuPage::prepareMenuPageForDisplay()
{
    for (int i = 0; i < display::maxRows; i++)
        for (int j = 0; j < display::maxColumns; j++)
            display::displayBuff[i][j] = ' ';

    constexpr int startPosColumn = 3;
    int startPosRow              = 0;

    startPosRow = std::max(0, mChoosenOption + 1 - display::maxRows);
    startPosRow = std::min(startPosRow, mNumberOfOptions - display::maxRows);
    startPosRow = std::max(startPosRow, 0);

    char text[display::maxColumns];
    for (int i = 0; i < display::maxRows && i < mNumberOfOptions; i++)
    {
        memset(text, 0, sizeof(text));
        mOptions[startPosRow].getTextToDisplay(text);
        if (startPosRow == mChoosenOption and mDisplayPromt)
        {
            sprintf(display::displayBuff[i], "> %s", text);
        }
        else
        {
            sprintf(display::displayBuff[i], "  %s", text);
        }
        startPosRow++;
    }
}

}  // namespace menu