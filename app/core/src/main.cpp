#include "mainwindow.h"

#include "Application.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<Application> app = std::make_unique<Application>(argc, argv);
    app->Init();

    MainWindow w;
    w.show();
    return app->exec();
}
