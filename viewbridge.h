#pragma once

#include <QMouseEvent>
#include <QObject>
#include <QPixmap>
class CenterControl;
class ImageProvider;
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
    Q_INVOKABLE void updatePixmap(QPixmap pixmap);

    ImageProvider *m_imageprovider;
    CenterControl *_cctrl;

    void closeEvent(); //client关闭

    void mouseMoveEvent(QMouseEvent *event);    //鼠标移动
    void mouseReleaseEvent(QMouseEvent *event); //鼠标点击

signals:
    void needUpdate();
    void acceptInfo(bool);  //true accept连接成功 false accept连接失败
    void connectOver();     //连接被对方关闭
    void connectInfo(bool); //link连接失败 或 失败

public slots:
    bool handlerValidShare();                                             //处理共享
    bool handleValidLink(QString textIP, QString textPort);               //
    void handleUnShare();
};
