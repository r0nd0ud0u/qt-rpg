#include "Application.h"
#include "ApplicationView.h"

#include "Stylizer.h"

int main(int argc, char *argv[])
{
    auto app = std::make_unique<Application>(argc, argv);
    app->Init();

    Stylizer::SetTheme(StylizerSelector::THEMES::LIGHT);

    ApplicationView::GetInstance().StartMainWindow();
    ApplicationView::GetInstance().StartCharacterWindow();

    return app->exec();
}
