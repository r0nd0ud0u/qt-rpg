#include "editattakview.h"
#include "ui_editattakview.h"

// Create output json
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include "character.h"
#include <Application.h>

EditAttakView::EditAttakView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EditAttakView) {
  ui->setupUi(this);
}

EditAttakView::~EditAttakView() { delete ui; }

void EditAttakView::InitView() {
  const auto &app = Application::GetInstance();
  if (app.m_GameManager == nullptr ||
      app.m_GameManager->m_PlayersManager == nullptr ||
      app.m_GameManager->m_PlayersManager->m_SelectedHero == nullptr) {
    return;
  }
  const auto &selectedHeroAtkList =
      app.m_GameManager->m_PlayersManager->m_SelectedHero->m_AttakList;

  // init attributes
  m_SelectedCharaName =
      app.m_GameManager->m_PlayersManager->m_SelectedHero->m_Name;

  // Create model
  auto *model = new QStringListModel(this);
  // Make data
  QStringList List;
  // Init List and m_AttakList
  m_AttakList.clear();
  for (const auto &[atkName, atk] : selectedHeroAtkList) {
    List << atkName;
    EditAttak editAtk;
    editAtk.type = atk;
    m_AttakList.push_back(editAtk);
  }

  // Populate our model
  model->setStringList(List);

  // Glue model and view together
  ui->atk_list_view->setModel(model);

  // Update view
  if (!m_AttakList.empty()) {
    ui->atk_list_view->setCurrentIndex(model->index(0));
    InitComboBoxes();
    UpdateValues(m_AttakList.front());
  } else {
    EnableAllWidgets(false);
  }
  ui->apply_button->setEnabled(false);
}

void EditAttakView::on_apply_button_clicked() { Apply(); }

void EditAttakView::Apply() {
  // disable button
  ui->apply_button->setEnabled(false);
  ui->effect_widget->GetTable();
}

void EditAttakView::Save() {
  // apply all the changes
  Apply();

  QFile file;
  QDir logDir;
  QString pathAtkChara = OFFLINE_ATK + m_SelectedCharaName + "/";
  logDir.mkpath(pathAtkChara);

  if (const int index = ui->atk_list_view->currentIndex().row();
      index > m_AttakList.size()) {
    return;
  }
  for (const auto &atk : m_AttakList) {
    if (!atk.updated) {
      continue;
    }
    // init json doc
    QJsonObject obj;
    obj.insert(ATK_NAME, atk.type.name);
    obj.insert(ATK_TARGET, atk.type.target);
    obj.insert(ATK_REACH, atk.type.reach);
    obj.insert(ATK_DURATION, atk.type.turnsDuration);
    obj.insert(ATK_MANA_COST, static_cast<int>(atk.type.manaCost));
    obj.insert(ATK_VIGOR_COST, static_cast<int>(atk.type.vigorCost));
    obj.insert(ATK_BERSECK_COST, static_cast<int>(atk.type.berseckCost));
    obj.insert(ATK_AGGRO, static_cast<int>(atk.type.aggro));
    obj.insert(ATK_PHOTO, atk.type.namePhoto);
    obj.insert(ATK_DAMAGE, static_cast<int>(atk.type.damage));
    obj.insert(ATK_HEAL, static_cast<int>(atk.type.heal));
    obj.insert(ATK_REGEN_MANA, static_cast<int>(atk.type.regenMana));
    obj.insert(ATK_LEVEL, static_cast<int>(atk.type.level));
    obj.insert(ATK_REGEN_BERSECK, static_cast<int>(atk.type.regenBerseck));
    obj.insert(ATK_REGEN_VIGOR, static_cast<int>(atk.type.regenVigor));
    for(const auto& effect : ui->effect_widget->GetTable()){
        obj.insert(effect.effect, effect.value);
    }
    // output attak json
    QJsonDocument doc(obj);

    QString logFilePath =
        logDir.filePath(pathAtkChara + atk.type.name + ".json");
    file.setFileName(logFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QMessageBox::information(
          nullptr, tr("Error log file"),
          tr("Log file could not be created at %1. No log will be produced.")
              .arg(logFilePath));
    }
    QTextStream out(&file);
    out << doc.toJson() << "\n";
  }

  // update selected character
  auto &selectedHeroAtkList =
      Application::GetInstance()
          .m_GameManager->m_PlayersManager->m_SelectedHero->m_AttakList;
  selectedHeroAtkList.clear();
  for (const auto &atk : m_AttakList) {
    selectedHeroAtkList[atk.type.name] = atk.type;
  }
}

