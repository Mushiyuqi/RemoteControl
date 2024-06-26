#include "pevent.h"
#include <QCursor>
#include <QEvent>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QString>
#include <iostream>

EventNode::EventNode(int x, int y, double px, double py, int type,int keyType)
    : m_x(x)
    , m_y(y)
    , m_px(px)
    , m_py(py)
    , m_type(type)
    , m_keyType(keyType)
{}

QJsonObject EventNode::toJson() const
{
    QJsonObject obj;
    obj["x"] = m_x;
    obj["y"] = m_y;
    obj["px"] = m_px;
    obj["py"] = m_py;
    obj["type"] = m_type;
    obj["keyType"] = m_keyType;
    return obj;
}

EventNode EventNode::fromJson(const QJsonObject &obj)
{
    int m_x = obj["x"].toInt();
    int m_y = obj["y"].toInt();
    double m_px = obj["px"].toDouble();
    double m_py = obj["py"].toDouble();
    int m_type = obj["type"].toInt();
    int m_keyType = obj["keyType"].toInt();
    return EventNode(m_x, m_y, m_px, m_py, m_type, m_keyType);
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

bool PEvent::eventToDo(EventNode &eNode) {
    //点击
    if (eNode.m_type == EventNode::Type::mouseLeftClick
        || eNode.m_type == EventNode::Type::mouseRightClick) {
        if (eNode.m_type == EventNode::Type::mouseLeftClick) {
            m_process.start("xdotool",
                            QStringList() << "click"
                                          << "1");
        } else {
            m_process.start("xdotool",
                            QStringList() << "click"
                                          << "3");
        }
        m_process.waitForFinished();
    }

    //滚动
    else if (eNode.m_type == EventNode::Type::mouseScrollUp
        || eNode.m_type == EventNode::Type::mouseScrollDown) {
        if (eNode.m_type == EventNode::Type::mouseScrollUp) {
            m_process.start("xdotool",
                            QStringList() << "click"
                                          << "4"
                                          << "--repeat"
                                          << "1");
        } else {
            m_process.start("xdotool",
                            QStringList() << "click"
                                          << "5"
                                          << "--repeat"
                                          << "1");
        }
        m_process.waitForFinished();
    }

    //移动
    else if (eNode.m_type == EventNode::Type::mouseMove) {
        //计算坐标
        int x = m_screenWidth * eNode.m_px * m_globalScaleRatio;
        int y = m_screenHeight * eNode.m_py * m_globalScaleRatio;
        m_process.start("xdotool",QStringList() << "mousemove" << QString::number(x) << QString::number(y));
        m_process.waitForFinished();
    }

    //双击(xdotool只能用两次单击实现)
    else if (eNode.m_type == EventNode::Type::mouseDouble) {
        m_process.start("xdotool",
                        QStringList() << "click"
                                      << "1");
        m_process.waitForFinished();
        m_process.start("xdotool",
                        QStringList() << "click"
                                      << "1");
        m_process.waitForFinished();
    }

    else if (eNode.m_keyType >= 'a' && eNode.m_keyType <= 'z') {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << QString(QChar(static_cast<uchar>(eNode.m_keyType))));
            qDebug() << "按下键盘";
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList()
                                << "keyup" << QString(QChar(static_cast<uchar>(eNode.m_keyType))));
            qDebug() << "松开键盘";
        }
        m_process.waitForFinished();
    } else if (eNode.m_keyType == EventNode::KeyType::key_Ctrl) {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Control_L");
            qDebug() << "按下键盘";
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Control_L");
            qDebug() << "松开键盘";
        }
        m_process.waitForFinished();
    }
    return true;
}
