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

PositionNode::PositionNode(int x, int y, double px, double py, int type)
    : m_x(x)
    , m_y(y)
    , m_px(px)
    , m_py(py)
    , m_type(type)
{}
KeyNode::KeyNode(int type)
    : m_type(type)
{}

QJsonObject KeyNode::toJson() const {
    QJsonObject obj;
    obj["type"] = m_type;
    return obj;
}

KeyNode KeyNode::fromJson(const QJsonObject &obj) {
    int m_type = obj["type"].toInt();
    return KeyNode(m_type);
}


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
                                          << "5"
                                          << "--repeat"
                                          << "1");
        } else {
            m_process.start("xdotool",
                            QStringList() << "click"
                                          << "4"
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



    //键盘
    else if (eNode.m_keyType == EventNode::KeyType::key_A) {

        if (eNode.m_type == EventNode::Type::keyPress) {
            qDebug() << "按下a键";
            m_process.start("xdotool", QStringList() << "keydown" << "a");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            qDebug() << "松开a键";
            m_process.start("xdotool", QStringList() << "keyup" << "a");
        }

        // m_process.start("xdotool",
        //                 QStringList() << "key"
        //                               << "a");
        m_process.waitForFinished();
    } else if (eNode.m_keyType == EventNode::KeyType::key_B) {

        if (eNode.m_type == EventNode::Type::keyPress) {
            qDebug() << "按下b键";
            m_process.start("xdotool", QStringList() << "keydown" << "b");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            qDebug() << "松开b键";
            m_process.start("xdotool", QStringList() << "keyup" << "b");
        }

        // m_process.start("xdotool",
        //                 QStringList() << "key"
        //                               << "b");
        m_process.waitForFinished();
    }
    return true;
}

//鼠标的
bool PEvent::mouseToDo(EventNode &eNode)
{
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
    if (eNode.m_type == EventNode::Type::mouseScrollUp
        || eNode.m_type == EventNode::Type::mouseScrollDown) {
        if (eNode.m_type == EventNode::Type::mouseScrollUp) {
            m_process.start("xdotool",
                            QStringList() << "click"
                                          << "5"
                                          << "--repeat"
                                          << "1");
        } else {
            m_process.start("xdotool",
                            QStringList() << "click"
                                          << "4"
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

    return true;
}

//键盘的
bool PEvent::KeyTodo(EventNode &eNode)
{
    if (eNode.m_keyType == EventNode::KeyType::key_A) {

        if (eNode.m_type == EventNode::Type::keyPress) {
            qDebug() << "按下a键";
            m_process.start("xdotool", QStringList() << "keydown" << "a");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            qDebug() << "松开a键";
            m_process.start("xdotool", QStringList() << "keyup" << "a");
        }

       // m_process.start("xdotool",
       //                 QStringList() << "key"
       //                               << "a");
        m_process.waitForFinished();
    } else if (eNode.m_keyType == EventNode::KeyType::key_B) {

        if (eNode.m_type == EventNode::Type::keyPress) {
            qDebug() << "按下b键";
            m_process.start("xdotool", QStringList() << "keydown" << "b");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            qDebug() << "松开b键";
            m_process.start("xdotool", QStringList() << "keyup" << "b");
        }

        // m_process.start("xdotool",
        //                 QStringList() << "key"
        //                               << "b");
        m_process.waitForFinished();
    }
    return true;
}
