#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedSize(QSize(1280, 720));
    ui->setupUi(this);

    ui->hiraganaChart->hide();
    ui->katakanaChart->hide();
    ui->nextPage->setFlat(true);
    ui->previousPage->setFlat(true);

    initVideoPlayer();

    readSongs();
    initSongList();
    printDynamicStructure();
    // vytvoreni OnClick funkci pro dynamicke widgety, ktere jsem vytvoril v initSongList();
    for (unsigned long long int i=0; i<mSongRecords.size(); i++){
        connect(mSongRecords[i]->button, &QPushButton::clicked, this, &MainWindow::songButtonClicked);
    }
    on_homeButton_clicked(); 

//    QFont font = ui->pageLabel->font();
//    font.setPointSize(8);
//    ui->pageLabel->setFont(font);
qDebug() << 64 << 65;
}

void MainWindow::songButtonClicked()
{
    delete mMediaPlayer;
    delete mVideoWidget;
    initVideoPlayer();

    QObject *senderObj = sender(); // senderObject = prislusny songButton (propojeno v konstruktoru pres connect)
    unsigned long long index = senderObj->objectName().toULongLong(); // object name = index kde ho najdem ve vektoru

    Song *song = new Song(mSongRecords[index]->artist.toStdString(), mSongRecords[index]->song.toStdString(), mSongRecords[index]->filBaseName.toStdString());

    QString lyrics = QString::fromStdString(song->getLyrics());

    ui->songTitle->setText(mSongRecords[index]->artist + "<br>" + mSongRecords[index]->song);
    ui->lyricsBox->setText(lyrics);
    ui->songImageSmaller->setText("<img src=\":/imgs/Songs/" + mSongRecords[index]->filBaseName + ".jpg\" width=\"150\" height=\"150\"/ > ");

    // videa s relativni cestou se musi prehravat z docasne slozky z <QTemporaryDir>
    // videoFileName -> tempPath;
    QString videoFileName = ":/videa/Songs/" + mSongRecords[index]->filBaseName + ".mp4";
    QTemporaryDir temporaryDir;
    QFile::copy(videoFileName, temporaryDir.path() + mSongRecords[index]->filBaseName + ".mp4");
    QString tempPath = QString(temporaryDir.path() + mSongRecords[index]->filBaseName + ".mp4");

    mMediaPlayer->setMedia(QUrl::fromLocalFile(tempPath));
    mMediaPlayer->setVolume(ui->volume->value());
    mMediaPlayer->play();


    ui->hiraganaChart->hide();
    ui->katakanaChart->hide();
    on_kanjiButton_clicked();

    // TODO vyresit proc se nekdy video nenacte // zatim reseno tlacitkem reload
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::readSongs(){
    // ifstream nevidi resources, proto je musime prekopirovat do docasne slozky z <QTemporaryDir>
    QTemporaryDir temporaryDir;
    QFile::copy(":/others/Songs/01_songList.txt", temporaryDir.path() + "/01_songList.txt");
    std::ifstream input(QString(temporaryDir.path() + "/01_songList.txt").toLatin1().data());

    if (input.is_open()) {
        while (!input.eof()){
            QString artist;
            QString song;
            QString filename;
            int byte;
            byte = input.get();
            while (byte != ',') {
                artist.append(byte);
                byte = input.get();
            }
            byte = input.get();
            while (byte != ',') {
                song.append(byte);
                byte = input.get();
            }
            byte = input.get();
            while (byte != '\n' and byte != -1) { // '\n' pro konec radku, -1 pro konec souboru
                filename.append(byte);
                byte = input.get();
            }
            if(artist != "" and song != "" and filename !="") {
             SongRecord *sg = new SongRecord();
             sg->artist = artist;
             sg->song = song;
             sg->filBaseName = filename;
             mSongRecords.push_back(sg);
            }
        }
        input.close();
    }
}

void MainWindow::initSongList(){
    // Struktura: songListPages > page > layout > item > songButton
    for (unsigned int songSegment=0; songSegment<mSongRecords.size(); songSegment+=8){       
        QWidget *page = new QWidget();
        page->setObjectName("songs_page_" + QString::number(songSegment/8));

        QGridLayout *layout = new QGridLayout();
        layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        layout->setSpacing(20);
        layout->setMargin(20);
        int row=0, col=0;
        for (unsigned int i=0; i<SONGS_PER_PAGE; i++){
            if (i == SONGS_PER_PAGE/2) {
                row = 1;
                col = 0;
            }
            unsigned long long int songIndex = songSegment+i;
            if (songIndex < mSongRecords.size()){ // podminka potrebna pro posledni stranku
                QPushButton *songButton = createButton(songIndex);
                QLayoutItem *item = new QWidgetItem(songButton);
                layout->addItem(item, row, col++, 1, 1);
                mSongRecords[songIndex]->button = songButton;
            }
        }
        page->setLayout(layout);
        ui->SongListPages->addWidget(page);
        mSongPages = (songSegment/8)+1;
    }
    ui->SongListPages->setCurrentIndex(2);
    updateCurrPageLabel();
}

QPushButton* MainWindow::createButton(unsigned long long int songIndex){
    QPushButton *songButton = new QPushButton();
    songButton->setObjectName(QString::number(songIndex)); // NEMENIT! object name = jeho index ve vektoru mSongRecords
    songButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    songButton->setMaximumSize(200,200);
    songButton->setMinimumSize(200,200);
    //songButton->setGeometry(0,0,200,200);

    QString title = mSongRecords[songIndex]->song;
    QString artist = mSongRecords[songIndex]->artist;
    // pokud ma nazev pisne > 24 pismen, tak ho rozdel na 2 radky
    if (title.length() > 24) {
        QStringList wordList = title.split(" ");
        QString partOne="", partTwo="";
        for (int wordIndex=0; wordIndex<wordList.size(); wordIndex++){
            if (partOne.length() <= title.length()/2) partOne += wordList[wordIndex] + " ";
            else partTwo += wordList[wordIndex] + " ";
        }
        title = partOne + "\n " + partTwo;
    }
    songButton->setText(title + "\n by \n" + artist);
    songButton->setCursor(Qt::PointingHandCursor);
    // border-image, protoze qt nepodporuje backround-size
    songButton->setStyleSheet("QPushButton#" + songButton->objectName() +
                              "{border-image: url(\":/imgs/Songs/" + mSongRecords[songIndex]->filBaseName +
                              ".jpg\") 0 0 0 0 stretch stretch; color: rgba(0,0,0,0);}"
                              "QPushButton#" + songButton->objectName() + ":hover {"
                              "font: bold 14px; color: white; "
                              "border-image: url(\":/textures/Textures/songHover.png\");}"
                              );
    return songButton;
}

void MainWindow::on_kanjiButton_clicked()
{
    Song  *song = new Song("Mongol800", "Chiisana Koi No Uta", "Mongol800_ChiisanaKoiNoUta");
    QString qstring = ui->kanjiInput->text();
    string str = qstring.toStdString();
    vector<Kanji*> translation = song->getTranslation(str);

    QString finalOutput="";
    for (unsigned long int i=0; i<translation.size(); i++){
        if (translation[i]->jukugo != "") {
            QString jukugo = "<span style=\"color: red; font-size: 50px; \">" + QString::fromStdString(translation[i]->jukugo) + "</span>  ";
            QString jukReading = "<span style=\"color: grey; font-size: 15px; \">" + QString::fromStdString(translation[i]->jukugoReading) + "</span><br>";
            QString jukTranslation = "<span style=\"color: black; font-size: 20px; \">" +  QString::fromStdString(translation[i]->jukugoTranslation) + "</span><br>";
            finalOutput = finalOutput + jukugo + jukReading + jukTranslation;
        } else if (translation[i]->onAndKunReadings != "") {
            QString kanji = "<span style=\"color: red; font-size: 50px; \">" + QString::fromStdString(translation[i]->kanji) + "</span>  ";
            QString kanjiReading = "<span style=\"color: grey; font-size: 15px; \">" + QString::fromStdString(translation[i]->onAndKunReadings) + "</span><br>";
            QString kanjiTranslation="";
            if (translation[i]->kunTranslation != "-") kanjiTranslation = "<span style=\"color: black; font-size: 20px; \">" +  QString::fromStdString(translation[i]->kunTranslation) + "</span>";
            if (translation[i]->onTranslation != "-") kanjiTranslation = "<span style=\"color: black; font-size: 20px; \">" +  QString::fromStdString(translation[i]->onTranslation) + "</span>";
            finalOutput = finalOutput + kanji + kanjiReading + kanjiTranslation;
        } else {
            QString kanji = "<span style=\"color: red; font-size: 50px; \">" + QString::fromStdString(translation[i]->kanji) + "</span>  ";
            QString notFound = "<span style=\"color: grey; font-size: 15px; \"> Error 404: Not Found :( </span><br>";
            finalOutput = kanji + notFound;
        }
        finalOutput += "<br><br>";
    }
    ui->kanjiInfoArea->setText(finalOutput);

}

//========================================================== zasírači místa ==========================================================//

void MainWindow::on_homeButton_clicked()
{    
    mMediaPlayer->stop();
    ui->hiraganaChart->hide();
    ui->katakanaChart->hide();
    ui->stackedWidget->setCurrentIndex(1); // index 1 je pro stranku HomePage    
}

void MainWindow::on_videoProgress_sliderReleased()
{
    int value = ui->videoProgress->value();
    mMediaPlayer->setPosition(value);
}

void MainWindow::on_stopButton_clicked()
{
    mMediaPlayer->stop();
}

void MainWindow::on_volume_valueChanged(int value)
{
    mMediaPlayer->setVolume(value);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initVideoPlayer(){
    mMediaPlayer = new QMediaPlayer(this);
    mVideoWidget = new QVideoWidget(this);
    mMediaPlayer->setVideoOutput(mVideoWidget);

    auto L = static_cast<QVBoxLayout *>(ui->videoVerticalLayout);
    L->insertWidget(0, mVideoWidget);

    connect(mMediaPlayer, &QMediaPlayer::positionChanged, [&](int pos){
        ui->videoProgress->setValue(pos);
    });

    connect(mMediaPlayer, &QMediaPlayer::durationChanged, [&](int dur){
        ui->videoProgress->setMaximum(dur);
    });
};

void MainWindow::on_hiraganaButton_clicked()
{
    if (ui->hiraganaChart->isHidden()) ui->hiraganaChart->show();
    else ui->hiraganaChart->hide();
    if(ui->katakanaChart->isHidden()){
        animation = new QPropertyAnimation(ui->hiraganaChart, "geometry");
        animation->setDuration(500);
        animation->setStartValue(QRect(0,30,1280,0));
        animation->setEndValue(QRect(0,30,1280,200));
        animation->setEasingCurve(QEasingCurve::InOutCubic);
        animation->start();
    }
    ui->katakanaChart->hide();
}

void MainWindow::on_katakanaButton_clicked()
{
    if (ui->katakanaChart->isHidden()) ui->katakanaChart->show();
    else ui->katakanaChart->hide();    
    if(ui->hiraganaChart->isHidden()){
        animation = new QPropertyAnimation(ui->katakanaChart, "geometry");
        animation->setDuration(500);
        animation->setStartValue(QRect(0,30,1280,0));
        animation->setEndValue(QRect(0,30,1280,200));
        animation->setEasingCurve(QEasingCurve::InOutCubic);
        animation->start();
    }
    ui->hiraganaChart->hide();
}

void MainWindow::on_playButton_clicked()
{
    if(ui->playButton->text()=="Play"){
        mMediaPlayer->play();
        ui->playButton->setText("Pause");
        ui->playButton->setStyleSheet("QPushButton#playButton {"
                                      "border-image: url(\":/textures/Textures/pause.png\") 0 0 0 0 stretch stretch;"
                                      "color: rgba(0,0,0,0);"
                                      "background: rgba(0,0,0,0.1);}"
                                      "QPushButton#playButton:hover {"
                                      "background: rgba(0,0,0,0.2);}"
                                      );
    } else {
        mMediaPlayer->pause();
        ui->playButton->setText("Play");
        ui->playButton->setStyleSheet("QPushButton#playButton {"
                                      "border-image: url(\":/textures/Textures/play.png\") 0 0 0 0 stretch stretch;"
                                      "color: rgba(0,0,0,0);"
                                      "background: rgba(0,0,0,0.1);}"
                                      "QPushButton#playButton:hover {"
                                      "background: rgba(0,0,0,0.2);}"
                                      );
    }
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::on_backwardButton_clicked()
{
    int currVal = ui->videoProgress->value();
    mMediaPlayer->setPosition(currVal - 5000);
}

void MainWindow::on_forwardButton_clicked()
{
    int currVal = ui->videoProgress->value();
    mMediaPlayer->setPosition(currVal + 5000);
}

void MainWindow::on_nextPage_clicked(){
    ui->SongListPages->setCurrentIndex(ui->SongListPages->currentIndex()+1);
    updateCurrPageLabel();
}

void MainWindow::on_previousPage_clicked(){
    int targetPage = ui->SongListPages->currentIndex()-1; // prvni dve stranky jsou defaultni od stacked Widget a nevim jak je odstranit
    if (targetPage>=2) ui->SongListPages->setCurrentIndex(targetPage);
    updateCurrPageLabel();
}

void MainWindow::printDynamicStructure(){  // Tisk truktury songListPages: page > layout > item > songButton
    QObjectList pageList = ui->SongListPages->children();
    for(int i=0; i<pageList.size(); i++) {
        qDebug() << pageList.at(i)->objectName();
        QObjectList itemList = pageList.at(i)->children();
        for(int j=0; j<itemList.size(); j++) qDebug() << "-songButton objectName: " + itemList.at(j)->objectName();
    }
}

void MainWindow::updateCurrPageLabel(){
    ui->pageLabel->setText("page " + QString::number(ui->SongListPages->currentIndex()-1) + "/" + QString::number(mSongPages));
//    ui->pageLabel->setFont(QFont::setPixelSize(10));
}

void MainWindow::on_reloadVideoButton_clicked(){
    mMediaPlayer->stop();
    mMediaPlayer->play();
}
