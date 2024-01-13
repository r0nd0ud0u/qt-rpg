#include "gamedisplay.h"
#include "ui_gamedisplay.h"

GameDisplay::GameDisplay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameDisplay)
{
    ui->setupUi(this);
}

GameDisplay::~GameDisplay()
{
    delete ui;
}
