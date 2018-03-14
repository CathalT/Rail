#include "Ui\MainWindow.h"

//#include "Ui\TransactionsViewModel.h"

#include "Endpoints\IEndpoint.h"

#include "Model\EndpointTypes.h"
#include "Model\Account.h"

#include "Controllers\ICore.h"
#include "Controllers\IBank.h"

#include "Model\Marshaller.h"

#include "Crypto\CryptoUtils.h"

#include <QMessageLogger>
#include <QDebug>
#include <QTimer>
#include <QStringListModel>

using namespace rail;
using namespace rail::control;
using namespace rail::endpoint;

MainWindow::~MainWindow() = default;
MainWindow::MainWindow(ICore* _coreController) :
    coreController(_coreController)
{
    ui.setupUi(this);

    const auto regEx = QRegExp("^xrb\\_[13456789abcdefghijkmnopqrstuwxyz]{60}$");

    QValidator *regExVal = new QRegExpValidator(regEx, this);
    QValidator *validator = new QIntValidator(this);

    model = new QStringListModel(this);
    ui.transactionsListView->setModel(model);

    ui.amountToSendTextbox->setValidator(validator);
    ui.sendToAddressTextbox->setValidator(regExVal);
    ui.sendToAddressTextbox->setMaxLength(64);

    connect(ui.sendButton, SIGNAL(clicked()), this, SLOT(onSendButtonPressed()));
    connect(ui.accountsListComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onAccountIndexChanged(const QString &)));

    auto marshaller = coreController->getMarshaller();
    connect(marshaller, SIGNAL(updateBalance(const QString&, const QString&)), this, SLOT(onBalanceUpdated(const QString&, const QString&)));
    connect(marshaller, SIGNAL(updatePendingBalance(const QString&, const QString&)), this, SLOT(onPendingBalanceUpdated(const QString&, const QString&)));
    connect(marshaller, SIGNAL(accountFinishedSyncing()), this, SLOT(onAccountsFinishedSyncing()));
    connect(marshaller, SIGNAL(transactionsUpdated(const QString&)), this, SLOT(onTransactionsUpdated(const QString&)));

    onAccountsFinishedSyncing();
}

void MainWindow::onSendButtonPressed()
{
    const auto currentAddress = ui.accountsListComboBox->currentText();
    const auto sendTo = ui.sendToAddressTextbox->text();
    const auto amountToSend = ui.amountToSendTextbox->text();
    if (sendTo.length() == 64 && !amountToSend.isEmpty())
    {
        coreController->getBank()->sendToAccount(currentAddress.toStdString(), sendTo.toStdString(), amountToSend.toStdString());
    }
    else
    {
        QMessageLogger().debug() << "Address or amount incorrect.";
    }
}

void MainWindow::onAccountIndexChanged(const QString & text)
{
    const auto account = text.toStdString();

    ui.balanceSumLabel->setText(QString::number(coreController->getBank()->getAccountBalance(account)));
    ui.pendingBalanceSumLabel->setText(QString::number(coreController->getBank()->getAccountPendingBalance(account)));
    onTransactionsUpdated(text);
}

void MainWindow::onBalanceUpdated(const QString & balance, const QString& accountId)
{
    if (accountId == ui.accountsListComboBox->currentText())
    {
        ui.balanceSumLabel->setText(balance);
    }
}

void MainWindow::onPendingBalanceUpdated(const QString & pendingBalance, const QString& accountId)
{
    if (accountId == ui.accountsListComboBox->currentText())
    {
        ui.pendingBalanceSumLabel->setText(pendingBalance);
    }
}

void MainWindow::onAccountsFinishedSyncing()
{
    const auto addresses = coreController->getBank()->getAllAddresses();
    for (const auto a : addresses)
    {
        ui.accountsListComboBox->addItem(QString::fromStdString(a));
    }
}

void MainWindow::onTransactionsUpdated(const QString& accountId)
{
    if (accountId == ui.accountsListComboBox->currentText())
    {
        QStringList strList;
        auto transactions = coreController->getBank()->getTransactionHistory(accountId.toStdString());
        for (const auto& t : transactions)
        {
            strList.push_back(
                (t.direction == Direction::Receive ? QString("Received") : QString("Sent")) + QString(": ") + QString::number(t.amount) +
                (t.direction == Direction::Receive ? QString(" NANO from ") : QString(" NANO to ")) + QString::fromStdString(t.otherAddress));
        }

        model->setStringList(strList);
    }
}
