#include "dialog.h"
#include "ui_dialog.h"
#include <QFile>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    vid = new VideoDevice("/dev/video0");
    if((-1 == vid->open_device()) || (-1 == vid->init_device()))
    {
            perror("open device");
    }

    udp = new Udp();

    if(-1 == vid->start_capturing())
    {
        exit(EXIT_FAILURE);
    }

    forever
    {
        sendData();
//        sleep(1);
    }
    if(-1 == vid->stop_capturing())
    {
        exit(EXIT_FAILURE);
    }
}

Dialog::~Dialog()
{
    delete udp;
    delete ui;
}

void Dialog::sendData()
{


    char* frame;
    size_t frame_len;


    if(-1 == vid->get_frame((void**)&frame, &frame_len))
    {
        exit(EXIT_FAILURE);
    }

    qDebug("addr: %u, len: %d",(unsigned int)frame, frame_len);
    //        qDebug(frame);

    udp->sendData((char *)frame, frame_len);


//    QFile file("/home/alex/a.jpg");
//    file.open(QFile::WriteOnly);
//    file.write(frame, frame_len);
//    file.close();


    if(-1 == vid->unget_frame())
    {
        exit(EXIT_FAILURE);
    }


}

