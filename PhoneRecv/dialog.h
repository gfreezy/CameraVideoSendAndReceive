#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QUdpSocket>
#include <QImage>

#define PAYLOAD_SIZE (30*1024)
#define PACKET_NUM 7
#define PACKAGE_SIZE (PAYLOAD_SIZE*PACKET_NUM)

namespace Ui {
    class Dialog;
}

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

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    void process(DataPacket *packet);
    void updatePicture();
    ~Dialog();

private:
    Ui::Dialog *ui;
    QUdpSocket *s;
    DataPacket *packet;
    DataPacketHeader *header;
    char *data;
    char *sign;
    QHostAddress addr;
    quint16 port;
    quint64 current_id;
    QImage *img;

public slots:
    void ready_read();
};

#endif // DIALOG_H
