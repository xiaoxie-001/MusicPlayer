#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H

#include <QWidget>
#include "mythread.h"
#include "mymusicplayer.h"
#include <QThread>
#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QMovie>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QtDebug>
#include <QFileDialog>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MusicWidget; }
QT_END_NAMESPACE

class MusicWidget : public QWidget
{
    Q_OBJECT

public:
    MusicWidget(QWidget *parent = nullptr);
    ~MusicWidget();
    void UI_Design();
    void movieToLoad(); //加载gif
    void setPreOrNextPlay();//当上一曲或者下一曲播放时调用


public slots:
    void on_pushButtonMini_clicked();   //最小化按钮
    void on_pushButtonDestroy_clicked();//关闭按钮
    void updateCurrent(); //更新时间

    void on_btnPause_clicked();
    void on_btnNext_clicked();
    void on_btnLast_clicked();
    void sethSliderValue(const int &number);

    void on_hSliderPlayProgress_valueChanged(int value);
    void on_hSliderPlayProgress_sliderReleased();
    void on_vSliderVolume_valueChanged(int value);
    void on_vSliderVolume_sliderReleased();
    void on_btnVolume_clicked();
    void on_btnAddMusic_clicked();
    void on_lwMusicList_itemDoubleClicked(QListWidgetItem *item); //双击播放
    void on_lwMusicList_itemClicked(QListWidgetItem *item); //单击选中
    void showMusicPosition(QString time);
    void showMusicDuration(QString time);
private:
    Ui::MusicWidget *ui;

    QPoint startPos;//鼠标点击时的全局位置
    QPoint startPos1;//鼠标点击时的相对位置
    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标追踪


    MyMusicPlayer *myMediaPlayer;
    MyThread *myT;
    QThread *thread;



    QMovie *movie;          //背景动画gif
    QMovie *movie3;
    QMovie *movie4;

    QTimer *timer;
    QTimer *timer2; //读取播放器进度条的值
    int sliderPlayValue; //记录读取进度条的值
    QString mediaPath;//存储当前音乐文件的目录加文件名
    QHash<QString,int> musicFileIndex; //添加一个成员变量来保存音乐文件路径和索引的映射
    int valueindex; //用于判断当前上一曲或下一曲的索引value值
    int countFiles=0;//hash key的键值value 计算添加的文件个数
    int playtime; //显示当前歌曲进度时间

};
#endif // MUSICWIDGET_H
