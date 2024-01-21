#include "characterwindow.h"
#include "ui_characterwindow.h"

CharacterWindow::CharacterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CharacterWindow)
{
    ui->setupUi(this);
}

CharacterWindow::~CharacterWindow()
{
    delete ui;
}
