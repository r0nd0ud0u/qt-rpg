#include "Application.h"
#include "ApplicationView.h"
#include "Stylizer.h"

#include "rust-rpg-bridge/lib.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    // test rust lib with vec
    Shared test;
    test.v = 50;
    rust::Vec<Shared> toto = rusty_cxxbridge_vector();
    test = toto.at(0);
    qDebug() <<"" << test.v;

    auto app = std::make_unique<Application>(argc, argv);
    app->Init();

    Stylizer::SetTheme(StylizerSelector::THEMES::LIGHT);

    ApplicationView::GetInstance().StartMainWindow();
    ApplicationView::GetInstance().StartCharacterWindow();

    return app->exec();
}
