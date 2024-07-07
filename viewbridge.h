#pragma once

#include <QMouseEvent>
#include <QObject>
#include <QPixmap>
#include "pevent.h"
class CenterControl;
class ImageProvider;
class ViewControl;
class ViewBridge : public QObject
{
    Q_OBJECT
public:
    explicit ViewBridge(QObject *parent = nullptr);
    ~ViewBridge();

    ImageProvider *getImageProvider();
    Q_INVOKABLE int getImgWidth();
    Q_INVOKABLE int getImgHeight();

    //更新屏幕数据
    void updatePixmap(QPixmap pixmap);

    ImageProvider *m_imageprovider; //不需要释放
    CenterControl *_cctrl;
    ViewControl *_vctrl = nullptr;

    void setViewControl(ViewControl *vctrl);

signals:
    void needUpdate();
    void acceptInfo(bool);  //true accept连接成功 false accept连接失败

    void connectSeverOver();  //连接被对方关闭
    void connectClientOver(); //连接被对方关闭

public slots:
    void handlerShare();                               //处理共享
    bool handleLink(QString textIP, QString textPort); //
    void handleCloseShare();
    void handleClientClose();

    //鼠标事件
    void mouseTappedEvent(
        const int x, const int y, const int width, const int height, const int type);
    void mouseMoveEvent(const int x, const int y, const int width, const int height);
    void mouseDoubleTappedEvent(const int x, const int y, const int width, const int height);
    void mouseWheelEvent(const int x, const int y, const int width, const int height, int roolLength);
    void keyPressEvent(int keyType);
    void keyReleaseEvent(int keyType);

private:
    bool getKetType(EventNode &eNode, int keyType);
};