void EditAttakView::InitComboBoxes() {
  // init only one the combo boxes
  if (m_FirstShow) {
    return;
  }
  m_FirstShow = true;

  // disconnect signals combo boxes
  disconnect(ui->target_comboBox, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->reach_comboBox, &QComboBox::currentTextChanged, nullptr,
             nullptr);
  disconnect(ui->photo_comboBox, &QComboBox::currentTextChanged, nullptr,
             nullptr);

  ui->target_comboBox->setEnabled(true);
  for (const auto &target : AttaqueType::TARGET_TYPES) {
    ui->target_comboBox->addItem(target);
  }
  for (const auto &reach : AttaqueType::REACH_TYPES) {
    ui->reach_comboBox->addItem(reach);
  }
  // List all attak png string and add them to photo_comboBox
  QString directoryPath = OFFLINE_IMG; // Replace with the actual path
  QDir directory(directoryPath);
  if (!directory.exists()) {
    qDebug() << "Directory does not exist: " << directoryPath;
  }
  QStringList fileList =
      directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
  for (const QString &file : fileList) {
    ui->photo_comboBox->addItem(file);
  }

  // Re-activate them
  connect(ui->target_comboBox, &QComboBox::currentTextChanged, this,
          &EditAttakView::on_target_comboBox_currentTextChanged);
  connect(ui->reach_comboBox, &QComboBox::currentTextChanged, this,
          &EditAttakView::on_reach_comboBox_currentTextChanged);
  connect(ui->photo_comboBox, &QComboBox::currentTextChanged, this,
          &EditAttakView::on_photo_comboBox_currentTextChanged);
}

void EditAttakView::UpdateValues(const EditAttak &selectedAttak) {
  ui->target_comboBox->setCurrentText(selectedAttak.type.target);
  ui->reach_comboBox->setCurrentText(selectedAttak.type.reach);
  ui->name_lineEdit->setText(selectedAttak.type.name);
  ui->duration_spinBox->setValue(selectedAttak.type.turnsDuration);
  ui->aggro_spinBox->setValue(selectedAttak.type.aggro);
  ui->mana_cost_spinBox->setValue(
      static_cast<int>(selectedAttak.type.manaCost));
  ui->vigor_spinBox->setValue(selectedAttak.type.vigorCost);
  ui->berseck_spinBox->setValue(selectedAttak.type.berseckCost);
  ui->heal_spinBox->setValue(selectedAttak.type.heal);
  ui->damage_spinBox->setValue(selectedAttak.type.damage);
  ui->photo_comboBox->setCurrentText(selectedAttak.type.namePhoto);
  ui->regen_mana_spinBox->setValue(selectedAttak.type.regenMana);
  ui->level_spinBox->setValue(selectedAttak.type.level);
  ui->regen_rage_spinBox->setValue(selectedAttak.type.regenBerseck);
  ui->regen_vigor_spinBox->setValue(selectedAttak.type.regenVigor);
}

void EditAttakView::EnableAllWidgets(const bool value) const {
  for (int i = 0; i < ui->values_form_lay->count(); i++) {
    auto *widget = ui->values_form_lay->itemAt(i)->widget();
    if (widget != nullptr) {
      widget->setEnabled(value);
    }
  }
}
void EditAttakView::on_atk_list_view_clicked(const QModelIndex &index) {
  const int idx = index.row();
  if (idx >= m_AttakList.size()) {
    return;
  }

  // enable all widgets of the values form layout to true
  const auto &selectedAttak = m_AttakList[idx];
  EnableAllWidgets(true);

  // update values with the ones from the current selected character
  UpdateValues(selectedAttak);

  // disable apply button
  ui->apply_button->setEnabled(false);
}

