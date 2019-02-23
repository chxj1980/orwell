#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "VideoSurface.h"
#include "MediaStream.h"
#include <QMainWindow>
#include "XVideoWidget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    MediaStream* mediaStream;
public:
    explicit MainWindow(QWidget *parent = 0);
    void setVideoSurface(VideoSurface* videoSurface);
    void run();
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    //MediaStream mediaStream;
    //QMediaPlayer player;
    //QVideoWidget videoWidget;
    XVideoWidget* xVideoWidget;
};

#endif // MAINWINDOW_H
