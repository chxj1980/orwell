#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <boost/thread.hpp>
std::string newCameraUtil(std::string ip);
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)//,
    //ui(new Ui::MainWindow)
{
    xVideoWidget = new XVideoWidget(parent);
    xVideoWidget->resize(1280, 720);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::red);
    xVideoWidget->setAutoFillBackground(true);
    xVideoWidget->setPalette(pal);
    MediaStream* camera1 = new MediaStream(newCameraUtil("192.168.0.103"));
    camera1->setFrameUpdater((FrameUpdater *)xVideoWidget);
    //camera1->run();
    this->layout()->addWidget(xVideoWidget);
    boost::thread mediaThread(&MediaStream::run, camera1);
    //ui->setupUi(this);
   
    //mediaStream.receiveFrame();
    //VideoSurface* videoSurface = new VideoSurface(mediaStream);
    //std::cout << "gonna setVideoOutput"  << std::endl;

    //player.setVideoOutput(videoSurface);
    //w.show();
    //const size_t bufferSize = 398304;
    //uint8_t frameBuffer[bufferSize];

    //player.play();
    
    /*
    QFFmpegPlayer::Init();
    player=new QFFmpegPlayer();
    player->play();
    */
    //XVideoWidget *xVideoWidget = new XVideoWidget();
    //player->setGLWidget(gl);
}
/*
void MainWindow::setVideoSurface(VideoSurface* videoSurface) {
    videoWidget->setVideoOutput(videoSurface);
    ///player->setVideoOutput(videoWidget);
}
*/

void MainWindow::run()
{
    mediaStream->receiveFrame();
}


MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
