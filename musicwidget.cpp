#include "musicwidget.h"
#include "ui_musicwidget.h"

MusicWidget::MusicWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicWidget)
{
    ui->setupUi(this);
    UI_Design();
    timer = new QTimer(this);  //定时器显示获取当前时间
    connect(timer, &QTimer::timeout, this, &MusicWidget::updateCurrent);
    timer->start(1000);

    myMediaPlayer = new MyMusicPlayer;
    connect(myMediaPlayer, &MyMusicPlayer::calculateFinished, this, &MusicWidget::sethSliderValue);

    timer2 = new QTimer(this);  //定时器0.5s获取一次播放进度条 刷新进度条
    connect(timer2, &QTimer::timeout, myMediaPlayer, &MyMusicPlayer::setSliderPlayProgress);

    myT = new MyThread; //动态分配空间，不能指定父对象
    thread = new QThread(this);//创建子线程
    myT->moveToThread(thread);//把自定义线程加入到子线程中
    connect(myMediaPlayer, &MyMusicPlayer::playPosition, myT, &MyThread::handlePlayPosition);
    connect(myMediaPlayer, &MyMusicPlayer::playDuration, myT, &MyThread::handlePlayDuration);
    connect(myT, &MyThread::completePosition, this, &MusicWidget::showMusicPosition);
    connect(myT, &MyThread::completeDuration, this, &MusicWidget::showMusicDuration);
    qDebug() << "主线程号：" << QThread::currentThread();
    thread->start();
}

MusicWidget::~MusicWidget()
{
    thread->quit(); //停止线程
    thread->wait(); //等待线程处理完手头动作
    delete myMediaPlayer;
    delete myT;
    delete ui;
}

void MusicWidget::updateCurrent() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QString nowDate = currentTime.toString("yyyy-MM-dd dddd");
    QString nowTime = currentTime.toString("hh:mm:ss");
    ui->label_currentTime->setText(nowTime);
    ui->label_currentDate->setText(nowDate);
}
void MusicWidget::UI_Design() {
    this->setWindowTitle("xtc天气");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);  //窗口为无边框，同时保留系统菜单和最小化按钮

    ui->btnPause->setIcon(QIcon(":/Image/pause.png"));
    ui->btnPause->setStyleSheet( "QPushButton{background:rgba(255,255,255,0.2);border-radius:25px;padding:5px 5px}");
    ui->btnVolume->setStyleSheet( "QPushButton{background:rgba(255,255,255,0.2);border-radius:25px;padding:5px 5px}");
    ui->btnLast->setStyleSheet( "QPushButton{background:rgba(255,255,255,0.2);border-radius:25px;padding:5px 5px}");
    ui->btnNext->setStyleSheet("QPushButton{background:rgba(255,255,255,0.2);border-radius:25px;padding:5px 5px}");
    ui->btnAddMusic->setStyleSheet("QPushButton::hover{font:19px;}" "QPushButton{background:rgba(255,255,225,0.2);color:#FFDEAD;border-radius:16px;padding:5px 5px}");

    ui->pushButtonMini->setIcon(QIcon(":/Image/Min.png"));   //最小化按钮
    ui->pushButtonMini->setStyleSheet("QPushButton{border:none;color:rgb(122, 197, 205);}" "QPushButton:hover{background-color: #1E90FF;border:none;color:rgb(255, 255, 255);}");
    ui->pushButtonDestroy->setIcon(QIcon(":/Image/Close.png")); //关闭按钮
    ui->pushButtonDestroy->setStyleSheet("QPushButton{border:none;color:rgb(122, 197, 205);}" "QPushButton:hover{background-color: #FF0000;border:none;color:rgb(255, 255, 255);}");

    ui->vSliderVolume->setVisible(false);
    ui->lwMusicList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//关闭水平滚动条
    movieToLoad(); //加载gif
}
/**
 * @brief MusicWidget::movieToLoad
 * 显示一些gif动画效果
 */
void MusicWidget::movieToLoad() {
    movie = new QMovie(":/Image/timg07.gif");   //设置GIF格式背景图
    ui->label_background->setMovie(movie);
    movie->start();
    movie3 = new QMovie(":/Image/haimianbaby.gif");
    movie3->setScaledSize(QSize(55, 60)); // 设置GIF动画的尺寸与QLabel的尺寸相同
    ui->label_haimianbaby->setMovie(movie3);
    movie3->start();
    movie4 = new QMovie(":/Image/paidaxing.gif");
    movie4->setScaledSize(QSize(55, 60)); // 设置GIF动画的尺寸与QLabel的尺寸相同
    ui->label_paidaxing->setMovie(movie4);
    movie4->start();
}


