#pragma once

#include "IConfigInline.hh"
#include <cstdio>

namespace menu {
namespace config_inline {

template <typename T>
class ConfigInlineSingleValue : public IConfigInline
{
public:
    ConfigInlineSingleValue(const char* name, T value, T step = 1);
    virtual ~ConfigInlineSingleValue() = default;

    virtual void handleButtonUp();
    virtual void handleButtonDown();
    virtual void fillLabel(char* text, int maxLen, bool prompt);

private:
    const char* mName;
    T mValue;
    T mStep;
};

template<typename T>
ConfigInlineSingleValue<T>::ConfigInlineSingleValue(const char* name, T value, T step)
    : mName(name),
      mValue(value),
      mStep(step)
{
}

template<typename T>
void ConfigInlineSingleValue<T>::handleButtonUp()
{
    mValue += mStep;
}

template<typename T>
void ConfigInlineSingleValue<T>::handleButtonDown()
{
    mValue -= mStep;
}

template<typename T>
void ConfigInlineSingleValue<T>::fillLabel(char* text, int maxLen, bool prompt)
{
    if (prompt)
    {
        sprintf(text, "%s    >%d", mName, mValue);
    }
    else
    {
        sprintf(text, "%s     %d", mName, mValue);
    }
}

/////////////// bool specialization ///////////////////
template<>
void ConfigInlineSingleValue<bool>::handleButtonUp()
{
    mValue = !mValue;
}

template<>
void ConfigInlineSingleValue<bool>::handleButtonDown()
{
    mValue = !mValue;
}

template <>
void ConfigInlineSingleValue<bool>::fillLabel(char* text, int maxLen, bool prompt)
{
    if (prompt)
    {
        sprintf(text, "%s    >%s", mName, mValue ? "true" : "false");
    }
    else
    {
        sprintf(text, "%s     %s", mName, mValue ? "true" : "false");
    }
}
////////////////////////////////////////////////////////

/////////////// double/float specialization ////////////
template <>
void ConfigInlineSingleValue<double>::fillLabel(char* text, int maxLen, bool prompt)
{
    if (prompt)
    {
        sprintf(text, "%s    >%f", mName, mValue);
    }
    else
    {
        sprintf(text, "%s     %f", mName, mValue);
    }
}

template <>
void ConfigInlineSingleValue<float>::fillLabel(char* text, int maxLen, bool prompt)
{
    if (prompt)
    {
        sprintf(text, "%s    >%f", mName, mValue);
    }
    else
    {
        sprintf(text, "%s     %f", mName, mValue);
    }
}
////////////////////////////////////////////////////////

}
}