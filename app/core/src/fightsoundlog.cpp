#include "fightsoundlog.h"

#include "common.h"

#include <QAudioOutput>
#include <QMediaPlayer>

FightSoundLog::FightSoundLog() {
  m_MediaPlayer = std::make_unique<QMediaPlayer>();
  auto audioOutput = std::make_unique<QAudioOutput>();
  m_MediaPlayer->setAudioOutput(audioOutput.get());
}

QString FightSoundLog::OutputDodge(const QString &name,
                                   const QString &randType) {
  return QString("%1 esquive.(%2)").arg(name, randType);
}

void FightSoundLog::LaunchFailAtkOnBossSound(const bool isBoss) {
  if (!isBoss) {
    return;
  }
  m_MediaPlayer->setSource(QUrl::fromLocalFile(SOUNDS_DIR + "goat.wav"));
  m_MediaPlayer->audioOutput()->setVolume(50);
  m_MediaPlayer->play();
}
