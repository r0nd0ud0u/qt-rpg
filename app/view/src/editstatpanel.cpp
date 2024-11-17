#include "editstatpanel.h"
#include "ui_editstatpanel.h"

#include "utils.h"

EditStatPanel::EditStatPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::EditStatPanel) {
  ui->setupUi(this);
}

EditStatPanel::~EditStatPanel() { delete ui; }

void EditStatPanel::Init(const QString &name, const Stats &stats) {
  ui->stats_name_lbl->setText(name);
}

void EditStatPanel::on_max_spinBox_valueChanged(int arg1) { m_RawMax = arg1; }

void EditStatPanel::SetStatsValue(const StatsType &stat) {
  ui->max_spinBox->setValue(stat.m_RawMaxValue);
  ui->spinBox->setValue(stat.m_CurrentRawValue);
  m_CurrRaw = stat.m_CurrentRawValue;
  m_RawMax = stat.m_RawMaxValue;
}

void EditStatPanel::on_spinBox_valueChanged(int arg1) { m_CurrRaw = arg1; }
