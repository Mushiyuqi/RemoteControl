#pragma once
#include <QJsonObject>
#include <QObject>
#include <QPoint>
#include <QProcess>

class PositionNode
{
public:
    explicit PositionNode(int x = 0, int y = 0, double px = 0, double py = 0, int type = 0);

    //以view窗口为基准的坐标
    int m_x;
    int m_y;
    //百分比坐标
    double m_px;
    double m_py;
    //事件类型
    enum Type {
        nullEvent = 0,
        mouseLeftRelease = 1,
        mouseRightRelease = 2,
        mouseMove = 3,
        mouseDouble = 4,
        keyPress = 5,
        keyRelease = 6
    };
    int m_type;

    //转换为JSON格式
    QJsonObject toJson() const;
    // 从QJsonObject构造实例
    static PositionNode fromJson(const QJsonObject &obj);
};

//用于处理各种事件
class PEvent : public QObject
{
    Q_OBJECT
public:
    explicit PEvent(QObject *parent = nullptr);
    bool toDo(PositionNode &pNode);
    qreal m_globalScaleRatio; //全局缩放率
    int m_screenWidth;
    int m_screenHeight;
    QProcess m_process;
};
