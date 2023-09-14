#include "mythread.h"

#include <QString>
#include <QDebug>
MyThread::MyThread(QObject *parent) : QObject(parent)
{
    qDebug() << "子线程号_构造函数：" << QThread::currentThread();
}

MyThread::~MyThread(){
    qDebug() << "子线程号_析构函数：" << QThread::currentThread();
}
/**
 * @brief MyThread::handlePlayPosition
 * @param musicTime
 * 计算当前播放位置的时间并返回字符串
 */
void MyThread::handlePlayPosition(const int &musicTime){
    //    qDebug() << "子线程号_Position：" << QThread::currentThread();
    int h,m,s;
    h = musicTime/3600;
    m = (musicTime-h*3600)/60;
    s = musicTime-h*3600-m*60;
    time1 = QString("%1:%2:%3").arg(h).arg(m).arg(s);
    emit completePosition(time1);
}
/**
 * @brief MyThread::handlePlayDuration
 * @param musicTime
 * 计算总时间然后返回
 */
void MyThread::handlePlayDuration(const int &musicTime){
    if(musicTime != time){
        time = musicTime;
        int h,m,s;
        h = musicTime/3600;
        m = (musicTime-h*3600)/60;
        s = musicTime-h*3600-m*60;
        QString time2 = QString("%1:%2:%3").arg(h).arg(m).arg(s);   //计算总时间
        qDebug() << "子线程号_handlePlayDuration：" << QThread::currentThread();
        emit completeDuration(time2);
    }
}
