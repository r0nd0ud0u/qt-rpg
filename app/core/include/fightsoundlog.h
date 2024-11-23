#ifndef FIGHTSOUNDLOG_H
#define FIGHTSOUNDLOG_H

#include <QObject>

#include <memory>

#include <QMediaPlayer>

class FightSoundLog
{
public:
    FightSoundLog();
    // logs
    static QString OutputDodge(const QString& name, const QString& randType);
    // sounds
    void LaunchFailAtkOnBossSound(const bool isBoss);

    std::unique_ptr<QMediaPlayer> m_MediaPlayer = nullptr;
};

#endif // FIGHTSOUNDLOG_H
