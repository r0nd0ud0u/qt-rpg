#include "statsendgame.h"

#include "Application.h"

#include <QFile>
#include <QDir>

void StatsEndGame::GenerateStatsEndGame(){
    // output
    QFile file;
    QDir logDir;
    logDir.mkpath(OUTPUT_DIR);
    QString logFilePath = logDir.filePath(OUTPUT_ENDGAME);
    file.setFileName(logFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        Application::GetInstance().log(" Could not open the file for writing " +
                                       logFilePath);
    }
    QTextStream out(&file);
    out << "test" << "\n";
}
