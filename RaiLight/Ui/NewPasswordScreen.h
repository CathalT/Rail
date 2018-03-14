#pragma once

#include "ui_NewPasswordScreen.h"

#include <QWidget>

class NewPasswordScreen : public QWidget
{
    Q_OBJECT
public:
    NewPasswordScreen();

private:
    Ui::NewPassswordScreen ui;
};
