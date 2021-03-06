#pragma once

namespace rail
{
    class Marshaller;
    class IWorkLoop;
    class SecretsStore;
    class IDbWrapper;

    namespace endpoint
    {
        class IEndpoint;
    }

    namespace blocks
    {
        class IBlockStore;
    }

    namespace control
    {
        class IBank;

        class ICore
        {
        public:
            virtual endpoint::IEndpoint* getEndpoint() = 0;
            virtual IBank* getBank() = 0;
            virtual Marshaller* getMarshaller() = 0;
            virtual IWorkLoop* getWorkLoop() = 0;
            virtual IDbWrapper* getDatabase() = 0;
            virtual SecretsStore * getSecretsStore() = 0;

            virtual ~ICore() = default;
        };
    }
}
