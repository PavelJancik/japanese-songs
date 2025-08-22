#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QListWidgetItem>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <QTemporaryDir>
#include <QPropertyAnimation>
#include "song.h"

const int SONGS_PER_PAGE = 8;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QMediaPlayer;
class QVideoWidget;

struct SongRecord {
    QString artist;
    QString song;
    QString filBaseName;
    QPushButton *button;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QMediaPlayer *mMediaPlayer;
    QVideoWidget *mVideoWidget;
    vector<SongRecord*> mSongRecords;
    int mSongPages;
    vector<QPropertyAnimation *> animationsVector;
    QPropertyAnimation *animation;
    QParallelAnimationGroup *animationGroup;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void readSongs();
    void initVideoPlayer();
    void initSongList();
    QPushButton* createButton(unsigned long long int songIndex);
    void songButtonClicked();    
    void printDynamicStructure();
    void updateCurrPageLabel();
private slots:
    void on_hiraganaButton_clicked();
    void on_katakanaButton_clicked();
    void on_kanjiButton_clicked();
    void on_playButton_clicked();
    void on_stopButton_clicked();   
    void on_volume_valueChanged(int value);          
    void on_videoProgress_sliderReleased();
    void on_homeButton_clicked();
    void on_exitButton_clicked();
    void on_backwardButton_clicked();
    void on_forwardButton_clicked();
    void on_nextPage_clicked();
    void on_previousPage_clicked();
    void on_reloadVideoButton_clicked();
};
#endif // MAINWINDOW_H
