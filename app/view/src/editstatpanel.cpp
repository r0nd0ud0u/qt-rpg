#include "editstatpanel.h"
#include "ui_editstatpanel.h"

EditStatPanel::EditStatPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::EditStatPanel) {
  ui->setupUi(this);
}

EditStatPanel::~EditStatPanel() { delete ui; }

void EditStatPanel::Init(const QString &name, const Stats &stats) {
  ui->stats_name_lbl->setText(name);
}

void EditStatPanel::on_start_spinBox_valueChanged(int arg1) {
  m_StartingValue = arg1;
}

void EditStatPanel::on_max_spinBox_valueChanged(int arg1) { m_MaxValue = arg1; }

void EditStatPanel::on_regen_spinBox_valueChanged(int arg1) {
  m_RegenValue = arg1;
}
