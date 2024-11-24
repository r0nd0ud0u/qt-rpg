#include "fightsoundlog.h"

#include "common.h"

FightSoundLog::FightSoundLog() {
  m_MediaPlayer = std::make_unique<QMediaPlayer>();
  m_AudioOutput = std::make_unique<QAudioOutput>();
  m_MediaPlayer->setAudioOutput(m_AudioOutput.get());
  m_AudioOutput->setVolume(50.);
}

QString FightSoundLog::OutputDodge(const QString &name,
                                   const QString &randType) {
  return QString("%1 esquive.(%2)").arg(name, randType);
}

void FightSoundLog::PlayDodgingSound(const bool isBossDodging) {
  if (m_MediaPlayer == nullptr) {
    return;
  }
  QString soundName;
  if (isBossDodging) {
    soundName += "on-boss-dodging.wav";
  } else {
    soundName += "on-hero-dodging.ogg";
  }
  LaunchSound(soundName);
}

void FightSoundLog::LaunchSound(const QString &soundName) {
  if (soundName.isEmpty()) {
    return;
  }
  QString soundPath = SOUNDS_DIR + soundName;
  m_MediaPlayer->setSource(soundPath);
  m_MediaPlayer->play();
}
