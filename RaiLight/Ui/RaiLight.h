#pragma once

#include <QtWidgets\QMainWindow>
#include "ui_RaiLight.h"

#include <memory>

namespace rail::endpoint
{
    class IEndpoint;
}

namespace rail::control
{
    class ICore;
}

class QStringListModel;

class RaiLight : public QMainWindow
{
    Q_OBJECT

public:
    RaiLight(rail::control::ICore *_coreController);
    ~RaiLight();

public slots:
    void onSendButtonPressed();
    void onAccountIndexChanged(const QString & text);
    void onBalanceUpdated(const QString& balance, const QString& accountId);
    void onPendingBalanceUpdated(const QString& pendingBalance, const QString& accountId);
    void onAccountsFinishedSyncing();
    void onTransactionsUpdated(const QString& accountId);


private:
    Ui::RaiLightClass ui;
    QStringListModel* model{ nullptr };

    rail::control::ICore *coreController;
};