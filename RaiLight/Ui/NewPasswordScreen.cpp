#include "Ui\NewPasswordScreen.h"

#include "Crypto\PasswordHasher.h"

#include "Controllers\ICore.h"

NewPasswordScreen::NewPasswordScreen(rail::control::ICore* _coreController, const QString& _seed) : coreController(_coreController), seed(_seed)
{
    ui.setupUi(this);

    connect(ui.confirmButton, SIGNAL(clicked()), this, SLOT(onConfirmClicked()));
    connect(ui.firstPasswordTextBox, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(onLineEditChanged(int, int)));
    connect(ui.secondPasswordTextbox, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(onLineEditChanged(int, int)));
}


void NewPasswordScreen::onConfirmClicked()
{
    if (ui.firstPasswordTextBox->text() == ui.secondPasswordTextbox->text())
    {
        rail::CryptoUtils::hashAndStorePassord(coreController->getDatabase(), ui.firstPasswordTextBox->text().toStdString());
        emit onPasswordMatch(seed);
    }
}

void NewPasswordScreen::onLineEditChanged(int, int)
{
    QString textColor = "red";
    const auto firstPassword = ui.firstPasswordTextBox->text();
    const auto secondPassword = ui.secondPasswordTextbox->text();

    if (!firstPassword.isEmpty() && firstPassword == secondPassword)
    {
        textColor = "green";
    }

    const QString firstStyleSheet = "QLabel#firstMatchLabel{\n color:" + textColor + "\n}";
    const QString secondSyleSheet = "QLabel#secondMatchLabel{\n color:" + textColor + "\n}";

    ui.firstMatchLabel->setStyleSheet(firstStyleSheet);
    ui.secondMatchLabel->setStyleSheet(secondSyleSheet);
}
