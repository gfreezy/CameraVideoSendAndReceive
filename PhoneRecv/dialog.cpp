#include "dialog.h"
#include "ui_dialog.h"
#include <QImage>
#include <QPixmap>
extern "C"
{
#include <assert.h>
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    s = new QUdpSocket();
    s->bind(4567);

    connect(s, SIGNAL(readyRead()), this, SLOT(ready_read()));
    current_id = 0;
    data = new char[PACKAGE_SIZE + sizeof(DataPacketHeader)]; //space for the last header in spite of override
    sign = new char[PACKET_NUM];
    header = new DataPacketHeader();
    img = new QImage((unsigned char *)data, 320, 240, QImage::Format_RGB888);
}

Dialog::~Dialog()
{
    delete s;
    delete ui;
}


void Dialog::ready_read()
{

    if(s->hasPendingDatagrams())
    {
        quint64 len = s->pendingDatagramSize();
        char *read_addr = data;
        quint16 i;
        for(i = 0; i < PACKET_NUM; ++i)
        {
            if(sign[i] == 1)
            {
                continue;
            }
            read_addr += i * PAYLOAD_SIZE;
            break;
        }
//        qDebug("read_addr %u", (read_addr - data)/PAYLOAD_SIZE);

        if(-1 == s->readDatagram(read_addr, len, &addr, &port)) //read packet header information
        {
            qDebug("receive data error");
            return;
        }
//        qDebug("read data");
        packet = (DataPacket *)read_addr;

        qDebug("Receive data from %s:%d", addr.toString().toStdString().c_str(), port);
        qDebug("Total number: %d, number: %d, id %lu", packet->header.total_num, packet->header.num,
               packet->header.id);
//        qDebug("i: %d", i);
        process(packet);
//        qDebug("process");
        if(i + 1 >= PACKET_NUM)
        {
            qDebug("all data received");
            updatePicture();
            memset(sign, 0, PACKET_NUM);
        }
    }
}

void Dialog::process(DataPacket *packet)
{
    if(current_id < packet->header.id)
    {
        memset(sign, 0, PACKET_NUM);
        current_id = packet->header.id;
//        qDebug("new package");
    }

    memcpy(header, &packet->header, sizeof(DataPacketHeader));
    if( (((char *)packet - data) / PAYLOAD_SIZE != (signed)packet->header.num) &&
            (sign[packet->header.num] == 0))
    {
        char *dest = data + PAYLOAD_SIZE * packet->header.num;
        qDebug("dest %u, src %u, num %u", (int)(dest - data)/PAYLOAD_SIZE, (int)((char *)packet - data)/PAYLOAD_SIZE, packet->header.num);
        memcpy(dest, packet, PAYLOAD_SIZE);
//        qDebug("memcpy, num %d", header->num);
    }

//    qDebug("memset sign");
//    qDebug("%u", header->num);
    sign[header->num] = 1;
//    qDebug("in process function");
}

void Dialog::updatePicture()
{
    if(img->isNull())
        qDebug("img null");
    img->loadFromData((uchar *)data, PACKAGE_SIZE);
    ui->label->setPixmap(QPixmap::fromImage(*img));
//        qDebug("set img");
//    delete img;
//    qDebug(data);
}
