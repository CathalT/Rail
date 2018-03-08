#include "Controllers\Core.h"

#include "Controllers\IBank.h"
#include "Controllers\Bank.h"

#include "Endpoints\IEndpoint.h"
#include "Endpoints\HttpClient.h"

#include "Model\IBlockStore.h"
#include "Model\BlockStore.h"

#include "Utilities\WorkLoop.h"
#include "Utilities\Conversions.h"

#include "Database\RailDb.h"


#include "Model\Marshaller.h"

#include <QMessageLogger>
#include <QDebug>

namespace rail
{
    namespace control
    {
        Core::~Core() = default;

        Core::Core(const std::string& _seed, const std::string& _nodeIp) :
            workLoop(std::make_unique<WorkLoop>()),   
            bank(std::make_unique<Bank>(this)),
            marshaller(std::make_unique<Marshaller>(this)),
            endpoint(std::make_unique<endpoint::RaiRCPClient>(_nodeIp, this)),
            database(std::make_unique<RailDb>())
            
        {
            extern std::string secret_key;
            bank->initWithSeed(_seed);
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
    }
}
