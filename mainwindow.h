#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSocketNotifier>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setSlider(int index, float value);
    void setPotar(int index, float value);
    void setButton(int index, bool clicked);

public slots:
    void onNewMidiEvent();

protected:
    int dev_fd;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
