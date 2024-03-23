#include "Application.h"
#include "ApplicationView.h"
#include "Stylizer.h"

#include "rusty_bridge/lib.h"

int main(int argc, char *argv[])
{
    // test rust lib with vec
    rust::Vec<Shared> toto = rusty_cxxbridge_vector();

    auto app = std::make_unique<Application>(argc, argv);
    app->Init();

    Stylizer::SetTheme(StylizerSelector::THEMES::LIGHT);

    ApplicationView::GetInstance().StartMainWindow();
    ApplicationView::GetInstance().StartCharacterWindow();

    return app->exec();
}
