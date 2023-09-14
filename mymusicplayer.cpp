#include "mymusicplayer.h"
MyMusicPlayer::MyMusicPlayer(QObject *parent) : QObject(parent)
{
    myPlayer = new QMediaPlayer(this);
}

void MyMusicPlayer::playMusic(QString mediaPath){
    qDebug()<<"playMusic: myPlayer->state:"<<myPlayer->state();
    if(QMediaPlayer:: PlayingState != myPlayer->state() && mediaPath != playpath ){
        playpath = mediaPath;
        myPlayer->setMedia(QUrl::fromLocalFile(playpath));
    }
    myPlayer->play();
}

void MyMusicPlayer::pauseMusic(){
    if(QMediaPlayer:: PlayingState == myPlayer->state()){
        qDebug()<<"pauseMusic: myPlayer->state:"<<myPlayer->state();
        myPlayer->pause();
    }
}
/**
 * @brief MyMusicPlayer::setSliderPlayProgress
 * 1.音乐在走的时候，进度条也跟随变换 把音乐当前的百分比值传递给进度条
 * 2.传递信号 音乐当前播放的时间位置
 * 3.传递音乐的总时间信号 让子进程处理
 */
void MyMusicPlayer::setSliderPlayProgress(){
    int sliderNumber = myPlayer->position()*100/myPlayer->duration();
    emit calculateFinished(sliderNumber);
    int musicPosition=myPlayer->position()/1000;
    emit playPosition(musicPosition);
    int musicDuration =myPlayer->duration()/1000;
    emit playDuration(musicDuration); //返回音乐总帧数 总时间
}

/**
 * @brief MyMusicPlayer::setMusicPosition
 * @param position
 * 拖动进度条，改变滑块的值，传递给音乐，改变音乐的播放位置
 */
void MyMusicPlayer::setMusicPosition(int sliderPlayValue){
    myPlayer->setPosition(myPlayer->duration()*sliderPlayValue/100); //总帧数 时长
    myPlayer->play();
}
/*设置播放器音量*/
void MyMusicPlayer::setMusicVolume(int volume)
{
    myPlayer->setVolume(volume);
}

/**
 * @brief MyMusicPlayer::isToplay
 * @return bool值
 * 判断当前是否播放
 * 如果为true 则说明正在播放 反之亦然
 */
bool MyMusicPlayer::isToplay(){
    return (QMediaPlayer:: PlayingState == myPlayer->state());
}
