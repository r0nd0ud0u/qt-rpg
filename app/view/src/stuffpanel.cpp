#include "stuffpanel.h"
#include "ui_stuffpanel.h"

StuffPanel::StuffPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::StuffPanel) {
  ui->setupUi(this);
}

StuffPanel::~StuffPanel() { delete ui; }

void StuffPanel::Init(const QString &name) {
  m_Name = name;
  ui->stat_label->setText(name);
}

void StuffPanel::on_percent_radioButton_toggled(bool checked) {
  m_IsPercent = checked;
}

void StuffPanel::on_value_spinBox_valueChanged(int arg1) { m_BufValue = arg1; }

void StuffPanel::ResetValues() {
  ui->value_radioButton->setChecked(false);
  ui->value_spinBox->setValue(0);
}
