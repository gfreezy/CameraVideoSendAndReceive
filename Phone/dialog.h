#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "udp.h"
#include "videodevice.h"

namespace Ui {
    class Dialog;
}
class QUdpSocket;
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void sendData();

private:
    Ui::Dialog *ui;
    Udp *udp;
    VideoDevice *vid;
};

#endif // DIALOG_H
