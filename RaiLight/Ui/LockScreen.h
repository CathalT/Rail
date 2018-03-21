#pragma once

#include "ui_LockScreen.h"

#include <QWidget>

namespace rail::control
{
    class ICore;
}

class LockScreen : public QWidget
{
    Q_OBJECT
public:
    LockScreen(rail::control::ICore *_coreController);

public slots:
    void onPasswordEntered();
   
signals:
    void backButtonClicked();
    void passwordCorrect();
    void passwordWrong();

private slots:
    void wrongPasswordEntered();

private:
    Ui::LockScreen ui;

    rail::control::ICore *coreController;
};