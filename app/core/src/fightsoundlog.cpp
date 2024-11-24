#include "fightsoundlog.h"

#include "common.h"

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
  AddToQueue(soundName);
}

void FightSoundLog::AddToQueue(const QString &soundName) {
  if (soundName.isEmpty()) {
    return;
  }
  QString soundPath = SOUNDS_DIR + soundName;
  m_Queue.enqueue(soundPath);
  qDebug() << "add to queue-enqueue " << soundPath;
  if (!m_IsPlaying && !m_Queue.isEmpty()) {
    qDebug() << "add to queue-play next " << soundPath;
    playNext();
  }
}

void FightSoundLog::onStateChanged(QMediaPlayer::PlaybackState state) {
  qDebug() << "onStateChanged" << state;
  if (state == QMediaPlayer::StoppedState) {
    qDebug() << "onStateChanged " << m_IsPlaying;
    m_IsPlaying = false;
  }
  if (state == QMediaPlayer::StoppedState && !m_Queue.isEmpty()) {
    qDebug() << "onStateChanged-play next ";
    playNext();
  }
}

void FightSoundLog::playNext() {
  if (!m_Queue.isEmpty()) {
    QString nextSound = m_Queue.dequeue();
    qDebug() << "playNext " << nextSound;
    m_MediaPlayer->setSource(QUrl::fromLocalFile(nextSound));
    if (m_AudioOutput == nullptr || m_MediaPlayer->audioOutput() == nullptr) {
      m_AudioOutput = new QAudioOutput();
      m_MediaPlayer->setAudioOutput(m_AudioOutput);
      m_AudioOutput->setVolume(1.0f);
      qDebug() << "set audiooutput ";
    }
    m_MediaPlayer->play();
    m_IsPlaying = true;
    qDebug() << "playNext " << m_IsPlaying;
  }
}
