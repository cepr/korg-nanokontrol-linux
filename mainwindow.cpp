#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MIN_SLIDER_POSITION 242.
#define MAX_SLIDER_POSITION 124.

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dev_fd = ::open("/dev/midi1", O_RDONLY);
    Q_ASSERT(dev_fd >= 0);
    QSocketNotifier *notifier = new QSocketNotifier(dev_fd, QSocketNotifier::Read, this);
    connect(notifier, SIGNAL(activated(int)), SLOT(onNewMidiEvent()));
}

MainWindow::~MainWindow()
{
    ::close(dev_fd);
    delete ui;
}

void MainWindow::setSlider(int index, float value)
{
    if (value < 0.f) {
        value = 0.f;
    } else if (value > 1.f) {
        value = 1.f;
    }

    QLabel *slider;
    switch(index) {
    case 1:
        slider=ui->slider_1;
        break;
    case 2:
        slider=ui->slider_2;
        break;
    case 3:
        slider=ui->slider_3;
        break;
    case 4:
        slider=ui->slider_4;
        break;
    case 5:
        slider=ui->slider_5;
        break;
    case 6:
        slider=ui->slider_6;
        break;
    case 7:
        slider=ui->slider_7;
        break;
    case 8:
        slider=ui->slider_8;
        break;
    case 9:
        slider=ui->slider_9;
        break;
    default:
        return;
    }

    QRect geo(slider->geometry());
    geo.moveTop(MIN_SLIDER_POSITION + value * (MAX_SLIDER_POSITION - MIN_SLIDER_POSITION));
    slider->setGeometry(geo);
}

void MainWindow::setPotar(int index, float value)
{
    if (value < 0.f) {
        value = 0.f;
    } else if (value > 1.f) {
        value = 1.f;
    }

    QLabel *potar;
    switch(index) {
    case 1:
        potar=ui->potar_1;
        break;
    case 2:
        potar=ui->potar_2;
        break;
    case 3:
        potar=ui->potar_3;
        break;
    case 4:
        potar=ui->potar_4;
        break;
    case 5:
        potar=ui->potar_5;
        break;
    case 6:
        potar=ui->potar_6;
        break;
    case 7:
        potar=ui->potar_7;
        break;
    case 8:
        potar=ui->potar_8;
        break;
    case 9:
        potar=ui->potar_9;
        break;
    default:
        return;
    }

    QPixmap pixmap(QString::fromUtf8(":/potar.png"));
    QMatrix rotation_matrix;
    rotation_matrix.rotate((0.5 - value) * 270.);
    const QPixmap & rotated_pixmap = pixmap.transformed(rotation_matrix, Qt::SmoothTransformation);
    potar->setPixmap(rotated_pixmap);
}

void MainWindow::setButton(int index, bool clicked)
{
    QLabel *button;
    switch(index) {
    case 0x72:  // <<
        button = ui->button_1;
        break;
    case 0x75:  // >
        button = ui->button_2;
        break;
    case 0x73:  // >>
        button = ui->button_3;
        break;
    case 0x71:  // Repeat
        button = ui->button_4;
        break;
    case 0x74:  // Stop
        button = ui->button_5;
        break;
    case 0x76:  // Record
        button = ui->button_6;
        break;
    case 0x6e:  // 1
        button = ui->button_7;
        break;
    case 0x31:
        button = ui->button_8;
        break;
    case 0x6f:  // 2
        button = ui->button_9;
        break;
    case 0x32:
        button = ui->button_10;
        break;
    case 0x0e:  // 3
        button = ui->button_11;
        break;
    case 0x33:
        button = ui->button_12;
        break;
    case 0x0f:    // 4
        button = ui->button_13;
        break;
    case 0x34:
        button = ui->button_14;
        break;
#if 0
    case 0x:    // 5 (BROKEN)
        button = ui->button_15;
        break;
#endif
    case 0x35:
        button = ui->button_16;
        break;
#if 0
    case 0x:    // 6 (BROKEN)
        button = ui->button_17;
        break;
#endif
    case 0x36:
        button = ui->button_18;
        break;
    case 0x0c:    // 7
        button = ui->button_19;
        break;
    case 0x37:
        button = ui->button_20;
        break;
    case 0x0d:    // 8
        button = ui->button_21;
        break;
    case 0x38:
        button = ui->button_22;
        break;
    case 0x6d:    // 9
        button = ui->button_23;
        break;
    case 0x39:
        button = ui->button_24;
        break;
    default:
        return;
    }

    button->setEnabled(clicked);
}

void MainWindow::onNewMidiEvent()
{
    // Called when new data can be read on the MIDI device
    unsigned char buffer[3];
    if(::read(dev_fd, buffer, 3) != 3) {
        return;
    }
    if (buffer[0] != 0xb7) {
        return;
    }
    if (buffer[1] > 0x20 && buffer[1] < 0x2a) {
        setSlider(buffer[1] - 0x20, (float)buffer[2] / 127.);
    } else if (buffer[1] > 0x10 && buffer[1] < 0x1a) {
        setPotar(buffer[1] - 0x10, (float)buffer[2] / 127.);
    } else {
        setButton(buffer[1], (bool)buffer[2]);
    }
}
