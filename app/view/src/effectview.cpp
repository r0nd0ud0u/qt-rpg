#include "effectview.h"
#include "ui_effectview.h"

#include "common.h"

EffectView::EffectView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EffectView) {
  ui->setupUi(this);
}

EffectView::~EffectView() { delete ui; }

void EffectView::InitComboBoxes() {
  // init only one the combo boxes
  if (m_FirstShow) {
    return;
  }
  m_FirstShow = true;

  // disconnect signals combo boxes
  disconnect(ui->effect_comboBox, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->effect_comboBox_2, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->effect_comboBox_3, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->stats_comboBox, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->stats_comboBox_2, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->stats_comboBox_3, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  // target
  disconnect(ui->target_comboBox, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->target_comboBox_2, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->target_comboBox_3, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  // reach
  disconnect(ui->reach_comboBox, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->reach_comboBox_2, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->reach_comboBox_3, &QComboBox::currentTextChanged, nullptr,
             nullptr);

  for (const auto &stat : ALL_STATS) {
      ui->stats_comboBox->addItem(stat);
      ui->stats_comboBox_2->addItem(stat);
      ui->stats_comboBox_3->addItem(stat);
  }
  for (const auto &target : EFFECTS) {
    ui->effect_comboBox->addItem(target);
    ui->effect_comboBox_2->addItem(target);
    ui->effect_comboBox_3->addItem(target);
  }
  for (const auto &target : ALL_TARGETS) {
      ui->target_comboBox->addItem(target);
      ui->target_comboBox_2->addItem(target);
      ui->target_comboBox_3->addItem(target);
  }
  for (const auto &reach : ALL_REACH) {
      ui->reach_comboBox->addItem(reach);
      ui->reach_comboBox_2->addItem(reach);
      ui->reach_comboBox_3->addItem(reach);
  }

  // Re-activate them
  // effect
  connect(ui->effect_comboBox, &QComboBox::currentTextChanged, this,
          &EffectView::on_effect_comboBox_currentTextChanged);
  connect(ui->effect_comboBox_2, &QComboBox::currentTextChanged, this,
          &EffectView::on_effect_comboBox_2_currentTextChanged);
  connect(ui->effect_comboBox_3, &QComboBox::currentTextChanged, this,
          &EffectView::on_effect_comboBox_3_currentTextChanged);
  // stats
  connect(ui->stats_comboBox, &QComboBox::currentTextChanged, this,
          &EffectView::on_stats_comboBox_currentTextChanged);
  connect(ui->stats_comboBox_2, &QComboBox::currentTextChanged, this,
          &EffectView::on_stats_comboBox_2_currentTextChanged);
  connect(ui->stats_comboBox_3, &QComboBox::currentTextChanged, this,
          &EffectView::on_stats_comboBox_3_currentTextChanged);
  // target
  connect(ui->target_comboBox, &QComboBox::currentTextChanged, this,
          &EffectView::on_target_comboBox_currentTextChanged);
  connect(ui->target_comboBox_2, &QComboBox::currentTextChanged, this,
          &EffectView::on_target_comboBox_2_currentTextChanged);
  connect(ui->target_comboBox_3, &QComboBox::currentTextChanged, this,
          &EffectView::on_target_comboBox_3_currentTextChanged);
  // reach
  connect(ui->reach_comboBox, &QComboBox::currentTextChanged, this,
          &EffectView::on_reach_comboBox_currentTextChanged);
  connect(ui->reach_comboBox_2, &QComboBox::currentTextChanged, this,
          &EffectView::on_reach_comboBox_2_currentTextChanged);
  connect(ui->reach_comboBox_3, &QComboBox::currentTextChanged, this,
          &EffectView::on_reach_comboBox_3_currentTextChanged);
}

void EffectView::SetIndex(const int index) {
    m_Index = index;
}

void EffectView::SetVectorSize(const size_t size){
    m_EffectTable.resize(size);
    for(auto & effect : m_EffectTable){
        effect.resize(3);
    }
}

void EffectView::on_effect_comboBox_currentTextChanged(const QString &arg1) {
  m_EffectTable[m_Index][0].effect = arg1;
  m_EffectTable[m_Index][0].updated = true;
  emit SigTableUpdated();
}

void EffectView::on_effect_comboBox_2_currentTextChanged(const QString &arg1) {
  m_EffectTable[m_Index][1].effect = arg1;
  m_EffectTable[m_Index][1].updated = true;
  emit SigTableUpdated();
}

void EffectView::on_effect_comboBox_3_currentTextChanged(const QString &arg1) {
  m_EffectTable[m_Index][2].effect = arg1;
  m_EffectTable[m_Index][2].updated = true;
  emit SigTableUpdated();
}

void EffectView::on_value_spinBox_valueChanged(int arg1) {
  m_EffectTable[m_Index][0].value = arg1;
  m_EffectTable[m_Index][0].updated = true;
  emit SigTableUpdated();
}

void EffectView::on_value_spinBox_2_valueChanged(int arg1) {
  m_EffectTable[m_Index][1].value = arg1;
  m_EffectTable[m_Index][1].updated = true;
  emit SigTableUpdated();
}

void EffectView::on_value_spinBox_3_valueChanged(int arg1) {
  m_EffectTable[m_Index][2].value = arg1;
  m_EffectTable[m_Index][2].updated = true;
  emit SigTableUpdated();
}

const std::vector<effectParam> &EffectView::GetTable() const { return m_EffectTable[m_Index]; }

void EffectView::on_checkBox_stateChanged(int arg1) {
  if (arg1 == 2) {
        //1
    ui->effect_comboBox->setEnabled(true);
    ui->value_spinBox->setEnabled(true);
    ui->value_spinBox->setEnabled(true);
    ui->nb_turns_spinBox->setEnabled(true);
    ui->target_comboBox->setEnabled(true);
    ui->reach_comboBox->setEnabled(true);
    ui->stats_comboBox->setEnabled(true);
    // 2
    ui->checkBox_2->setEnabled(true);
  } else {
    // checkbox1
    ui->effect_comboBox->setEnabled(false);
    ui->value_spinBox->setEnabled(false);
    ui->effect_comboBox->setCurrentIndex(0);
    ui->nb_turns_spinBox->setEnabled(false);
    ui->target_comboBox->setEnabled(false);
    ui->reach_comboBox->setEnabled(false);
    ui->stats_comboBox->setEnabled(false);
    ui->stats_comboBox->setCurrentIndex(0);
    ui->target_comboBox->setCurrentIndex(0);
    ui->reach_comboBox->setCurrentIndex(0);
    // checkbox2
    ui->effect_comboBox_2->setCurrentIndex(0);
    ui->effect_comboBox_2->setEnabled(false);
    ui->value_spinBox_2->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    ui->nb_turns_spinBox_2->setEnabled(false);
    ui->target_comboBox_2->setEnabled(false);
    ui->reach_comboBox_2->setEnabled(false);
    ui->stats_comboBox_2->setEnabled(false);
    ui->stats_comboBox_2->setCurrentIndex(0);
    ui->target_comboBox_2->setCurrentIndex(0);
    ui->effect_comboBox_2->setCurrentIndex(0);
    ui->reach_comboBox_2->setCurrentIndex(0);
    // checkbox3
    ui->checkBox_3->setEnabled(false);
    ui->effect_comboBox_3->setCurrentIndex(0);
    ui->effect_comboBox_3->setEnabled(false);
    ui->value_spinBox_3->setEnabled(false);
    ui->nb_turns_spinBox_3->setEnabled(false);
    ui->target_comboBox_3->setEnabled(false);
    ui->reach_comboBox_3->setEnabled(false);
    ui->stats_comboBox_3->setEnabled(false);
    ui->stats_comboBox_3->setCurrentIndex(0);
    ui->target_comboBox_3->setCurrentIndex(0);
    ui->effect_comboBox_3->setCurrentIndex(0);
    ui->reach_comboBox_3->setCurrentIndex(0);
    for (auto& it : m_EffectTable) {
        it[m_Index] = effectParam();
    }
  }
  emit SigTableUpdated();
}

void EffectView::on_checkBox_2_stateChanged(int arg1) {
  if (arg1 == 2) {
        // 2
    ui->effect_comboBox_2->setEnabled(true);
    ui->value_spinBox_2->setEnabled(true);
    ui->nb_turns_spinBox_2->setEnabled(true);
    ui->target_comboBox_2->setEnabled(true);
    ui->reach_comboBox_2->setEnabled(true);
    ui->stats_comboBox_2->setEnabled(true);
    // 3
    ui->checkBox_3->setEnabled(true);
  } else {
    // checkbox2
    ui->effect_comboBox_2->setCurrentIndex(0);
    ui->effect_comboBox_2->setEnabled(false);
    ui->value_spinBox_2->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    ui->nb_turns_spinBox_2->setEnabled(false);
    ui->target_comboBox_2->setEnabled(false);
    ui->reach_comboBox_2->setEnabled(false);
    ui->stats_comboBox_2->setEnabled(false);
    ui->stats_comboBox_2->setCurrentIndex(0);
    ui->target_comboBox_2->setCurrentIndex(0);
    ui->effect_comboBox_2->setCurrentIndex(0);
    ui->reach_comboBox_2->setCurrentIndex(0);
    // checkbox3
    ui->checkBox_3->setEnabled(false);
    ui->effect_comboBox_3->setCurrentIndex(0);
    ui->effect_comboBox_3->setEnabled(false);
    ui->value_spinBox_3->setEnabled(false);
    ui->nb_turns_spinBox_3->setEnabled(false);
    ui->target_comboBox_3->setEnabled(false);
    ui->reach_comboBox_3->setEnabled(false);
    ui->stats_comboBox_3->setEnabled(false);
    ui->stats_comboBox_3->setCurrentIndex(0);
    ui->target_comboBox_3->setCurrentIndex(0);
    ui->effect_comboBox_3->setCurrentIndex(0);
    ui->reach_comboBox_3->setCurrentIndex(0);
    // effect table
    m_EffectTable[m_Index][1] = effectParam();
    m_EffectTable[m_Index][2] = effectParam();
  }
  emit SigTableUpdated();
}

void EffectView::on_checkBox_3_stateChanged(int arg1) {
  if (arg1 == 2) {
    ui->effect_comboBox_3->setEnabled(true);
    ui->value_spinBox_3->setEnabled(true);
    ui->nb_turns_spinBox_3->setEnabled(true);
    ui->target_comboBox_3->setEnabled(true);
    ui->reach_comboBox_3->setEnabled(true);
    ui->stats_comboBox_3->setEnabled(true);
  } else {
    ui->effect_comboBox_3->setEnabled(false);
    ui->value_spinBox_3->setEnabled(false);
    ui->effect_comboBox_3->setCurrentIndex(0);
    ui->effect_comboBox_3->setEnabled(false);
    ui->nb_turns_spinBox_3->setEnabled(false);
    ui->target_comboBox_3->setEnabled(false);
    ui->reach_comboBox_3->setEnabled(false);
    ui->stats_comboBox_3->setEnabled(false);
    ui->stats_comboBox_3->setCurrentIndex(0);
    ui->target_comboBox_3->setCurrentIndex(0);
    ui->effect_comboBox_3->setCurrentIndex(0);
    ui->reach_comboBox_3->setCurrentIndex(0);
    m_EffectTable[m_Index][2] = effectParam();
  }
  emit SigTableUpdated();
}

void EffectView::SetValues(const std::vector<effectParam>& table){
    if(!table.empty() && !table[0].statsName.isEmpty()){
        ui->effect_comboBox->setCurrentText(table[0].effect);
        ui->value_spinBox->setValue(table[0].value);
        ui->nb_turns_spinBox->setValue(table[0].nbTurns);
        ui->target_comboBox->setCurrentText(table[0].target);
        ui->reach_comboBox->setCurrentText(table[0].reach);
        ui->stats_comboBox->setCurrentText(table[0].statsName);
        ui->checkBox->setCheckState(Qt::CheckState::Checked);
    } else{
        ui->effect_comboBox->setCurrentText("");
        ui->target_comboBox->setCurrentText("");
        ui->reach_comboBox->setCurrentText("");
        ui->stats_comboBox->setCurrentText("");
        ui->nb_turns_spinBox->setValue(0);
        ui->value_spinBox->setValue(0);
        ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
        return;
    }
    if(table.size() > 1 && !table[1].statsName.isEmpty()){
        ui->effect_comboBox_2->setCurrentText(table[1].effect);
        ui->value_spinBox_2->setValue(table[1].value);
        ui->nb_turns_spinBox_2->setValue(table[1].nbTurns);
        ui->target_comboBox_2->setCurrentText(table[1].target);
        ui->reach_comboBox_2->setCurrentText(table[1].reach);
        ui->stats_comboBox_2->setCurrentText(table[1].statsName);
        ui->checkBox_2->setCheckState(Qt::CheckState::Checked);
    }else{
        ui->effect_comboBox_2->setCurrentText("");
        ui->value_spinBox_2->setValue(0);
        ui->target_comboBox_2->setCurrentText("");
        ui->reach_comboBox_2->setCurrentText("");
        ui->stats_comboBox_2->setCurrentText("");
        ui->nb_turns_spinBox_2->setValue(0);
        ui->checkBox_2->setCheckState(Qt::CheckState::Unchecked);
        return;
    }
    if(table.size() > 2 && !table[2].statsName.isEmpty()){
        ui->effect_comboBox_3->setCurrentText(table[2].effect);
        ui->value_spinBox_3->setValue(table[2].value);
        ui->nb_turns_spinBox_3->setValue(table[2].nbTurns);
        ui->target_comboBox_3->setCurrentText(table[2].target);
        ui->reach_comboBox_3->setCurrentText(table[2].reach);
        ui->stats_comboBox_3->setCurrentText(table[2].statsName);
        ui->checkBox_3->setCheckState(Qt::CheckState::Checked);
    }else{
        ui->effect_comboBox_3->setCurrentText("");
        ui->value_spinBox_3->setValue(0);
        ui->target_comboBox_3->setCurrentText("");
        ui->reach_comboBox_3->setCurrentText("");
        ui->stats_comboBox_3->setCurrentText("");
        ui->nb_turns_spinBox_3->setValue(0);
        ui->checkBox_3->setCheckState(Qt::CheckState::Unchecked);
        return;
    }

    //update effect list,
    m_EffectTable[m_Index] = table;
}

void EffectView::on_reach_comboBox_currentTextChanged(const QString &arg1)
{
    m_EffectTable[m_Index][0].reach = arg1;
    m_EffectTable[m_Index][0].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_stats_comboBox_currentTextChanged(const QString &arg1)
{
    m_EffectTable[m_Index][0].statsName = arg1;
    m_EffectTable[m_Index][0].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_stats_comboBox_2_currentTextChanged(const QString &arg1)
{
    m_EffectTable[m_Index][1].statsName = arg1;
    m_EffectTable[m_Index][1].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_stats_comboBox_3_currentTextChanged(const QString &arg1)
{
    m_EffectTable[m_Index][2].statsName = arg1;
    m_EffectTable[m_Index][2].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_target_comboBox_currentTextChanged(const QString &arg1)
{
    m_EffectTable[m_Index][0].target = arg1;
    m_EffectTable[m_Index][0].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_target_comboBox_2_currentTextChanged(const QString &arg1)
{
    m_EffectTable[m_Index][1].target = arg1;
    m_EffectTable[m_Index][1].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_target_comboBox_3_currentTextChanged(const QString &arg1)
{
    m_EffectTable[m_Index][2].target = arg1;
    m_EffectTable[m_Index][2].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_reach_comboBox_2_currentTextChanged(const QString &arg1)
{
    m_EffectTable[m_Index][1].reach = arg1;
    m_EffectTable[m_Index][1].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_reach_comboBox_3_currentTextChanged(const QString &arg1)
{
    m_EffectTable[m_Index][2].reach = arg1;
    m_EffectTable[m_Index][2].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_nb_turns_spinBox_valueChanged(int arg1)
{
    m_EffectTable[m_Index][0].nbTurns = arg1;
    m_EffectTable[m_Index][0].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_nb_turns_spinBox_2_valueChanged(int arg1)
{
    m_EffectTable[m_Index][1].nbTurns = arg1;
    m_EffectTable[m_Index][1].updated = true;
    emit SigTableUpdated();
}


void EffectView::on_nb_turns_spinBox_3_valueChanged(int arg1)
{
    m_EffectTable[m_Index][2].nbTurns = arg1;
    m_EffectTable[m_Index][2].updated = true;
    emit SigTableUpdated();
}

