#include "equiponbodypanel.h"
#include "ui_equiponbodypanel.h"

#include "Application.h"

EquipOnBodyPanel::EquipOnBodyPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EquipOnBodyPanel)
{
    ui->setupUi(this);
}

EquipOnBodyPanel::~EquipOnBodyPanel()
{
    delete ui;
}

void EquipOnBodyPanel::on_equip_comboBox_currentTextChanged(const QString &arg1)
{
    m_Equipment = arg1;
}

void EquipOnBodyPanel::InitComboBox(const QString& name){
    const auto& pm = Application::GetInstance().m_GameManager->m_PlayersManager;
    ui->body_label->setText(name);

    for(const auto& [equipName, equip] : pm->m_Equipments[name]){
        ui->equip_comboBox->addItem(equipName);
    }
}

void EquipOnBodyPanel::SetValue(const QString& value){
    ui->equip_comboBox->setCurrentText(value);
}

void EquipOnBodyPanel::ResetUi(){
    ui->equip_comboBox->clear();
}
