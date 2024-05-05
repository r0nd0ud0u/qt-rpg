#include "dotview.h"
#include "ui_dotview.h"

DotView::DotView(QWidget *parent) : QWidget(parent), ui(new Ui::DotView) {
  ui->setupUi(this);
  setStyleSheet(
      "#hot_label{ background:     #00ff00;} #dot_label{ background:     "
      "#ff0000;} #buf_label{ background:     #33a2ff;} #debuf_label{ background: "
      "#ff8d33;}");
}

DotView::~DotView() { delete ui; }

void DotView::SetHotDotValues(const int hot, const int dot) {
  ui->hot_nb_label->setText(QString::number(hot));
  ui->dot_nb_label->setText(QString::number(dot));
}

void DotView::SetBufDebufValues(const int buf, const int debuf) {
  ui->buf_label_nb->setText(QString::number(buf));
  ui->debuf_label_nb->setText(QString::number(debuf));
}
