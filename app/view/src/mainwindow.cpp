#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "hostpage.h"
#include "Stylizer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Stylizer::ApplyTheme(this);

    connect(ui->page, &HostPage::showGameDisplay, this, &MainWindow::ShowPageGameDisplay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowPageGameDisplay(){
    ui->stackedWidget->setCurrentIndex(1);
}
