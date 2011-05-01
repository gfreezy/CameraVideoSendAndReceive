#ifndef UDP_H
#define UDP_H

#include <QUdpSocket>
#include <QThread>

#define PAYLOAD_SIZE 30*1024

typedef struct _DataPacketHeader
{
    quint64 id; //the specific sign
    quint32 total_num;
    quint32 num;
    quint32 len;
} DataPacketHeader;

typedef struct _DataPacket
{
    quint8 payload[PAYLOAD_SIZE];
    struct _DataPacketHeader header;
} DataPacket;

class Udp : public QThread
{
    Q_OBJECT
public:
    explicit Udp(QObject *parent = 0);
    void sendData(char *buf, quint32 len);
    void run();
    ~Udp();
signals:

private:
    QHostAddress addr;
    quint16 port;
    DataPacket *data_packet;
    QUdpSocket *socket;

};

#endif // UDP_H
