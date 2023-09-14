#ifndef MYMUSICPLAYER_H
#define MYMUSICPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QString>
#include <QDebug>

class MyMusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MyMusicPlayer(QObject *parent = nullptr);

    void playMusic(QString mediaPath); //播放
    void pauseMusic();  //暂停
    void setMusicPosition(int sliderPlayValue); //设置音乐播放当前位置
    void setMusicVolume(int volume);    //设置音量
    bool isToplay(); //判断现在是否在播放

public slots:
    void setSliderPlayProgress();//水平滑块播放进度

signals:
 void calculateFinished(const int &number);
 void playPosition(const int &musicPositon);
 void playDuration(const int &musicDuration);

private:
 QMediaPlayer *myPlayer;
 QString playpath; //路径判断，如果不相等则更换路径，否则继续播放

};

#endif // MYMUSICPLAYER_H
