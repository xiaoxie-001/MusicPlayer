#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>

class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);
    ~MyThread();

signals:
    void completePosition(const QString &time);
    void completeDuration(const QString &time);

public slots:
    void handlePlayPosition(const int &musicTime);
    void handlePlayDuration(const int &musicTime);
private:
    QString time1;  //当前播放时间
    int time;       //得到传送过来的总时间秒数，如果和上一次不相等则更新、否则不操作
};

#endif // MYTHREAD_H
