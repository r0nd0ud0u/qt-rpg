#include "mainwindow.h"

#include "Application.h"

#include "Stylizer.h"

int main(int argc, char *argv[])
{
    auto app = std::make_unique<Application>(argc, argv);
    app->Init();

    Stylizer::SetTheme(StylizerSelector::THEMES::LIGHT);

    MainWindow w;
    w.show();
    return app->exec();
}
