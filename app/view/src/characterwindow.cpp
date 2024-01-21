#include "characterwindow.h"
#include "ui_characterwindow.h"

CharacterWindow::CharacterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CharacterWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

}

CharacterWindow::~CharacterWindow()
{
    delete ui;
}
