#ifndef FIGHTSOUNDLOG_H
#define FIGHTSOUNDLOG_H

#include <QObject>

#include <memory>

#include <QMediaPlayer>
#include <QAudioOutput>

class FightSoundLog
{
public:
    FightSoundLog();
    // logs
    static QString OutputDodge(const QString& name, const QString& randType);
    // sounds
    void PlayDodgingSound(const bool isBoss);

    std::unique_ptr<QMediaPlayer> m_MediaPlayer = nullptr;
    std::unique_ptr<QAudioOutput> m_AudioOutput = nullptr;
};

#endif // FIGHTSOUNDLOG_H
