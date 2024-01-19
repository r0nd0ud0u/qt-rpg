#include "equipmentview.h"
#include "ui_equipmentview.h"


#include "Application.h"

#include "gamedisplay.h"

EquipmentView::EquipmentView(QWidget *parent) : QWidget(parent), ui(new Ui::EquipmentView) {
    ui->setupUi(this);
    ui->equipment_table->setModel(createStatsModel(parent, ""));

    connect((GameDisplay*)parentWidget(), &GameDisplay::selectCharacter, this,
            &EquipmentView::UpdateStats);
}

EquipmentView::~EquipmentView() { delete ui; }

void EquipmentView::addStatRow(QAbstractItemModel *model, const QString &subject,
                           const int sender) const {
    model->insertRow(0);
    model->setData(model->index(0, 0), subject);
    model->setData(model->index(0, 1), sender);
}

QAbstractItemModel *EquipmentView::createStatsModel(QObject *parent,
                                                QString playerName) {
    auto *model = new QStandardItemModel(0, 2, parent);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Partie du corps"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Equipement"));

    const auto &app = Application::GetInstance();
    if (app.m_GameManager != nullptr &&
        app.m_GameManager->m_PlayersManager != nullptr) {

        const auto &heroList = app.m_GameManager->m_PlayersManager->m_HeroesList;

        // Case hero list empty
        if (heroList.empty()) {
            return model;
        }
        int indexPlayer = 0;
        if (!playerName.isEmpty()) {
            // lambda function to add here
            for(int i = 0; i < heroList.size(); i++){
                if(heroList[i]->m_Name == playerName){
                    indexPlayer = i;
                    break;
                }
            }
        }

        const auto &hero = heroList[indexPlayer];
        addStatRow(model, "ArmPhy", hero->m_Stats.m_ArmPhy);
        addStatRow(model, "ArmMag", hero->m_Stats.m_ArmMag);
        addStatRow(model, "PowPhy", hero->m_Stats.m_powPhy);
        addStatRow(model, "PowMag", hero->m_Stats.m_powMag);
        addStatRow(model, "Aggro", hero->m_Stats.m_aggro);
        addStatRow(model, "Speed", hero->m_Stats.m_speed);
        addStatRow(model, "Crit", hero->m_Stats.m_CriticalStrike);
        addStatRow(model, "Dodge", hero->m_Stats.m_dogde);
        addStatRow(model, "regenHP", hero->m_Stats.m_regenHP);
        addStatRow(model, "regenMana", hero->m_Stats.m_regenMana);
        addStatRow(model, "regenVigor", hero->m_Stats.m_regenVigueur);
    }

    return model;
}

void EquipmentView::UpdateStats(QString name) {
    ui->equipment_table->setModel(createStatsModel(parentWidget(),name));
}
