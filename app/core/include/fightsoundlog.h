#ifndef FIGHTSOUNDLOG_H
#define FIGHTSOUNDLOG_H

#include <QObject>
#include <QQueue>

#include <memory>

#include <QAudioOutput>
#include <QMediaPlayer>

class FightSoundLog : public QObject {
  Q_OBJECT
public:
  FightSoundLog(QObject *parent = nullptr)
        : QObject(parent), m_MediaPlayer(new QMediaPlayer(this)), m_AudioOutput(new QAudioOutput(this)) {
    // Connect to playbackStateChanged signal
    connect(m_MediaPlayer, &QMediaPlayer::playbackStateChanged, this,
            &FightSoundLog::onStateChanged);
    m_MediaPlayer->setAudioOutput(m_AudioOutput);
    m_AudioOutput->setVolume(1.);
  }
  // logs
  static QString OutputDodge(const QString &name, const QString &randType);
  // sounds
  void PlayDodgingSound(const bool isBoss);
  void AddToQueue(const QString &soundName);

private:
  QMediaPlayer *m_MediaPlayer = nullptr;
  QAudioOutput* m_AudioOutput = nullptr;
  QQueue<QString> m_Queue;
  bool m_IsPlaying = false;

  void playNext();

private slots:
  void onStateChanged(QMediaPlayer::PlaybackState state);
};

#endif // FIGHTSOUNDLOG_H
