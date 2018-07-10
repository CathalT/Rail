#pragma once

#include "RaiLight\Crypto\SecureTypes.h"

#include "RaiLight\Model\BasicTypes.h"

#include <memory>
#include <optional>
#include <shared_mutex>

namespace rail
{
    class MemoryFan;
    
    namespace control
    {
        class ICore;
    }

    class SecretsStore
    {
    public:
        SecretsStore(control::ICore* _coreController, ByteArray32& _generatedKey, const std::string& _seed);
        ~SecretsStore();

        void setSeed(const std::string& seed);
        void setSeed(ByteArray32& seed);

        void setPassword(const std::string& password);

        bool isSeedSet();

        MemoryFan* getGeneratedKey() const;
        MemoryFan* getPasswordKey() const;

        std::optional<ByteArray32> getSeed();

    private:
        control::ICore *           coreController;

        std::unique_ptr<MemoryFan> generatedKey;
        std::unique_ptr<MemoryFan> passwordKey;

        std::unique_ptr< SecureContainer< ByteArray32 > > secureSeed;

        std::shared_mutex ssMutex;
    };
}
