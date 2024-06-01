#include "pevent.h"
#include <QCursor>
#include <QEvent>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QString>
#include <iostream>
PositionNode::PositionNode(int x, int y, double px, double py, int type)
    : m_x(x)
    , m_y(y)
    , m_px(px)
    , m_py(py)
    , m_type(type)
{}

QJsonObject PositionNode::toJson() const
{
    QJsonObject obj;
    obj["x"] = m_x;
    obj["y"] = m_y;
    obj["px"] = m_px;
    obj["py"] = m_py;
    obj["type"] = m_type;
    return obj;
}

PositionNode PositionNode::fromJson(const QJsonObject &obj)
{
    int m_x = obj["x"].toInt();
    int m_y = obj["y"].toInt();
    double m_px = obj["px"].toDouble();
    double m_py = obj["py"].toDouble();
    int m_type = obj["type"].toInt();
    return PositionNode(m_x, m_y, m_px, m_py, m_type);
}

PEvent::PEvent(QObject *parent)
    : QObject{parent}
{
    //获得全局缩放
    m_globalScaleRatio = QGuiApplication::primaryScreen()->devicePixelRatio();
    //获取屏幕大小
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    m_screenWidth = screenGeometry.width();
    m_screenHeight = screenGeometry.height();
}

bool PEvent::toDo(PositionNode &pNode)
{
    //std::cout << "x : " << m_screenWidth << " y : " << m_screenHeight << std::endl;

    if (pNode.m_type == PositionNode::Type::mousePress) {
        std::cout << "service to click" << std::endl;
        QString xdotoolPath = "/usr/bin/xdotool";
        int x = m_screenWidth * pNode.m_px * m_globalScaleRatio;
        int y = m_screenHeight * pNode.m_py * m_globalScaleRatio;
        //移动
        m_process.start(xdotoolPath,
                        QStringList() << "mousemove" << QString::number(x) << QString::number(y));
        m_process.waitForFinished(); // 等待命令完成
        m_process.start(xdotoolPath,
                        QStringList() << "click"
                                      << "3");
        m_process.waitForFinished(); // 等待命令完成
    }
    return true;
}
