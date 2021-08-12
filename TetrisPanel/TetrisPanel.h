#pragma once

#include <QtWidgets/QWidget>
#include "ui_TetrisPanel.h"

class TetrisPanel : public QWidget
{
    Q_OBJECT

public:
    TetrisPanel(QWidget *parent = Q_NULLPTR);

private:
    Ui::TetrisPanelClass ui;
};
