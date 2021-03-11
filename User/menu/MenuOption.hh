#pragma once
#include "utils/AllSignals.hh"
#include "menu/config_inline/IConfigInline.hh"

namespace menu
{

class MenuPage;

using OptionCallbackType = void(*)(utils::AllSignals&);

enum class OptionType
{
    Page, // just next page of menu
    ConfigInline, // ability to change inline in page (buttons controll)
    ConfigCallback // changes or something else on new page (buttons controll)
};

enum class EnterType
{
    Changing, // we start configuring this option
    Saved // saving changes
};

class MenuOption
{
public:
    MenuOption();
    // for Page
    MenuOption(const char* name, OptionType type, MenuPage* page);
    // for ConfigInline
    MenuOption(const char* name, OptionType type, config_inline::IConfigInline* configObj);
    // for ConfigCallback
    MenuOption(const char* name, OptionType type, OptionCallbackType callback);
    
    void onEnter();
    void onButtonUp();
    void onButtonDown();

    void getTextToDisplay(char* text) const;
    OptionType getType() const;
    MenuPage* getPage();
    OptionCallbackType getCallback();

private:
    void checkDependencies();

    const char* mName;
    OptionType mType;
    EnterType mEnterType = EnterType::Saved;

    MenuPage* mPage = nullptr;
    OptionCallbackType mCallback = nullptr;
    config_inline::IConfigInline* mConfigObj = nullptr;
};

}