//记录拖动起始位置
void MusicWidget::mousePressEvent(QMouseEvent *event) {
    startPos = event->globalPos();//获取点击鼠标时的全局坐标
    startPos1 = event->pos();//获取点击鼠标的相对MusicWidget的坐标
}

//窗口移动
void MusicWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) { //判断是否是左键按下
        QPoint movePos = event->globalPos() - startPos;//计算鼠标移动位置
        if ( movePos.manhattanLength() > 4) //判断移动像素距离，防抖动
            this->move(event->globalPos() - startPos1);
        return;
    }
}
/**
 * @brief MusicWidget::on_pushButtonMini_clicked
 *  窗口最小化按钮
 */
void MusicWidget::on_pushButtonMini_clicked() {
    showMinimized();
    //showMaximized();//最大化
}
/**
 * @brief MusicWidget::on_pushButtonDestroy_clicked
 * 窗口关闭按钮
 */
void MusicWidget::on_pushButtonDestroy_clicked() {
    this->close();
}


/**
 * @brief MusicWidget::on_btnAddMusic_clicked
 * 添加音乐文件列表
 */
void MusicWidget::on_btnAddMusic_clicked() {
    QFileDialog fileDialog(this);//创建文件对话框
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setNameFilter("Music Files (*.mp3 *.wav *.wma)");
    if (fileDialog.exec()) { //获取要添加的列表
        QStringList fileNames = fileDialog.selectedFiles();
        QString fileName = fileNames[0];
        ui->lwMusicList->addItem(fileName); //将每个文件添加到QListWidget控件
        musicFileIndex[fileName] = countFiles + 1; // 将索引he文件路径添加到映射中
        qDebug() << "value:" << musicFileIndex.value(fileName) << "key:" << musicFileIndex.key(countFiles + 1);
    }
    countFiles = ui->lwMusicList->count();
    qDebug() << "总添加音乐count:" << countFiles;
}

/**
 * @brief MusicWidget::on_btnPause_clicked
 * 播放和暂停
 */
void MusicWidget::on_btnPause_clicked() {
    if (this->mediaPath.isNull()) {
        return;
    } else {
        if (myMediaPlayer->isToplay()) { //为真 true 正在播放,将其关闭
            ui->btnPause->setIcon(QIcon(":/Image/pause.png"));
            myMediaPlayer->pauseMusic();
            timer2->stop();
        } else { //false 没有播放，点击后播放
            ui->btnPause->setIcon(QIcon(":/Image/stop.png"));
            myMediaPlayer->playMusic(this->mediaPath);
            timer2->start(500);
        }
    }
}

/**
 * @brief MusicWidget::sethSliderValue
 * @param number
 * 设置进度条 当歌曲播放时，返回当前播放进度
 */
void MusicWidget::sethSliderValue(const int &number) {
    ui->hSliderPlayProgress->setValue(number);
}

/**
 * @brief MusicWidget::on_hSliderPlayProgress_valueChanged
 * @param value
 * 当滑动滑块时关闭播放，并且关闭进度条刷新定时器
 *
 * 目前设置的只有滑动有效，直接点击跳转位置无效
 */
void MusicWidget::on_hSliderPlayProgress_valueChanged(int value) {
    if (ui->hSliderPlayProgress->isSliderDown()) { //isSliderDown判断滑块是否被按下
        ui->btnPause->setIcon(QIcon(":/Image/pause.png"));
        myMediaPlayer->pauseMusic(); //关闭播放
        if (timer2->isActive()) {
            timer2->stop();//关闭定时器
        }
        // 记录滑块的值
        this->sliderPlayValue = value;
    }
}

/**
 * @brief MusicWidget::on_hSliderPlayProgress_sliderReleased
 * 当释放滑块后读取现在的值
 */
void MusicWidget::on_hSliderPlayProgress_sliderReleased() {
    if (mediaPath.isNull()) {
        return;
    }
    myMediaPlayer->setMusicPosition(this->sliderPlayValue); //滑块的值传递改变音乐位置
    ui->btnPause->setIcon(QIcon(":/Image/stop.png")); //开始播放在setMusicPosition中完成
    timer2->start(500);
}

