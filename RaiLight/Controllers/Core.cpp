#include "RaiLight\Controllers\Core.h"

#include "RaiLight\Controllers\IBank.h"
#include "RaiLight\Controllers\Bank.h"

#include "RaiLight\Endpoints\IEndpoint.h"
#include "RaiLight\Endpoints\HttpClient.h"

#include "RaiLight\Utilities\IWorkLoop.h"
#include "RaiLight\Utilities\WorkLoop.h"
#include "RaiLight\Utilities\Conversions.h"
#include "RaiLight\Utilities\MemoryFan.h"

#include "RaiLight\Database\IDbWrapper.h"
#include "RaiLight\Database\DbWrapper.h"

#include "RaiLight\Model\Marshaller.h"
#include "RaiLight\Crypto\SecretsStore.h"

#include <QMessageLogger>
#include <QDebug>

namespace rail
{
    namespace control
    {
        Core::~Core() = default;

        Core::Core(const std::string& _nodeIp, const std::string& _listenPort, const std::string& _seed) :
            workLoop(std::make_unique<WorkLoop>()),   
            bank(std::make_unique<Bank>(this)),
            marshaller(std::make_unique<Marshaller>(this)),
            endpoint(std::make_unique<endpoint::RaiRCPClient>(_nodeIp, _listenPort, this)),
            database(std::make_unique<DbWrapper>())
        {
            extern ByteArray32 secret_key;
            secretsStore = std::make_unique<SecretsStore>(this, secret_key, _seed);
        }

        IBank * Core::getBank()
        {
            return bank.get();
        }

        endpoint::IEndpoint * Core::getEndpoint()
        {
            return endpoint.get();
        }

        IWorkLoop* Core::getWorkLoop()
        {
            return workLoop.get();
        }

        Marshaller * Core::getMarshaller()
        {
            return marshaller.get();
        }

        IDbWrapper * Core::getDatabase()
        {
            return database.get();
        }

        SecretsStore * Core::getSecretsStore()
        {
            return secretsStore.get();
        }
    }
}
