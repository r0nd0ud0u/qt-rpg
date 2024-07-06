#include "editatkpanel.h"
#include "ui_editatkpanel.h"

#include "common.h"

EditAtkPanel::EditAtkPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::EditAtkPanel) {
  ui->setupUi(this);
  InitComboBoxes();
}

EditAtkPanel::~EditAtkPanel() { delete ui; }

void EditAtkPanel::UpdatePanel(const effectParam &ep) {
  m_EffectParam = ep;

  ui->stats_comboBox->setCurrentText(ep.statsName);
  ui->effect_comboBox->setCurrentText(ep.effect);
  ui->effect_value_spinbox->setValue(ep.subValueEffect);
  ui->target_comboBox->setCurrentText(ep.target);
  ui->reach_comboBox->setCurrentText(ep.reach);
  ui->value_spinBox->setValue(ep.value);
  ui->nb_turns_spinBox->setValue(ep.nbTurns);
}

void EditAtkPanel::InitComboBoxes() {
  for (const auto &stat : ALL_STATS) {
    ui->stats_comboBox->addItem(stat);
  }
  for (const auto &effect : EFFECTS) {
    ui->effect_comboBox->addItem(effect);
  }
  for (const auto &target : ALL_TARGETS) {
    ui->target_comboBox->addItem(target);
  }
  for (const auto &reach : ALL_REACH) {
    ui->reach_comboBox->addItem(reach);
  }
}

void EditAtkPanel::on_stats_comboBox_currentTextChanged(const QString &arg1)
{
    m_EffectParam.statsName = arg1;
    emit SigValueChanged();
}


void EditAtkPanel::on_effect_comboBox_currentTextChanged(const QString &arg1)
{
    m_EffectParam.effect = arg1;
    emit SigValueChanged();
}


void EditAtkPanel::on_effect_value_spinbox_valueChanged(int arg1)
{
    m_EffectParam.subValueEffect = arg1;
    emit SigValueChanged();
}


void EditAtkPanel::on_target_comboBox_currentTextChanged(const QString &arg1)
{
    m_EffectParam.target = arg1;
    emit SigValueChanged();
}


void EditAtkPanel::on_reach_comboBox_currentTextChanged(const QString &arg1)
{
    m_EffectParam.reach = arg1;
    emit SigValueChanged();
}


void EditAtkPanel::on_value_spinBox_valueChanged(int arg1)
{
    m_EffectParam.value = arg1;
    emit SigValueChanged();
}


void EditAtkPanel::on_nb_turns_spinBox_valueChanged(int arg1)
{
    m_EffectParam.nbTurns = arg1;
    emit SigValueChanged();
}

