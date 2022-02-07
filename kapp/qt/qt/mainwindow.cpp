

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

struct lightnow {
    char l1;
    char l2;
    char l3;
} lght;

int fd;
void send_lght(){
    if (fd <= 0) {
        fd = open("/dev/kbl", O_RDWR); // chmod 777 /dev/kbl
        if (fd < 0) {
            printf("open errored=%d!\n", fd);
            goto errored;
        }
    }
    int error;
    if ((error = ioctl(fd, 123, &lght)) < 0) {
        printf("ioctl errored=%d!\n", error);
        goto errored;
    } else {
        printf("ioctl ok!\n");
    }
    return;
errored:
    if (fd >= 0)
       close(fd);
}
void stateChanged(int led, int state) {
    if (state == Qt::CheckState::PartiallyChecked)
        return;
    if (led == 0)
        lght.l1 = state == Qt::CheckState::Checked;
    if (led == 1)
        lght.l2 = state == Qt::CheckState::Checked;
    if (led == 2)
        lght.l3 = state == Qt::CheckState::Checked;
    send_lght();
}

void MainWindow::on_l1_stateChanged(int arg1) {
    stateChanged(0, arg1);
}

void MainWindow::on_l2_stateChanged(int arg1) {
    stateChanged(1, arg1);
}

void MainWindow::on_l3_stateChanged(int arg1) {
    stateChanged(2, arg1);
}
