#include "stuff.h"

#include "Application.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QMessageBox>

void EditStuff::SaveStuffInJson(const EditStuff &es, const QString &bodyPart) {
  if (es.m_Name.isEmpty()) {
    return;
  }
  QFile file;
  QDir logDir;
  const auto * gm = Application::GetInstance().m_GameManager.get();
  if(gm == nullptr || gm->m_GameState == nullptr){
      return;
  }
  // init json doc
  QJsonObject obj;

  obj.insert(EQUIP_NAME, es.m_Stuff.m_Name);
  obj.insert(EQUIP_UNIQUE_NAME, es.m_Stuff.m_UniqueName);
  obj.insert(EQUIP_CATEGORY, bodyPart);
  for (const auto &stats : ALL_STATS) {
    if (es.m_Stuff.m_Stats.m_AllStatsTable.count(stats) == 0) {
      continue;
    }
    const auto &equipStats = es.m_Stuff.m_Stats.m_AllStatsTable.at(stats);
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
  const QString filepath = gm->GetEquipmentPath(es.m_Stuff.m_IsLoot);
  const QString equipFilePath = filepath + bodyPart + "/" + es.m_Name + ".json";
  QString dirTmp = QFileInfo(equipFilePath).absoluteFilePath();
  logDir.mkpath(QFileInfo(equipFilePath).absolutePath());
  file.setFileName(equipFilePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    Application::GetInstance().log(" Could not open the file for writing " +
                                   equipFilePath);
  }
  QTextStream out(&file);
#if QT_VERSION_MAJOR == 6
  out.setEncoding(QStringConverter::Encoding::Utf8);
#else
  out.setCodec("UTF-8");
#endif
  out << doc.toJson() << "\n";
}
