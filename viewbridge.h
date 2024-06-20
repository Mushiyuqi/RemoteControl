#pragma once

#include <QMouseEvent>
#include <QObject>
#include <QPixmap>
class CenterControl;
class ImageProvider;
class ViewControl;
class ViewBridge : public QObject
{
    Q_OBJECT
public:
    explicit ViewBridge(QObject *parent = nullptr);
    ~ViewBridge();

    ImageProvider *getImageProvider() { return m_imageprovider; }
    Q_INVOKABLE int getImgWidth();
    Q_INVOKABLE int getImgHeight();

    //更新屏幕数据
    void updatePixmap(QPixmap pixmap);

    ImageProvider *m_imageprovider;
    CenterControl *_cctrl;
    ViewControl *_vctrl = nullptr;

    void setViewControl(ViewControl *vctrl);

    void mouseMoveEvent(QMouseEvent *event);    //鼠标移动
    void mouseReleaseEvent(QMouseEvent *event); //鼠标点击

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
};
