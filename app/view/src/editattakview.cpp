#include "editattakview.h"
#include "ui_editattakview.h"

// Create output json
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QXmlStreamReader>

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
      app.m_GameManager->m_PlayersManager->m_SelectedHero->attakList;
  if (selectedHeroAtkList.empty()) {
      return;
  }

  // Create model
  auto *model = new QStringListModel(this);
  // Make data
  QStringList List;
  // Init List and m_AttakList
  m_AttakList.clear();
  for (const auto &atk : selectedHeroAtkList) {
    List << atk.name;
    qDebug() << atk.name;
    EditAttak editAtk;
    editAtk.type = atk;
    m_AttakList.push_back(editAtk);
  }

  // Populate our model
  model->setStringList(List);

  // Glue model and view together
  ui->atk_list_view->setModel(model);

  // Update view
  ui->atk_list_view->setCurrentIndex(model->index(0));
  UpdateValues(m_AttakList.front());
  ui->apply_button->setEnabled(false);
}

void EditAttakView::on_apply_button_clicked() { Apply(); }

void EditAttakView::Apply() {
   // disable button
    ui->apply_button->setEnabled(false);

  int curIndex = ui->atk_list_view->currentIndex().row();
  auto &curAtk = m_AttakList[curIndex];

  // atk list of edit attak view
  curAtk.type.name = ui->name_lineEdit->text();
  curAtk.type.target =
      static_cast<TargetType>(ui->target_comboBox->currentIndex());
  curAtk.type.reach =
      static_cast<ReachType>(ui->reach_comboBox->currentIndex());
  curAtk.type.turnsDuration = static_cast<uint16_t>(ui->duration_spinBox->value());
  curAtk.type.manaCost = static_cast<uint16_t>(ui->mana_cost_spinBox->value());
  curAtk.type.aggroCum = static_cast<uint16_t>(ui->rage_aggro_spinBox->value());
  curAtk.type.namePhoto = ui->photo_comboBox->currentText();
}

void EditAttakView::Save() {
    // apply all the changes
    Apply();

  QFile file;
  QDir logDir;
  logDir.mkpath(".");

  const int index = ui->atk_list_view->currentIndex().row();
  if (index > m_AttakList.size()) {
    return;
  }
    for (const auto &atk : m_AttakList) {
      if (!atk.updated) {
        continue;
      }
      // init json doc
      QJsonObject obj;
      obj.insert("name", atk.type.name);
      obj.insert("target", Character::GetTargetString(atk.type.target));
      obj.insert("reach", Character::GetReachString(atk.type.reach));
      obj.insert("duration", atk.type.turnsDuration);
      obj.insert("mana_cost", QString::number(atk.type.manaCost));
      obj.insert("rage_aggro", QString::number(atk.type.aggroCum));
      obj.insert("photo", atk.type.namePhoto);
      // output attak json
      QJsonDocument doc(obj);

      QString logFilePath = logDir.filePath("./offlines/attak/" +
                                            m_AttakList[index].type.name + ".json");
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
}

void EditAttakView::UpdateValues(const EditAttak &selectedAttak) {
  ui->name_lineEdit->setText(selectedAttak.type.name);
  ui->target_comboBox->setEnabled(true);
  for (int i = 0; i < static_cast<int>(TargetType::enumSize); i++) {
    ui->target_comboBox->addItem(
        Character::GetTargetString(static_cast<TargetType>(i)));
  }
  for (int i = 0; i < static_cast<int>(ReachType::enumSize); i++) {
    ui->reach_comboBox->addItem(
        Character::GetReachString(static_cast<ReachType>(i)));
  }
  ui->duration_spinBox->setValue(selectedAttak.type.turnsDuration);
  ui->rage_aggro_spinBox->setValue(selectedAttak.type.aggroCum);
  ui->mana_cost_spinBox->setValue(selectedAttak.type.manaCost);

  // List all attak png string and add them to photo_comboBox
  QString directoryPath = OFFLINE_IMG; // Replace with the actual path
  QDir directory(directoryPath);
  if (!directory.exists()) {
    qDebug() << "Directory does not exist: " << directoryPath;
  }
  QStringList fileList =
      directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
  qDebug() << "List of files in the directory:";
  ui->photo_comboBox->clear();
  for (const QString &file : fileList) {
    qDebug() << file;
    ui->photo_comboBox->addItem(file);
  }
}

void EditAttakView::on_atk_list_view_clicked(const QModelIndex &index) {
  const auto &app = Application::GetInstance();
  if (app.m_GameManager == nullptr ||
      app.m_GameManager->m_PlayersManager == nullptr ||
      app.m_GameManager->m_PlayersManager->m_SelectedHero == nullptr) {
    return;
  }
  const int idx = index.row();
  if (idx >=
      app.m_GameManager->m_PlayersManager->m_SelectedHero->attakList.size()) {
    return;
  }

  // enable all widgets of the values form layout to true
  const auto &selectedAttak = m_AttakList[idx];
  for (int i = 0; i < ui->values_form_lay->count(); i++) {
    auto *widget = ui->values_form_lay->itemAt(i)->widget();
    if (widget != nullptr) {
      widget->setEnabled(true);
    }
  }

  // update values with the ones from the current selected character
  UpdateValues(selectedAttak);

  // disable apply button
  ui->apply_button->setEnabled(false);
}

// form layout value changed

void EditAttakView::on_photo_comboBox_currentTextChanged(const QString &arg1) {
  // Update image character
  auto qp = QPixmap(OFFLINE_IMG + arg1);
  ui->img_char->setPixmap(qp);
}

void EditAttakView::OnValueChange()
{
    ui->apply_button->setEnabled(true);
    const int curIndex = ui->atk_list_view->currentIndex().row();
    m_AttakList[curIndex].updated = true;
}

void EditAttakView::on_name_lineEdit_textChanged([[maybe_unused]]const QString &arg1) {
  OnValueChange();
}

void EditAttakView::on_target_comboBox_currentIndexChanged([[maybe_unused]]int index) {
  OnValueChange();
}

void EditAttakView::on_reach_comboBox_currentIndexChanged([[maybe_unused]]int index) {
  OnValueChange();
}

void EditAttakView::on_duration_spinBox_valueChanged([[maybe_unused]]int arg1) {
  OnValueChange();
}

void EditAttakView::on_rage_aggro_spinBox_valueChanged([[maybe_unused]]int arg1) {
  OnValueChange();
}

void EditAttakView::on_mana_cost_spinBox_valueChanged([[maybe_unused]]int arg1) {
  OnValueChange();
}

void EditAttakView::on_photo_comboBox_currentIndexChanged([[maybe_unused]]int index) {
  OnValueChange();
}

// end form layout changed

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
  m_AttakList.emplace_back(EditAttak());
  m_AttakList.back().updated = true;
  UpdateValues(EditAttak());
}


