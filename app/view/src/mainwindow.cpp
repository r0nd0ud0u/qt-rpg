#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "hostpage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->page, &HostPage::showGameDisplay, this, &MainWindow::ShowPageGameDisplay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowPageGameDisplay(){
    ui->stackedWidget->setCurrentIndex(1);
}
