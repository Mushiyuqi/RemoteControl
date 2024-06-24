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

bool PEvent::toDo(PositionNode &pNode)
{
    if (pNode.m_type == PositionNode::Type::mouseLeftRelease
        || pNode.m_type == PositionNode::Type::mouseRightRelease) {

        //移动 左键 右键
        if (pNode.m_type == PositionNode::Type::mouseLeftRelease) {
            m_process.start("xdotool",
                            QStringList() << "click"<< "1");
        } else if (pNode.m_type == PositionNode::Type::mouseRightRelease) {
            m_process.start("xdotool",
                            QStringList() << "click"<< "3");
        }
        m_process.waitForFinished(); // 等待命令完成
    } else if (pNode.m_type == PositionNode::Type::mouseMove) {
        //计算坐标
        int x = m_screenWidth * pNode.m_px * m_globalScaleRatio;
        int y = m_screenHeight * pNode.m_py * m_globalScaleRatio;
        m_process.start("xdotool", QStringList() << "mousemove" << QString::number(x) << QString::number(y) );
        m_process.waitForFinished(); // 等待命令完成
    }

    return true;
}
bool PEvent::KeyTodo(KeyNode &kNode) {
    if (kNode.m_type == KeyNode::Type::keyValue_A) {
        m_process.start("xdotool",
                        QStringList() << "key"
                                      << "a");
        m_process.waitForFinished();
        qDebug() << "执行了a键";
    }
    return true;
}