/**
 * @brief MusicWidget::on_vSliderVolume_valueChanged
 * @param value
 * 设置音乐播放器的音量
 */
void MusicWidget::on_vSliderVolume_valueChanged(int value) {
    myMediaPlayer->setMusicVolume(value);
}

/**
 * @brief MusicWidget::on_btnVolume_clicked
 * 点击按钮设置音量条显示
 */
void MusicWidget::on_btnVolume_clicked() {
    if (ui->vSliderVolume->isVisible()) {
        ui->vSliderVolume->setVisible(false);
    } else {
        ui->vSliderVolume->setVisible(true);
    }
}
/**
 * @brief MusicWidget::on_vSliderVolume_sliderReleased
 * 音量条设置好后隐藏
 */
void MusicWidget::on_vSliderVolume_sliderReleased() {
    ui->vSliderVolume->setVisible(false);
}

/**
 * @brief MusicWidget::on_lwMusicList_itemDoubleClicked
 * @param item
 * 双击wMusicList清单播放
 */
void MusicWidget::on_lwMusicList_itemDoubleClicked(QListWidgetItem *item) {
    timer2->stop();
    this->mediaPath = item->text();
    qDebug() << "双击成功" << mediaPath << endl;
    myMediaPlayer->pauseMusic();
    ui->btnPause->setIcon(QIcon(":/Image/stop.png"));
    myMediaPlayer->playMusic(this->mediaPath);
    timer2->start(500);
}
/*单击选中*/
void MusicWidget::on_lwMusicList_itemClicked(QListWidgetItem *item) {
    this->mediaPath = item->text();
}

/**
 * @brief MusicWidget::on_btnLast_clicked
 * 上一首
 */
void MusicWidget::on_btnLast_clicked() {
    if (this->mediaPath.isNull()) {
        return;
    }
    QListWidgetItem *item = ui->lwMusicList->item(musicFileIndex.value(mediaPath) - 1);
    item->setSelected(false);        //设置是否选中 选中则有高亮 切换歌曲的时候当前这个高亮关闭

    valueindex = musicFileIndex.value(mediaPath) - 1; //通过哈希表的键找到对应的索引值 索引值必须大于等于0
    if (valueindex >= 1) {
        setPreOrNextPlay();
    } else {     //设置播放列表循环
        valueindex = countFiles;
        setPreOrNextPlay();
    }
}
/**
 * @brief MusicWidget::on_btnNext_clicked
 * 下一首
 */
void MusicWidget::on_btnNext_clicked() {
    if (this->mediaPath.isNull()) {
        return;
    }
    QListWidgetItem *item = ui->lwMusicList->item(musicFileIndex.value(mediaPath) - 1);
    item->setSelected(false);        //设置是否选中 选中则有高亮 切换歌曲的时候当前这个高亮关闭

    valueindex = musicFileIndex.value(mediaPath) + 1; //通过哈希表的键找到对应的索引值 索引值必须大于等于0
    if (valueindex <= countFiles) {
        qDebug() << "valueindex:" << valueindex;
        setPreOrNextPlay();
    } else {     //设置播放列表循环
        valueindex = 1;
        setPreOrNextPlay();
    }
}
/**
 * @brief MusicWidget::setPreOrNextPlay
 * 当点击上一曲/下一曲后如果成功则播放
 */
void MusicWidget::setPreOrNextPlay() {
    mediaPath = musicFileIndex.key(valueindex); //通过对应的索引值找到相应的键值
    qDebug() << "value:" << valueindex << "key:" << mediaPath;
    QListWidgetItem *item = ui->lwMusicList->item(musicFileIndex.value(mediaPath) - 1);
    item->setSelected(true);        //设置是否选中 选中则有高亮

    timer2->stop();
    myMediaPlayer->pauseMusic();
    //先关闭，然后播放
    ui->btnPause->setIcon(QIcon(":/Image/stop.png"));
    myMediaPlayer->playMusic(this->mediaPath);
    timer2->start(500);
}
/**
 * @brief MusicWidget::setMusicTime
 * @param musicTime
 * 显示音乐播放进行时 当前时间
 */
void MusicWidget::showMusicPosition(QString time) {
    ui->lblPlayTime->setText(time);
}
/**
 * @brief MusicWidget::setMusicDuration
 * @param musicTime
 * 显示音乐总时间
 */
void MusicWidget::showMusicDuration(QString time) {
    ui->lblMusicTime->setText(time);
}
