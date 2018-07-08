#include "Controllers\Core.h"

#include "Controllers\IBank.h"
#include "Controllers\Bank.h"

#include "Endpoints\IEndpoint.h"
#include "Endpoints\HttpClient.h"

#include "Utilities\WorkLoop.h"
#include "Utilities\Conversions.h"
#include "Utilities\MemoryFan.h"

#include "Database\RailDb.h"

#include "Model\Marshaller.h"
#include "Crypto\SecretsStore.h"

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
            database(std::make_unique<RailDb>())
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

        WorkLoop* Core::getWorkLoop()
        {
            return workLoop.get();
        }

        Marshaller * Core::getMarshaller()
        {
            return marshaller.get();
        }

        RailDb * Core::getDatabase()
        {
            return database.get();
        }

        SecretsStore * Core::getSecretsStore()
        {
            return secretsStore.get();
        }
    }
}
