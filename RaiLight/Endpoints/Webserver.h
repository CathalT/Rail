#pragma once

#include "Model\BasicTypes.h"
#include <cpprest\details\basic_types.h>

#include <memory>
#include <atomic>

namespace web::http::experimental::listener
{
    class http_listener;
}

namespace web::http
{
    class http_request;
}

namespace rail
{
    class Marshaller;

    namespace control
    {
        class ICore;
    }

    namespace endpoint
    {
        class Webserver
        {
        public:
            explicit Webserver(control::ICore* _coreController, const std::string& _listenPort);
            void start();
            virtual ~Webserver();

        private:

            void handlePost(const web::http::http_request & message);

        private:
            std::unique_ptr<web::http::experimental::listener::http_listener> webserver;
            rail::control::ICore* coreController;

            std::atomic_bool started;
        };
    }
}
