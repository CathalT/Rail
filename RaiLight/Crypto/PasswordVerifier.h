#pragma once

#include <string>

namespace rail
{
    namespace control
    {
        class ICore;
    }

    class PasswordVerifier
    {
    public:
        PasswordVerifier(rail::control::ICore* _coreController, const std::string& password);
        bool isValid() const;
    private:
        bool valid{ false };
    };
}
