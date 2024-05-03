#include "dotview.h"
#include "ui_dotview.h"

DotView::DotView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DotView)
{
    ui->setupUi(this);
    setStyleSheet(
        "#hot_label{ background:     #00ff00;}");
}

DotView::~DotView()
{
    delete ui;
}
