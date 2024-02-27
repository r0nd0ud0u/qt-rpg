#include "stuffsview.h"
#include "ui_stuffsview.h"

#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QMessageBox>

StuffsView::StuffsView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StuffsView)
{
    ui->setupUi(this);
    InitEditStuffsView();
}

StuffsView::~StuffsView()
{
    delete ui;
}

void StuffsView::InitEditStuffsView() {

    for(const auto& stats : ALL_STATS){
        if(stats.isEmpty()){
            continue;
        }
        auto* stuff = new StuffPanel();
        stuff->Init(stats);
        if(stuff == nullptr){
            continue;
        }
        ui->scrollAreaWidgetContents->layout()->addWidget(stuff);
        m_StuffList.push_back(stuff);
    }

    for (const auto &bodyPart : ALL_EQUIP) {
        ui->body_comboBox->addItem(bodyPart);
    }

}

EditStuff StuffsView::Save() {
    QFile file;
    QDir logDir;
    QString path = OFFLINE_ROOT_EQUIPMENT;
    logDir.mkpath(path);

    // prepare result
    EditStuff  editStuff;
    editStuff.m_Name = ui->name_textEdit->toPlainText();
    editStuff.m_BodyPart = ui->body_comboBox->currentText();

    for (const auto& panel : m_StuffList) {
        auto& stat = std::get<StatsType<int>>(editStuff.m_Stuff.m_Stats.m_AllStatsTable[panel->m_Name]);
        if(panel->m_IsPercent){
            stat.m_BufEquipPercent = panel->m_BufValue;
            stat.m_BufEquipValue = 0;
        } else{
            stat.m_BufEquipValue = panel->m_BufValue;
            stat.m_BufEquipPercent = 0;
        }
    }
    m_EditStuffList.clear();
    m_EditStuffList.push_back(editStuff);

    for (const auto &es : m_EditStuffList) {
        // init json doc
        QJsonObject obj;

        obj.insert(EQUIP_NAME, es.m_Name);
        obj.insert(EQUIP_CATEGORY, ui->body_comboBox->currentText());
        for(const auto& stats: ALL_STATS){
            if(es.m_Stuff.m_Stats.m_AllStatsTable.count(stats) == 0){
                continue;
            }
            const auto& equipStats = std::get<StatsType<int>>(es.m_Stuff.m_Stats.m_AllStatsTable.at(stats));
            QJsonObject item;
            QJsonArray jsonArray;
            obj.insert(stats, equipStats.m_CurrentValue);
            item["percent"] = equipStats.m_BufEquipPercent;
            item["value"] = equipStats.m_BufEquipValue;
            jsonArray.append(item);
            if (!jsonArray.empty()) {
                obj[stats] = jsonArray;
            }
        }

        // output json
        QJsonDocument doc(obj);
        QString logFilePath =
            logDir.filePath(path + ui->body_comboBox->currentText() + "\\" + es.m_Name + ".json");
        file.setFileName(logFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::information(
                nullptr, tr("Error log file"),
                tr("Log file could not be created at %1. No log will be produced.")
                    .arg(logFilePath));
        }
        QTextStream out(&file);
#if QT_VERSION_MAJOR == 6
        out.setEncoding(QStringConverter::Encoding::Utf8);
#else
        out.setCodec("UTF-8");
#endif
        out << doc.toJson() << "\n";
    }

    return editStuff;
}

