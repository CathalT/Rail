#pragma once

#include <QObject>
#include <QString>

namespace web::json
{
    class value;
    class array;
}

namespace rail
{
    namespace control
    {
        class ICore;
    }

    class Marshaller : public QObject
    {
        Q_OBJECT

    public:
        Marshaller(control::ICore* _coreController);
        virtual ~Marshaller();

        void parseBalances(const web::json::value& jsonBlob, const std::string& address);
        void parseCallbackResponse(const web::json::value & jsonBlob);
        void parsePendingBlocks(const std::string& address, const web::json::array& pendingBlocks);
        void parseAccountHistory(const std::string& address, const web::json::array& history);
        void updateUIBalances(const std::string & address, const std::string & balance, const std::string & pendingBalance);
        void notifyAccountsFinishedSyncing();

    signals:
        void updateBalance(const QString& balance, const QString& address);
        void updatePendingBalance(const QString& balance, const QString& address);
        void accountFinishedSyncing();
        void transactionsUpdated(const QString& accountId);

    private:
        control::ICore* coreController;
    };
}