void EditAttakView::on_new_atk_button_clicked() {
  // apply button
  ui->apply_button->setEnabled(true);
  // add row
  ui->atk_list_view->model()->insertRow(ui->atk_list_view->model()->rowCount());
  QModelIndex itemIndex = ui->atk_list_view->model()->index(
      ui->atk_list_view->model()->rowCount() - 1, 0);
  ui->atk_list_view->setCurrentIndex(itemIndex);
  ui->atk_list_view->model()->setData(itemIndex, EditAttak().type.name);
  // update m_AttakList
  m_AttakList.push_back(EditAttak());
  m_AttakList.back().updated = true;
  UpdateValues(m_AttakList.back());

  EnableAllWidgets(true);
  if (ui->atk_list_view->model()->rowCount() == 1) {
    InitComboBoxes();
      ui->effect_widget->InitComboBoxes();
  }
}
// form layout value changed

void EditAttakView::on_photo_comboBox_currentTextChanged(const QString &arg1) {
  // Update image character
  auto qp = QPixmap(OFFLINE_IMG + arg1);
  ui->img_char->setPixmap(qp);
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.namePhoto = arg1;
}

int EditAttakView::GetIndexSelectedRow() const {
  return ui->atk_list_view->currentIndex().row();
}

void EditAttakView::OnValueChange(const int index) {
  ui->apply_button->setEnabled(true);
  m_AttakList[index].updated = true;
}

void EditAttakView::on_name_lineEdit_textChanged(const QString &arg1) {
  ui->atk_list_view->model()->setData(ui->atk_list_view->currentIndex(), arg1);
  ui->apply_button->setEnabled(true);
  m_AttakList[GetIndexSelectedRow()].type.name = ui->name_lineEdit->text();
}

void EditAttakView::on_duration_spinBox_valueChanged(
    [[maybe_unused]] int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.turnsDuration =
      static_cast<uint16_t>(ui->duration_spinBox->value());
}

void EditAttakView::on_mana_cost_spinBox_valueChanged(
    [[maybe_unused]] int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.manaCost =
      ui->mana_cost_spinBox->value();
}

void EditAttakView::on_heal_spinBox_valueChanged([[maybe_unused]] int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.heal = ui->heal_spinBox->value();
}

void EditAttakView::on_damage_spinBox_valueChanged(int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.damage = arg1;
}

void EditAttakView::on_target_comboBox_currentTextChanged(const QString &arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.target = arg1;
}

void EditAttakView::on_reach_comboBox_currentTextChanged(const QString &arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.reach = arg1;
}

void EditAttakView::on_regen_mana_spinBox_valueChanged(int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.regenMana = arg1;
}

void EditAttakView::on_vigor_spinBox_valueChanged(int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.vigorCost = arg1;
}

void EditAttakView::on_berseck_spinBox_valueChanged(int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.berseckCost = arg1;
}

void EditAttakView::on_level_spinBox_valueChanged(int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.level = static_cast<uint8_t>(arg1);
}

void EditAttakView::on_regen_rage_spinBox_valueChanged(int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.regenBerseck = arg1;
}

void EditAttakView::on_regen_vigor_spinBox_valueChanged(int arg1) {
  OnValueChange(GetIndexSelectedRow());
  m_AttakList[GetIndexSelectedRow()].type.regenVigor = arg1;
}

void EditAttakView::on_aggro_spinBox_valueChanged(int arg1) {
  OnValueChange(GetIndexSelectedRow());
    m_AttakList[GetIndexSelectedRow()].type.aggro = arg1;
}

void EditAttakView::on_aggro_rate_spinBox_valueChanged(int arg1)
{
    OnValueChange(GetIndexSelectedRow());
    m_AttakList[GetIndexSelectedRow()].type.aggroRate = arg1;
}


void EditAttakView::on_berseck_rate_spinBox_valueChanged(int arg1)
{
    OnValueChange(GetIndexSelectedRow());
    m_AttakList[GetIndexSelectedRow()].type.berseckRate = arg1;
}

// end form layout changed
