#include "Menu.hh"

namespace menu
{

Menu::Menu(utils::AllSignals& sig)
    : mSignals{sig}
    , mState(MenuState::Normal)
{
    sig.buttonUp.connect<Menu, &Menu::onButtonUpPressed>(*this);
    sig.buttonDown.connect<Menu, &Menu::onButtonDownPressed>(*this);
    sig.buttonEnter.connect<Menu, &Menu::onEnterPressed>(*this);
}

void Menu::setDefaultMenuPage(MenuPage* page)
{
    mActualPage = page;
    if (isFirstRun)
    {
        mActualPage->prepareMenuPageForDisplay();
        mSignals.displayBuffor.emit();
        isFirstRun = false;
    }
}

void Menu::onButtonDownPressed()
{
    if (mState == MenuState::Normal)
    {
        mActualPage->shiftToNextOption();
        mActualPage->prepareMenuPageForDisplay();
        mSignals.displayBuffor.emit();
    }
    else  // MenuState::Changing
    {
        mActualPage->passButtonDownToChoosenOption();
        // ConfigCallback is drawing by its own
        if (mActualPage->getTypeOfChoosenOption() not_eq OptionType::ConfigCallback)
        {
            mActualPage->prepareMenuPageForDisplay();
            mSignals.displayBuffor.emit();
        }
    }
}

void Menu::onButtonUpPressed()
{
    if (mState == MenuState::Normal)
    {
        mActualPage->shiftToPreviousOption();
        mActualPage->prepareMenuPageForDisplay();
        mSignals.displayBuffor.emit();
    }
    else  // MenuState::Changing
    {
        mActualPage->passButtonUpToChoosenOption();
        // ConfigCallback is drawing by its own
        if (mActualPage->getTypeOfChoosenOption() not_eq OptionType::ConfigCallback)
        {
            mActualPage->prepareMenuPageForDisplay();
            mSignals.displayBuffor.emit();
        }
    }
}

void Menu::onEnterPressed()
{
    OptionType optionType = mActualPage->getTypeOfChoosenOption();
    if (mState == MenuState::Normal)
    {
        if (optionType == OptionType::Page)
        {
            auto page = mActualPage->getPageOfChoosenOption();
            if (page != nullptr)
            {
                setDefaultMenuPage(page);
            }
        }
        else if (optionType == OptionType::ConfigInline)
        {
            mActualPage->passEnterToChoosenOption();
            mActualPage->setDisplayPromt(false);
            mState = MenuState::Changing;
        }
    }
    else  // MenuState::Changing
    {
        if (optionType == OptionType::ConfigInline)
        {
            mActualPage->passEnterToChoosenOption();
            mActualPage->setDisplayPromt(true);
            mState = MenuState::Normal;
        }
    }
    mActualPage->prepareMenuPageForDisplay();
    mSignals.displayBuffor.emit();
}

}  // namespace menu