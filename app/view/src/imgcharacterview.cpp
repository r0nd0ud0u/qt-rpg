#include "imgcharacterview.h"
#include "ui_imgcharacterview.h"

#include "Application.h"
#include "gamedisplay.h"

ImgCharacterView::ImgCharacterView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ImgCharacterView) {
  ui->setupUi(this);
  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &ImgCharacterView::UpdateView);
}

ImgCharacterView::~ImgCharacterView() { delete ui; }

void ImgCharacterView::UpdateView(QString name) {

  const auto &app = Application::GetInstance();
  if (app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr) {

    const auto &heroList = app.m_GameManager->m_PlayersManager->m_HeroesList;

    // Case hero list empty
    if (heroList.empty()) {
      return;
    }
    int indexPlayer = 0;
    if (!name.isEmpty()) {
      // lambda function to add here
      for (int i = 0; i < heroList.size(); i++) {
        if (heroList[i]->m_Name == name) {
          indexPlayer = i;
          break;
        }
      }
    }

    const auto &hero = heroList[indexPlayer];
    QPixmap qp = QPixmap(":/" + name + ".png");
    ui->img_label->setPixmap(qp);
    ui->img_label->setScaledContents(true);
  }
}
