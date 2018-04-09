#pragma once

#include "Crypto\SecureData.h"


class SecureString : public SecureData<std::string>
{
public:
    SecureString(std::string& inputStr);
    ~SecureString();
    std::string& getString();
private:
    std::string visibleString;
};
