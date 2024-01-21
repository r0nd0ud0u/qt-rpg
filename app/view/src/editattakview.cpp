#include "editattakview.h"
#include "ui_editattakview.h"

#include <QStringListModel>
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

  const auto &app = Application::GetInstance();
  if (app.m_GameManager == nullptr ||
      app.m_GameManager->m_PlayersManager == nullptr ||
      app.m_GameManager->m_PlayersManager->m_SelectedHero == nullptr) {
    return;
  }

  // Create model
  auto *model = new QStringListModel(this);

  // Make data
  QStringList List;
  const auto &selectedHeroAtkList =
      app.m_GameManager->m_PlayersManager->m_SelectedHero->attakList;
  if (selectedHeroAtkList.empty()) {
    return;
  }

  // Init List and m_AttakList
  m_AttakList.clear();
  for (const auto &atk : selectedHeroAtkList) {
      List << atk.name;
      m_AttakList.push_back(atk);
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

EditAttakView::~EditAttakView() { delete ui; }

void EditAttakView::on_apply_button_clicked() {
  QFile file;
  QDir logDir;
  logDir.mkpath(".");

  QString logFilePath = logDir.filePath("./offlines/attak/test.json");
  file.setFileName(logFilePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::information(
        nullptr, tr("Error log file"),
        tr("Log file could not be created at %1. No log will be produced.")
            .arg(logFilePath));
  } else {
    QJsonObject obj;
    obj.insert("name", ui->name_lineEdit->text());
    obj.insert("target", ui->target_comboBox->currentText());
    obj.insert("reach", ui->reach_comboBox->currentText());
    obj.insert("duration", ui->duration_spinBox->value());
    obj.insert("mana_cost", ui->mana_cost_spinBox->value());
    obj.insert("rage_aggro", ui->rage_aggro_spinBox->value());
    obj.insert("photo", ui->photo_comboBox->currentText());

    // output attak json
    QJsonDocument doc(obj);
    QTextStream out(&file);
    out << doc.toJson() << "\n";
  }
}

void EditAttakView::UpdateValues(const AttaqueType &selectedAttak)
{
    ui->name_lineEdit->setText(selectedAttak.name);
    ui->target_comboBox->setEnabled(true);
    for (int i = 0; i < static_cast<int>(TargetType::enumSize); i++) {
        ui->target_comboBox->addItem(
            Character::GetTargetString(static_cast<TargetType>(i)));
    }
    for (int i = 0; i < static_cast<int>(ReachType::enumSize); i++) {
        ui->reach_comboBox->addItem(
            Character::GetReachString(static_cast<ReachType>(i)));
    }
    ui->duration_spinBox->setValue(selectedAttak.turnsDuration);
    ui->rage_aggro_spinBox->setValue(selectedAttak.aggroCum);
    ui->mana_cost_spinBox->setValue(selectedAttak.manaCost);

    // List all attak png string and add them to photo_comboBox
    QString directoryPath = "./offlines/attak"; // Replace with the actual path
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
  const AttaqueType &selectedAttak =
      app.m_GameManager->m_PlayersManager->m_SelectedHero->attakList[idx];
  for (int i = 0; i < ui->values_form_lay->count(); i++) {
    auto *widget = ui->values_form_lay->itemAt(i)->widget();
    if (widget != nullptr) {
      widget->setEnabled(true);
    }
  }
  ui->apply_button->setEnabled(false);

  // update values with the ones from the current selected character
  UpdateValues(selectedAttak);
}

// form layout value changed

void EditAttakView::on_photo_comboBox_currentTextChanged(const QString &arg1) {
  // Update image character
  auto qp = QPixmap("./offlines/attak/" + arg1);
  ui->img_char->setPixmap(qp);
}

void EditAttakView::on_name_lineEdit_textChanged(const QString &arg1) {
  ui->apply_button->setEnabled(true);
}

void EditAttakView::on_target_comboBox_currentIndexChanged(int index) {
  ui->apply_button->setEnabled(true);
}

void EditAttakView::on_reach_comboBox_currentIndexChanged(int index) {
  ui->apply_button->setEnabled(true);
}

void EditAttakView::on_duration_spinBox_valueChanged(int arg1) {
  ui->apply_button->setEnabled(true);
}

void EditAttakView::on_rage_aggro_spinBox_valueChanged(int arg1) {
  ui->apply_button->setEnabled(true);
}

void EditAttakView::on_mana_cost_spinBox_valueChanged(int arg1) {
  ui->apply_button->setEnabled(true);
}

void EditAttakView::on_photo_comboBox_currentIndexChanged(int index) {
  ui->apply_button->setEnabled(true);
}
