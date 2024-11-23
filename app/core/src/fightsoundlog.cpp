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
  QString soundsPath = SOUNDS_DIR;
  if (isBossDodging) {
    soundsPath += "on-boss-dodging.wav";
  } else {
    soundsPath += "on-hero-dodging.ogg";
  }
  m_MediaPlayer->setSource(soundsPath);

  m_MediaPlayer->play();
}
