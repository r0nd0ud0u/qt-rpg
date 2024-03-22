#include "Application.h"
#include "ApplicationView.h"

#include "Stylizer.h"

#include "rusty_bridge/lib.h"
#include <iostream>
#include <QtDebug>

extern "C" {
int32_t rusty_extern_c_integer();
}

int main(int argc, char *argv[])
{
    qDebug() << "A value given via generated cxxbridge "
              << rusty_cxxbridge_integer() << "\n";
    qDebug() << "A value given directly by extern c function "
              << rusty_extern_c_integer() << "\n";

    return 0;
    auto app = std::make_unique<Application>(argc, argv);
    app->Init();

    Stylizer::SetTheme(StylizerSelector::THEMES::LIGHT);

    ApplicationView::GetInstance().StartMainWindow();
    ApplicationView::GetInstance().StartCharacterWindow();

    return app->exec();
}
