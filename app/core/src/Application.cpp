#include "Application.h"

//#include "IniFileReadOnly.h"

#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>

// Constructors, destructors

static Application *g_pApp = nullptr;
Application::Application(int &argc, char **argv) : QApplication(argc, argv) {
  g_pApp = this;
}

Application::~Application() {
  delete m_GameManager;
  m_GameManager = nullptr;
}

Application &Application::GetInstance() { return *g_pApp; }

// bool Application::ReadGlobalParameters(const std::string& iniFilePath)
// {
//     IniFileReadOnly globalIni(iniFilePath);

//     if (globalIni.IsOpen())
//     {
//         // load ini file
//         m_global_ini = Parameters::ReadConfigFile(globalIni);
//         globalIni.Close();
//     }
//     else
//     {
//         log("Warning: No ini file found (at " +
//         QFileInfo(iniFilePath.c_str()).absoluteFilePath() + ") or cannot open
//         it. Default values will be used."); return false;
//     }

//     return true;
// }

void Application::Init() {
  m_GameManager = new GameManager();
  if (m_GameManager != nullptr) {
    m_GameManager->InitHeroes();
  }
  // ReadGlobalParameters("skycbt-mission-window.ini");
  // QDir logDir(QString::fromStdString(m_global_ini.GLOBAL.m_LOG_PATH));
  // logDir.mkpath(".");

  // QString logFilePath =
  // logDir.filePath(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh.mm.ss")
  // + ".log"); m_logFile.setFileName(logFilePath); if
  // (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text))
  //     QMessageBox::information(nullptr, tr("Error log file"), tr("Log file
  //     could not be created at %1. No log will be
  //     produced.").arg(logFilePath));
}

void Application::log(const QString &txt) {
  if (!m_logFile.isOpen())
    return;

  QTextStream out(&m_logFile);
  out << QDateTime::currentDateTime().toString(Qt::ISODate) << "\t   -   ";
  out << txt << "\n";
}
