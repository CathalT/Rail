#include "Endpoints\Webserver.h"

#include "Controllers\ICore.h"
#include "Model\Marshaller.h"

#include "Utilities\Conversions.h"

#include <cpprest\http_listener.h>
#include <cpprest\http_msg.h>

#include <QMessageLogger>
#include <QDebug>

#define U(x) _XPLATSTR(x)

static constexpr auto LOCAL_ADDRESS = U("http://127.0.0.1:");
static constexpr auto DEFAULT_LISTEN_PORT = U("8080");

using namespace web;
using namespace http;

namespace rail
{
    namespace endpoint
    {
        Webserver::Webserver(control::ICore* _coreController, const std::string& _listenPort) : coreController(_coreController), started(false)
        {
            utility::string_t listenAddress;
            if (_listenPort.empty())
            {
                listenAddress = LOCAL_ADDRESS + utility::string_t(DEFAULT_LISTEN_PORT);
            }
            else
            {
                listenAddress = LOCAL_ADDRESS + Conversions::toUtilString(_listenPort);
            }

            webserver = std::make_unique<web::http::experimental::listener::http_listener>(listenAddress);
            webserver->support(methods::POST, std::bind(&Webserver::handlePost, this, std::placeholders::_1));
        }

        void Webserver::start()
        {
            if (!started)
            {
                try
                {
                    started = true;
                    webserver->open().wait();
                }
                catch (const std::exception& e)
                {
                    QMessageLogger().critical() << "Exception thrown starting webserver: " << e.what();
                    started = false;
                }
            }
        }

        Webserver::~Webserver()
        {
            if (started)
            {
                webserver->close().wait();
            }
        }

        void Webserver::handlePost(const http_request& message)
        {
            try
            {
                message.reply(status_codes::OK);
                const auto jsonOut = message.extract_json().get();
                coreController->getMarshaller()->parseCallbackResponse(jsonOut);
            }
            catch (const std::exception& e)
            {
                QMessageLogger().warning() << "Exception thrown handling post data: " << e.what();
            }
        }
    }
}

