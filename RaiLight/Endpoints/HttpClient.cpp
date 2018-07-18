#include "RaiLight\Endpoints\HttpClient.h"

#include "RaiLight\Endpoints\Webserver.h"

#include "RaiLight\Model\EndpointTypes.h"

#include "RaiLight\Model\StateBlock.h"

#include "RaiLight\Controllers\ICore.h"

#include "RaiLight\Model\BasicTypes.h"
#include "RaiLight\Model\Marshaller.h"

#include "RaiLight\Utilities\Conversions.h"

#include "RaiLight\Crypto\CryptoUtils.h"
#include "RaiLight\Crypto\ProofOfWork.h"

#pragma warning( push )
#pragma warning( disable: 4266 )
#include <cpprest\http_client.h>
#pragma warning( pop )

#include <QMessageLogger>
#include <QDebug>

#define U(x) _XPLATSTR(x)

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace rail
{
    namespace endpoint
    {
        RaiRCPClient::~RaiRCPClient() = default;

        RaiRCPClient::RaiRCPClient(const std::string & _endpoint, const std::string & _listenPort, rail::control::ICore* _coreController) :
            httpClient(std::make_unique<http_client>(_endpoint.empty() ? U("http://127.0.0.1") : Conversions::toUtilString(_endpoint))),
            webserver(std::make_unique<Webserver>(_coreController, _listenPort)),
            coreController(_coreController)
        {
        }

        std::string RaiRCPClient::sendStateBlock(blocks::StateBlock& stateBlock)
        {
            const auto previous = Conversions::toUtilString(stateBlock.getPreviousBlock());
            const auto accountId = Conversions::toUtilString(stateBlock.getAddress());
            const auto destinationPublickey = Conversions::toUtilString(stateBlock.getDestinationPublicKey());
            const auto destinationAddress = Conversions::toUtilString(stateBlock.getDestinationAddress());
            const auto representative = Conversions::toUtilString(stateBlock.getRepresentative());
            const auto balance = Conversions::toUtilString(stateBlock.getBalance());
            const auto signature = Conversions::toUtilString(stateBlock.getSignature());
            const auto work = Conversions::toUtilString(stateBlock.getWork());

            utility::stringstream_t ss;
            ss << U("{ \"type\" : \"state\",");
            ss << U("\"account\" : \"") << accountId << U("\",");
            ss << U("\"previous\" : \"") << previous << U("\",");
            ss << U("\"representative\" : \"") << representative << U("\",");
            ss << U("\"balance\" : \"") << balance << U("\",");
            ss << U("\"link\" : \"") << destinationPublickey << U("\",");
            ss << U("\"work\" : \"") << work << U("\",");
            ss << U("\"signature\" : \"") << signature << U("\"}");

            return Conversions::toStdString(processBlockSync(ss.str()));
        }

        AccountStatus RaiRCPClient::getAccountStatusSync(const std::string & account)
        {
            AccountStatus accountStatus;

            http_request req(methods::POST);
            json::value postParameters(json::value::object());

            postParameters[U("action")] = json::value::string(U("account_info"));
            postParameters[U("account")] = json::value::string(rail::Conversions::toUtilString(account));
            postParameters[U("representative")] = json::value::boolean(true);
            postParameters[U("weight")] = json::value::boolean(true);
            postParameters[U("pending")] = json::value::boolean(true);

            req.set_body(postParameters);

            try
            {
                httpClient->request(req).then([&accountStatus](const http_response& response)
                {
                    try
                    {
                        auto statusCode = response.status_code();
                        if (statusCode == status_codes::OK)
                        {
                            const auto jsonTask = response.extract_json();
                            const auto jsonVal = jsonTask.get();

                            if (jsonVal.has_field(U("error")))
                            {
                                const auto errorDesc = jsonVal.at(U("error")).as_string();
                                if (errorDesc == U("Account not found"))
                                {
                                    accountStatus.isValid = false;
                                }
                            }
                            else
                            {
                                accountStatus.isValid = true;

                                if (jsonVal.has_field(U("block_count")))
                                {
                                    const auto blockStr = jsonVal.at(U("block_count")).as_string();
                                    accountStatus.blockCount = std::stoull(blockStr);
                                }

                                if (jsonVal.has_field(U("frontier")))
                                {
                                    accountStatus.frontier = Conversions::toStdString(jsonVal.at(U("frontier")).as_string());
                                }

                                if (jsonVal.has_field(U("open_block")))
                                {
                                    accountStatus.openBlock = Conversions::toStdString(jsonVal.at(U("open_block")).as_string());
                                }

                                if (jsonVal.has_field(U("representative_block")))
                                {
                                    accountStatus.representativeBlock = Conversions::toStdString(jsonVal.at(U("representative_block")).as_string());
                                }

                                if (jsonVal.has_field(U("balance")))
                                {
                                    accountStatus.balance = Conversions::toStdString(jsonVal.at(U("balance")).as_string());
                                }

                                if (jsonVal.has_field(U("modified_timestamp")))
                                {
                                    accountStatus.modifiedTimestamp = Conversions::toStdString(jsonVal.at(U("modified_timestamp")).as_string());
                                }

                                if (jsonVal.has_field(U("representative")))
                                {
                                    accountStatus.representative = Conversions::toStdString(jsonVal.at(U("representative")).as_string());
                                }

                                if (jsonVal.has_field(U("weight")))
                                {
                                    accountStatus.weight = Conversions::toStdString(jsonVal.at(U("weight")).as_string());
                                }

                                if (jsonVal.has_field(U("pending")))
                                {
                                    accountStatus.pending = Conversions::toStdString(jsonVal.at(U("pending")).as_string());
                                }
                            }
                        }
                    }
                    catch (const http_exception& e)
                    {
                        QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
                    }
                    catch (const std::exception& e)
                    {
                        QMessageLogger().warning() << "Caught unhandled std::exception: " << e.what();
                    }
                    catch (...)
                    {
                        QMessageLogger().warning() << "Caught unknown exception.";
                    }
                }).get();
            }
            catch (const http_exception& e)
            {
                QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
            }
            catch (const std::exception& ec)
            {
                accountStatus.error = true;
                QMessageLogger().warning() << ec.what();
            }

            return accountStatus;
        }

        BlockCounts RaiRCPClient::getLedgerBlockCountSync()
        {
            utility::string_t blockCount;
            utility::string_t uncheckedCount;

            json::value postParameters = json::value::object();
            postParameters[U("action")] = json::value::string(U("block_count"));

            http_request req(methods::POST);
            req.set_body(postParameters);

            try
            {
                httpClient->request(req).then([&blockCount, &uncheckedCount](const http_response& response)
                {
                    try
                    {
                        auto statusCode = response.status_code();
                        if (statusCode == status_codes::OK)
                        {
                            const auto jsonTask = response.extract_json();
                            const auto jsonVal = jsonTask.get();
                            blockCount = jsonVal.at(U("count")).as_string();
                            uncheckedCount = jsonVal.at(U("unchecked")).as_string();
                        }
                    }
                    catch (const http_exception& e)
                    {
                        QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
                    }
                    catch (const std::exception& e)
                    {
                        QMessageLogger().warning() << "Caught unhandled std::exception: " << e.what();
                    }
                    catch (...)
                    {
                        QMessageLogger().warning() << "Caught unknown exception.";
                    }
                }).get();
            }
            catch (const http_exception& e)
            {
                QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
            }
            catch (const std::exception& ec)
            {
                QMessageLogger().warning() << ec.what();
            }
            catch (...)
            {
                QMessageLogger().warning() << "Caught unknown exception.";
            }

            return{ Conversions::toStdString(blockCount), Conversions::toStdString(uncheckedCount) };
        }

        bool RaiRCPClient::getAccountBalanceSync(const std::string& address)
        {
            bool success{ false };
            json::value postParameters = json::value::object();
            postParameters[U("action")] = json::value::string(U("account_balance"));
            postParameters[U("account")] = json::value::string(rail::Conversions::toUtilString(address));

            http_request req(methods::POST);
            req.set_body(postParameters);

            try
            {
                httpClient->request(req).then([this, address, &success](Concurrency::task<http_response> responseTask)
                {

                        try
                        {
                            auto response = responseTask.get();
                            auto statusCode = response.status_code();
                            if (statusCode == status_codes::OK)
                            {
                                success = true;
                                const auto jsonTask = response.extract_json();
                                const auto jsonVal = jsonTask.get();
                                coreController->getMarshaller()->parseBalances(jsonVal, address);
                            }
                        }
                        catch (const http_exception& e)
                        {
                            QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
                        }
                        catch (const std::exception& e)
                        {
                            QMessageLogger().warning() << "Caught unhandled std::exception: " << e.what();
                        }
                        catch (...)
                        {
                            QMessageLogger().warning() << "Caught unknown exception.";
                        }
                }).get();
            }
            catch (const http_exception& e)
            {
                QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
            }
            catch (const std::exception& ec)
            {
                QMessageLogger().warning() << ec.what();
            }
            catch (...)
            {
                QMessageLogger().warning() << "Caught unknown exception.";
            }

            return success;
        }

        void RaiRCPClient::startWebServer()
        {
            webserver->start();
        }

        void RaiRCPClient::getPendingBlocks(const std::string& address, const uint32_t count)
        {
            json::value postParameters = json::value::object();
            postParameters[U("action")] = json::value::string(U("pending"));
            postParameters[U("account")] = json::value::string(Conversions::toUtilString(address));
            postParameters[U("count")] = json::value::string(Conversions::numberToUtilString(count));
            postParameters[U("source")] = json::value::boolean(true);

            http_request req(methods::POST);
            req.set_body(postParameters);

            try
            {
                httpClient->request(req).then([this, address](Concurrency::task<http_response> taskResponse)
                {
                    try
                    {
                        const auto response = taskResponse.get();
                        auto statusCode = response.status_code();
                        if (statusCode == status_codes::OK)
                        {
                            const auto jsonTask = response.extract_json();
                            const auto jsonVal = jsonTask.get();
                            if (jsonVal.has_field(U("blocks")))
                            {
                                auto test = jsonVal.serialize(); test;
                                const auto arrayBlocks = jsonVal.at(U("blocks")).as_object();
                                if (arrayBlocks.size() > 0)
                                {
                                    coreController->getMarshaller()->parsePendingBlocks(address, arrayBlocks);
                                }
                            }
                        }
                            
                    }
                    catch (const http_exception& e)
                    {  
                        QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " <<  e.what();
                    }
                    catch (const std::exception& e)
                    {
                        QMessageLogger().warning() << "Caught unhandled std::exception: " << e.what();
                    }
                    catch (...)
                    {
                        QMessageLogger().warning() << "Caught unknown exception.";
                    }
                });
            }
            catch (const http_exception& e)
            {
                QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
            }
            catch (const std::exception& ec)
            {
                QMessageLogger().warning() << ec.what();
            }
            catch (...)
            {
                QMessageLogger().warning() << "Caught unknown exception.";
            }
        }

        bool RaiRCPClient::arePendingBlocksSync(const std::string & address)
        {
            bool blocksPending{ false };
            json::value postParameters = json::value::object();
            postParameters[U("action")] = json::value::string(U("pending"));
            postParameters[U("account")] = json::value::string(Conversions::toUtilString(address));
            postParameters[U("count")] = json::value::string(U("1"));

            http_request req(methods::POST);
            req.set_body(postParameters);

            try
            {
                httpClient->request(req).then([this, &blocksPending](const http_response& response)
                {
                    try
                    {
                        auto statusCode = response.status_code();
                        if (statusCode == status_codes::OK)
                        {
                            const auto jsonTask = response.extract_json();
                            const auto jsonVal = jsonTask.get();
                            if (jsonVal.has_field(U("blocks")))
                            {
                                if (jsonVal.has_array_field(U("blocks")))
                                {
                                    const auto arrayBlocks = jsonVal.at(U("blocks")).as_array();
                                    if (arrayBlocks.size() > 0)
                                    {
                                        blocksPending = true;
                                    }
                                }
                            }
                        }
                    }
                    catch (const http_exception& e)
                    {
                        QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
                    }
                    catch (const std::exception& e)
                    {
                        QMessageLogger().warning() << "Caught unhandled std::exception: " << e.what();
                    }
                    catch (...)
                    {
                        QMessageLogger().warning() << "Caught unknown exception.";
                    }
                }).get();
            }
            catch (const http_exception& e)
            {
                QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
            }
            catch (const std::exception& ec)
            {
                QMessageLogger().warning() << ec.what();
            }
            catch (...)
            {
                QMessageLogger().warning() << "Caught unknown exception.";
            }

            return blocksPending;
        }

        utility::string_t RaiRCPClient::processBlockSync(const utility::string_t& block)
        {
            utility::string_t hash;

            json::value postParameters = json::value::object();
            postParameters[U("action")] = json::value::string(U("process"));
            postParameters[U("block")] = json::value::string(block);

            http_request req(methods::POST);
            req.set_body(postParameters);

            try
            {
                httpClient->request(req).then([&hash](const http_response& response)
                {

                    try
                    {
                        auto statusCode = response.status_code();
                        if (statusCode == status_codes::OK)
                        {
                            const auto jsonTask = response.extract_json();
                            const auto jsonVal = jsonTask.get();
                            hash = jsonVal.at(U("hash")).as_string();
                        }
                    }
                    catch (const http_exception& e)
                    {
                        QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
                    }
                    catch (const std::exception& e)
                    {
                        QMessageLogger().warning() << "Caught unhandled std::exception: " << e.what();
                    }
                    catch (...)
                    {
                        QMessageLogger().warning() << "Caught unknown exception.";
                    }
                }).get();
            }
            catch (const http_exception& e)
            {
                QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
            }
            catch (const std::exception& ec)
            {
                QMessageLogger().warning() << ec.what();
            }
            catch (...)
            {
                QMessageLogger().warning() << "Caught unknown exception.";
            }

            return hash;
        }

        void RaiRCPClient::getAccountHistory(const std::string& address, const uint32_t count)
        {
            json::value postParameters = json::value::object();
            postParameters[U("action")] = json::value::string(U("account_history"));
            postParameters[U("account")] = json::value::string(Conversions::toUtilString(address));
            postParameters[U("count")] = json::value::string(Conversions::numberToUtilString(count));

            http_request req(methods::POST);
            req.set_body(postParameters);

            try
            {
                httpClient->request(req).then([this, address](Concurrency::task<http_response> taskResponse)
                {
                    try
                    {
                        const auto response = taskResponse.get();
                        auto statusCode = response.status_code();
                        if (statusCode == status_codes::OK)
                        {
                            const auto jsonTask = response.extract_json();
                            const auto jsonVal = jsonTask.get();

                            if (jsonVal.has_array_field(U("history")))
                            {
                                const auto history = jsonVal.at(U("history")).as_array();
                                coreController->getMarshaller()->parseAccountHistory(address, history);
                            }
                        }
                    }
                    catch (const http_exception& e)
                    {
                        QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
                    }
                    catch (const std::exception& e)
                    {
                        QMessageLogger().warning() << "Caught unhandled std::exception: " << e.what();
                    }
                    catch (...)
                    {
                        QMessageLogger().warning() << "Caught unknown exception.";
                    }
                });
            }
            catch (const http_exception& e)
            {
                QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
            }
            catch (const std::exception& ec)
            {
                QMessageLogger().warning() << ec.what();
            }
            catch (...)
            {
                QMessageLogger().warning() << "Caught unknown exception.";
            }
        }

        std::string RaiRCPClient::getFrontiersSync(const std::string & address)
        {
            utility::string_t hash;

            const auto jsonAccount = json::value::string(Conversions::toUtilString(address));
            const auto jsonArray = json::value::array({ jsonAccount });

            json::value postParameters = json::value::object();
            postParameters[U("action")] = json::value::string(U("accounts_frontiers"));
            postParameters[U("accounts")] = jsonArray;
            
            http_request req(methods::POST);
            req.set_body(postParameters);

            try
            {
                httpClient->request(req).then([&hash, jsonAccount](const http_response& response)
                {
                    try
                    {
                        auto statusCode = response.status_code();
                        if (statusCode == status_codes::OK)
                        {
                            const auto jsonTask = response.extract_json();
                            const auto jsonVal = jsonTask.get();
                            const auto frontiers = jsonVal.at(U("frontiers"));
                            hash = frontiers.at(jsonAccount.as_string()).as_string();
                        }
                    }
                    catch (const http_exception& e)
                    {
                        QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
                    }
                    catch (const std::exception& e)
                    {
                        QMessageLogger().warning() << "Caught unhandled std::exception: " << e.what();
                    }
                    catch (...)
                    {
                        QMessageLogger().warning() << "Caught unknown exception.";
                    }
                }).get();
            }
            catch (const http_exception& e)
            {
                QMessageLogger().warning() << "Caught http exception, error_code: " << e.error_code().value() << " , what: " << e.what();
            }
            catch (const std::exception& ec)
            {
                QMessageLogger().warning() << ec.what();
            }
            catch (...)
            {
                QMessageLogger().warning() << "Caught unknown exception.";
            }

            return Conversions::toStdString(hash);
        }
    }
}