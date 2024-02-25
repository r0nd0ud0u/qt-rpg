#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "ApplicationView.h"
#include "Stylizer.h"
#include "hostpage.h"

#include "gamedisplay.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  Stylizer::ApplyTheme(this);

  connect(ui->page, &HostPage::showGameDisplay, this,
          &MainWindow::ShowPageGameDisplay);

  connect(this,
          &MainWindow::SigNewCharacter, ui->page_2,
          &GameDisplay::AddNewCharacter);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::ShowPageGameDisplay() {
  ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::AddNewCharacter(Character *ch) {
    emit SigNewCharacter(ch);
}
