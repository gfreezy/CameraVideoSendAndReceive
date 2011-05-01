#include "udp.h"
#include <QDateTime>
extern "C"
{
#include "utils.h"
#include "color.h"
}

Udp::Udp(QObject *parent) :
    QThread(parent)
{

    port = 4567;
//    addr = QHostAddress("192.168.1.122");
    addr = QHostAddress::LocalHost;
    data_packet = new DataPacket();

    socket = new QUdpSocket();
    initLut();
}

void Udp::sendData(char *frame, quint32 frame_len)
{
    quint32 frag_size = PAYLOAD_SIZE / 6 * 4;
    quint32 total_num = frame_len / frag_size + ((frame_len % frag_size) ? 0 : 1);
    quint32 left_size = 0;
    quint64 id = QDateTime::currentDateTime().toMSecsSinceEpoch();
    for(quint32 i = 0; i < total_num; ++i)
    {
        memset(&data_packet->header, 0, sizeof(DataPacketHeader));
        data_packet->header.id = id;
        data_packet->header.total_num = total_num;
        data_packet->header.num = i;
        if((left_size = frame_len - frag_size * i) >= frag_size)
        {
            data_packet->header.len = PAYLOAD_SIZE;
            Pyuv422torgb24((unsigned char*)(frame + i * frag_size), data_packet->payload, frag_size/2, 1);
        }
        else
        {
            data_packet->header.len = left_size / 4 * 6;
            Pyuv422torgb24((unsigned char *)(frame + i * frag_size), data_packet->payload, left_size /2, 1);
        }
        if(-1 == socket->writeDatagram((char *)data_packet, sizeof(DataPacket), addr, port))
            qDebug("send error");
//        qDebug((char *)data_packet->payload);
        qDebug("send data packet %d, len %d, id %u", i, sizeof(DataPacket), id);
        msleep(15);
    }
}

Udp::~Udp()
{
    freeLut();
    delete data_packet;
    delete socket;
}

void Udp::run()
{

}
