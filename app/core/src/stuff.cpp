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
  QString path = OFFLINE_ROOT_EQUIPMENT;
  logDir.mkpath(path);
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
  QString logFilePath =
      logDir.filePath(path + bodyPart + "/" + es.m_Name + ".json");
  file.setFileName(logFilePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    Application::GetInstance().log(" Could not open the file for writing " +
                                   logFilePath);
  }
  QTextStream out(&file);
#if QT_VERSION_MAJOR == 6
  out.setEncoding(QStringConverter::Encoding::Utf8);
#else
  out.setCodec("UTF-8");
#endif
  out << doc.toJson() << "\n";
}
