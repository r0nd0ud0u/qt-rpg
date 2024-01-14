#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QFile>

//#include "Parameters.hpp"
#include "gamemanager.h"

#include <memory>

class Application : public QApplication
{
    Q_OBJECT
    // Constructors, destructors
public:
    Application(int& argc, char** argv);
    static Application& GetInstance(); ///< Get singleton
    virtual ~Application();

    static std::string ini_global_location;

    void Init();
   // const Parameters& GetParameters() const { return m_global_ini; }
    void log(const QString& txt);

    std::unique_ptr<GameManager> m_GameManager;

private:
    Application(const Application&) = delete;
    Application(Application&&)      = delete;
    Application& operator=(const Application&) = delete;

    bool ReadGlobalParameters(const std::string& iniFilePath);

    QFile m_logFile;
  //  Parameters m_global_ini;

};

// inline const Parameters& IniParam()
// {
//     return Application::GetInstance().GetParameters();
// }
#endif // APPLICATION_H
