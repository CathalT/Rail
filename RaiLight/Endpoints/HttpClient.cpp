#include "Endpoints\HttpClient.h"

#include "Endpoints\Webserver.h"

#include "Model\EndpointTypes.h"

#include "Model\OpenBlock.h"
#include "Model\SendBlock.h"
#include "Model\ReceiveBlock.h"
#include "Model\ChangeBlock.h"

#include "Controllers\ICore.h"

#include "Model\BasicTypes.h"
#include "Model\Marshaller.h"

#include "Utilities\Conversions.h"

#include "Crypto\CryptoUtils.h"
#include "Crypto\ProofOfWork.h"

#include <cpprest\http_client.h>

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

        RaiRCPClient::RaiRCPClient(const std::string & _endpoint, rail::control::ICore* _coreController) :
            httpClient(std::make_unique<http_client>(_endpoint.empty() ? U("http://127.0.0.1") : Conversions::toUtilString(_endpoint))),
            webserver(std::make_unique<Webserver>(_coreController)),
            coreController(_coreController)
        {
        }

        std::string RaiRCPClient::send(const blocks::Send& sendBlock)
        {
            const auto previous = Conversions::toUtilString(sendBlock.getPreviousBlock());
            const auto destination = Conversions::toUtilString(sendBlock.getDestinationAddress());
            const auto amountToSend = Conversions::toUtilString(sendBlock.getAmountToSend());
            const auto signature = Conversions::toUtilString(sendBlock.getSignature());
            const auto work = Conversions::toUtilString(Conversions::uint64ToHexStr(Pow::generateWork(sendBlock.getPreviousBlockBytes())));

            utility::stringstream_t ss;
            ss << U("{ \"type\" : \"send\",");
            ss << U("\"previous\" : \"") << previous << U("\",");
            ss << U("\"destination\" : \"") << destination << U("\",");
            ss << U("\"balance\" : \"") << amountToSend << U("\",");
            ss << U("\"work\" : \"") << work << U("\",");
            ss << U("\"signature\" : \"") << signature << U("\"}");

            return Conversions::toStdString(processBlockSync(ss.str()));
        }

        std::string RaiRCPClient::receive(const blocks::Receive& receiveBlock)
        {
            const auto previous = Conversions::toUtilString(receiveBlock.getPreviousBlock());
            const auto source = Conversions::toUtilString(receiveBlock.getSourceBlock());
            const auto signature = Conversions::toUtilString(receiveBlock.getSignature());
            const auto work = Conversions::toUtilString(Conversions::uint64ToHexStr(Pow::generateWork(receiveBlock.getPreviousBlockBytes())));

            utility::stringstream_t ss;
            ss << U("{ \"type\" : \"receive\",");
            ss << U("\"previous\" : \"") << previous << U("\",");
            ss << U("\"source\" : \"") << source << U("\",");
            ss << U("\"work\" : \"") << work << U("\",");
            ss << U("\"signature\" : \"") << signature << U("\"}");

            return Conversions::toStdString(processBlockSync(ss.str()));
        }

        std::string RaiRCPClient::open(const blocks::Open& openBlock)
        {
            const auto address = Conversions::toUtilString(openBlock.getAddress());
            const auto representative = Conversions::toUtilString(openBlock.getRepresentative());
            const auto signature = Conversions::toUtilString(openBlock.getSignature());
            const auto source = Conversions::toUtilString(openBlock.getSourceBlock());
            const auto work = Conversions::toUtilString(Conversions::uint64ToHexStr(Pow::generateWork(openBlock.getPublicKeyBytes())));

            utility::stringstream_t ss;
            ss << U("{ \"type\" : \"open\",");
            ss << U("\"account\" : \"") << address << U("\",");
            ss << U("\"representative\" : \"") << representative << U("\",");
            ss << U("\"source\" : \"") << source << U("\",");
            ss << U("\"work\" : \"") << work << U("\",");
            ss << U("\"signature\" : \"") << signature << U("\"}");

            return Conversions::toStdString(processBlockSync(ss.str()));
        }
           
        std::string RaiRCPClient::change(const blocks::Change& changeBlock)
        {
            const auto previousBlock = Conversions::toUtilString(changeBlock.getPreviousBlock());
            const auto representative = Conversions::toUtilString(changeBlock.getRepresentativeAddress());
            const auto work = Conversions::toUtilString(Conversions::uint64ToHexStr(Pow::generateWork(changeBlock.getPreviousBlockBytes())));
            const auto signature = Conversions::toUtilString(changeBlock.getSignature());

            utility::stringstream_t ss;
            ss << U("{ \"type\" : \"change\",");
            ss << U("\"previous\" : \"") << previousBlock << U("\",");
            ss << U("\"representative\" : \"") << representative << U("\",");
            ss << U("\"work\" : \"") << work << U("\",");
            ss << U("\"signature\" : \"") << signature << U("\"}");

            return Conversions::toStdString(processBlockSync(ss.str()));
        }

        AccountStatus RaiRCPClient::getAccountStatusSync(const std::string & account)
        {
            AccountStatus accountStatus;

            http_request req(methods::POST);
            json::value postParameters(json::value::object());

            postParameters[U("action")] = json::value::string(U("account_block_count"));
            postParameters[U("account")] = json::value::string(rail::Conversions::toUtilString(account));

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

                            if (jsonVal.has_field(U("block_count")))
                            {
                                const auto blockStr = jsonVal.at(U("block_count")).as_string();
                                accountStatus.blockCount = std::stoull(blockStr);
                                accountStatus.isValid = true;
                            }
                            else if (jsonVal.has_field(U("error")))
                            {
                                const auto errorDesc = jsonVal.at(U("error")).as_string();
                                if (errorDesc == U("Account not found"))
                                {
                                    accountStatus.isValid = false;
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
                        QMessageLogger().warning() << "Caught unkown exception.";
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
                        QMessageLogger().warning() << "Caught unkown exception.";
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
                QMessageLogger().warning() << "Caught unkown exception.";
            }

            return{ Conversions::toStdString(blockCount), Conversions::toStdString(uncheckedCount) };
        }

        void RaiRCPClient::getAccountBalance(const std::string& address)
        {
            json::value postParameters = json::value::object();
            postParameters[U("action")] = json::value::string(U("account_balance"));
            postParameters[U("account")] = json::value::string(rail::Conversions::toUtilString(address));

            http_request req(methods::POST);
            req.set_body(postParameters);

            try
            {
                httpClient->request(req).then([this, address](Concurrency::task<http_response> responseTask)
                {

                        try
                        {
                            auto response = responseTask.get();
                            auto statusCode = response.status_code();
                            if (statusCode == status_codes::OK)
                            {
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
                                if (jsonVal.has_array_field(U("blocks")))
                                {
                                    const auto arrayBlocks = jsonVal.at(U("blocks")).as_array();
                                    if (arrayBlocks.size() > 0)
                                    {
                                        coreController->getMarshaller()->parsePendingBlocks(address, arrayBlocks);
                                    }
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

                            if (jsonVal.has_field(U("history")))
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