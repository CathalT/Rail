#pragma once

#include "ui_MainWindow.h"

#include <QWidget>

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

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(rail::control::ICore *_coreController);
    ~MainWindow();

public slots:
    void onSendButtonPressed();
    void onAccountIndexChanged(const QString & text);
    void onBalanceUpdated(const QString& balance, const QString& accountId);
    void onPendingBalanceUpdated(const QString& pendingBalance, const QString& accountId);
    void onAccountsFinishedSyncing();
    void onTransactionsUpdated(const QString& accountId);


private:
    Ui::MainWindow ui;
    QStringListModel* model{ nullptr };

    rail::control::ICore *coreController;
};