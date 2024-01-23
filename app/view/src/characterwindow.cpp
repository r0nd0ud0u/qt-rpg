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

void CharacterWindow::InitWindow(){
    ui->tabWidget->setCurrentIndex(0);
    ui->edit_atk_tab->InitView();
}

void CharacterWindow::on_pushButton_clicked()
{
    hide();
    ui->edit_atk_tab->Save();
}

