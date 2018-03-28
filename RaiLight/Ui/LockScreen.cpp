#include "Ui\LockScreen.h"

#include "Controllers\ICore.h"
#include "Crypto\PasswordVerifier.h"

#include <QMessageBox>

using namespace rail::control;

LockScreen::LockScreen(ICore* _coreController) :
    coreController(_coreController)
{
    ui.setupUi(this);

    connect(ui.backButton, SIGNAL(clicked()), this, SIGNAL(backButtonClicked()));
    connect(ui.unlockButton, SIGNAL(clicked()), this, SLOT(onPasswordEntered()));
    connect(this, SIGNAL(passwordWrong()), this, SLOT(wrongPasswordEntered()));
}

void LockScreen::onPasswordEntered()
{
    const auto password = ui.passwordBox->text().toStdString();
    if (!password.empty() && rail::PasswordVerifier(coreController, password).isValid())
    {
        emit passwordCorrect();
    }
    else
    {
        emit passwordWrong();
    }
}

void LockScreen::wrongPasswordEntered()
{
    auto errorMsg = new QMessageBox(QMessageBox::Icon::Warning, "Sorry", "Wrong Password!");
    errorMsg->show();
    connect(errorMsg, &QMessageBox::buttonClicked, [errorMsg]() 
    { 
        errorMsg->deleteLater(); 
    });
}