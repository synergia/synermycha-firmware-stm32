#include "MenuOption.hh"
#include "utils/FatalError.hh"
#include <cstdio>

namespace menu
{

MenuOption::MenuOption()
{

}

MenuOption::MenuOption(const char* name, OptionType type, MenuPage* page)
    : mName(name),
      mType(type),
      mPage(page)
{
    checkDependencies();
}

MenuOption::MenuOption(const char* name, OptionType type, config_inline::IConfigInline* configObj)
    : mName(name),
      mType(type),
      mConfigObj(configObj)
{
    checkDependencies();
}

MenuOption::MenuOption(const char* name, OptionType type, OptionCallbackType callback)
    : mName(name),
      mType(type),
      mCallback(callback)
{
    checkDependencies();
}

void MenuOption::onEnter()
{
    mEnterType = (mEnterType == EnterType::Changing) ? EnterType::Saved : EnterType::Changing;
}

void MenuOption::onButtonUp()
{
    if (mEnterType == EnterType::Changing)
    {
        if (mType == OptionType::ConfigInline)
        {
            mConfigObj->handleButtonUp();
        }
    }
}

void MenuOption::onButtonDown()
{
    if (mEnterType == EnterType::Changing)
    {
        if (mType == OptionType::ConfigInline)
        {
            mConfigObj->handleButtonDown();
        }
    }
}

void MenuOption::getTextToDisplay(char* text) const
{
    if (mType == OptionType::Page)
    {
        sprintf(text, "%s", mName);
    }
    else if (mType == OptionType::ConfigInline)
    {
        bool prompt = mEnterType == EnterType::Changing;
        mConfigObj->fillLabel(text, 20, prompt);
    }
    else if (mType == OptionType::ConfigCallback)
    {
        sprintf(text, "%s-callback", mName);
    }
}

OptionType MenuOption::getType() const
{
    return mType;
}

MenuPage* MenuOption::getPage()
{
    return mPage;
}

OptionCallbackType MenuOption::getCallback()
{
    return mCallback;
}

void MenuOption::checkDependencies()
{
    if (mType == OptionType::Page)
    {
        if (mPage == nullptr)
        {
            utils::FatalError("MenuOption with Page type requires valid mPage pointer!");
        }
    }
    else if (mType == OptionType::ConfigInline)
    {
        if (mConfigObj == nullptr)
        {
            utils::FatalError("MenuOption with ConfigInline type requires valid mConfigObj pointer!");
        }
    }
    else if (mType == OptionType::ConfigCallback)
    {
        if (mCallback == nullptr)
        {
            utils::FatalError("MenuOption with ConfigCallback type requires valid mCallback pointer!");
        }
    }
}

}