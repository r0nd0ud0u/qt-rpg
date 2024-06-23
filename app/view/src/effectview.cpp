#include "effectview.h"
#include "ui_effectview.h"

//#include "common.h"

EffectView::EffectView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EffectView) {
  ui->setupUi(this);
}

EffectView::~EffectView() { delete ui; }

// void EffectView::DisconnectComboSig()
// {
//     disconnect(ui->effect_comboBox, &QComboBox::currentTextChanged, nullptr,
//                nullptr);
//     disconnect(ui->stats_comboBox, &QComboBox::currentTextChanged, nullptr,
//                nullptr);
//     // target
//     disconnect(ui->target_comboBox, &QComboBox::currentTextChanged, nullptr,
//                nullptr);
//     // reach
//     disconnect(ui->reach_comboBox, &QComboBox::currentTextChanged, nullptr,
//                nullptr);
// }

// void EffectView::ReconnectComboSig()
// {
//     connect(ui->effect_comboBox, &QComboBox::currentTextChanged, this,
//             &EffectView::on_effect_comboBox_currentTextChanged);
//     // stats
//     connect(ui->stats_comboBox, &QComboBox::currentTextChanged, this,
//             &EffectView::on_stats_comboBox_currentTextChanged);
//     // target
//     connect(ui->target_comboBox, &QComboBox::currentTextChanged, this,
//             &EffectView::on_target_comboBox_currentTextChanged);
//     // reach
//     connect(ui->reach_comboBox, &QComboBox::currentTextChanged, this,
//             &EffectView::on_reach_comboBox_currentTextChanged);
// }

// void EffectView::InitComboBoxes() {
//   // init only one the combo boxes
//   if (m_FirstShow) {
//     return;
//   }
//   m_FirstShow = true;

//   // disconnect signals combo boxes
//   DisconnectComboSig();

//   for (const auto &stat : ALL_STATS) {
//       ui->stats_comboBox->addItem(stat);
//   }
//   for (const auto &target : EFFECTS) {
//     ui->effect_comboBox->addItem(target);
//   }
//   for (const auto &target : ALL_TARGETS) {
//       ui->target_comboBox->addItem(target);
//   }
//   for (const auto &reach : ALL_REACH) {
//       ui->reach_comboBox->addItem(reach);
//   }

//   // Re-activate them
//   // effect
//   ReconnectComboSig();
// }

// void EffectView::SetIndex(const int index) {
//     m_Index = index;
//     std::vector<effectParam> newEffect;
//     newEffect.resize(1);
//     m_EffectTable.push_back(newEffect);
// }

// void EffectView::SetVectorSize(const size_t size){
//     m_EffectTable.clear();
//     m_EffectTable.resize(size);
//     for(auto & effect : m_EffectTable){
//         effect.resize(1);
//     }
// }

// void EffectView::on_effect_comboBox_currentTextChanged(const QString &arg1) {
//   m_EffectTable[m_Index][0].effect = arg1;
//   m_EffectTable[m_Index][0].updated = true;
//   emit SigTableUpdated();
// }

// void EffectView::on_value_spinBox_valueChanged(int arg1) {
//   m_EffectTable[m_Index][0].value = arg1;
//   m_EffectTable[m_Index][0].updated = true;
//   emit SigTableUpdated();
// }

const std::vector<effectParam> &EffectView::GetTable() const { return m_EffectTable[m_Index]; }

// void EffectView::on_checkBox_stateChanged(int arg1) {
//   if (arg1 == 2) {
//         //1
//     ui->effect_comboBox->setEnabled(true);
//     ui->value_spinBox->setEnabled(true);
//     ui->value_spinBox->setEnabled(true);
//     ui->nb_turns_spinBox->setEnabled(true);
//     ui->target_comboBox->setEnabled(true);
//     ui->reach_comboBox->setEnabled(true);
//     ui->stats_comboBox->setEnabled(true);
//     ui->effect_value_spinbox->setEnabled(true);
//   } else {
//     // checkbox1
//     ui->effect_comboBox->setEnabled(false);
//     ui->value_spinBox->setEnabled(false);
//     ui->effect_comboBox->setCurrentIndex(0);
//     ui->nb_turns_spinBox->setEnabled(false);
//     ui->target_comboBox->setEnabled(false);
//     ui->reach_comboBox->setEnabled(false);
//     ui->stats_comboBox->setEnabled(false);
//     ui->effect_value_spinbox->setEnabled(false);
//     ui->stats_comboBox->setCurrentIndex(0);
//     ui->target_comboBox->setCurrentIndex(0);
//     ui->reach_comboBox->setCurrentIndex(0);
//     for (auto& it : m_EffectTable) {
//         if(m_Index < it.size()){
//             it[m_Index] = effectParam();
//         }
//     }
//   }
//   emit SigTableUpdated();
// }

// void EffectView::InitValues(const std::vector<effectParam>& table){
//     DisconnectComboSig();

//     if(!table.empty() && (!table[0].statsName.isEmpty() || !table[0].effect.isEmpty())){
//         ui->effect_comboBox->setCurrentText(table[0].effect);
//         ui->value_spinBox->setValue(table[0].value);
//         ui->nb_turns_spinBox->setValue(table[0].nbTurns);
//         ui->target_comboBox->setCurrentText(table[0].target);
//         ui->reach_comboBox->setCurrentText(table[0].reach);
//         ui->stats_comboBox->setCurrentText(table[0].statsName);
//         ui->effect_value_spinbox->setValue(table[0].subValueEffect);
//         ui->checkBox->setCheckState(Qt::CheckState::Checked);
//     } else{
//         ui->effect_comboBox->setCurrentText("");
//         ui->target_comboBox->setCurrentText("");
//         ui->reach_comboBox->setCurrentText("");
//         ui->stats_comboBox->setCurrentText("");
//         ui->nb_turns_spinBox->setValue(0);
//         ui->value_spinBox->setValue(0);
//         ui->effect_value_spinbox->setEnabled(false);
//         ui->checkBox->setEnabled(true);
//         ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
//     }

//     ReconnectComboSig();

//     //update effect list,
//     for(int i= 0; i< table.size(); i++){
//         if(i == 1){
//             // TODO at the time we display only 3 effets
//             // some effects such eveil de la foret has 5 effects, redesign should be done to display them
//             break;
//         }
//         m_EffectTable[m_Index][i] = table[i];
//     }

// }

// void EffectView::on_reach_comboBox_currentTextChanged(const QString &arg1)
// {
//     m_EffectTable[m_Index][0].reach = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }


// void EffectView::on_stats_comboBox_currentTextChanged(const QString &arg1)
// {
//     m_EffectTable[m_Index][0].statsName = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }

// void EffectView::on_target_comboBox_currentTextChanged(const QString &arg1)
// {
//     m_EffectTable[m_Index][0].target = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }

// void EffectView::on_nb_turns_spinBox_valueChanged(int arg1)
// {
//     m_EffectTable[m_Index][0].nbTurns = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }

// void EffectView::on_effect_value_spinbox_valueChanged(int arg1)
// {
//     m_EffectTable[m_Index][0].subValueEffect = arg1;
//     m_EffectTable[m_Index][0].updated = true;
//     emit SigTableUpdated();
// }
