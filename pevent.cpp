#include "pevent.h"
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
{}

bool PEvent::toDo(PositionNode &pNode)
{
    std::cout << "pNode" << std::endl
              << "x : " << pNode.m_x << "  y : " << pNode.m_y << std::endl
              << "px : " << pNode.m_px << "  Wpy : " << pNode.m_py << std::endl
              << "type : " << pNode.m_type << std::endl;
    return true;
}
