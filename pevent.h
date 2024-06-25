#pragma once
#include <QJsonObject>
#include <QObject>
#include <QPoint>
#include <QProcess>
#include <QKeyEvent>
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
        mouseLeftClick = 1,
        mouseRightClick = 2,
        mouseScrollUp = 3,
        mouseScrollDown = 4,
        keyTyped = 5,
        shortcutOverride = 6,
        mouseMove = 7,
        mouseDouble = 8,
        keyPress = 9,
        keyRelease = 10
    };
    int m_type;

    //转换为JSON格式
    QJsonObject toJson() const;
    // 从QJsonObject构造实例
    static PositionNode fromJson(const QJsonObject &obj);
};


class KeyNode {
public:
    explicit KeyNode(int type = 0);

    enum Type {
        nullKey = 0,
        keyValue_A = 1
    };
    int m_type;

    //转换为JSON格式
    QJsonObject toJson() const;
    // 从QJsonObject构造实例
    static KeyNode fromJson(const QJsonObject &obj);
};


//用于处理各种事件
class PEvent : public QObject
{
    Q_OBJECT
public:
    explicit PEvent(QObject *parent = nullptr);
    bool mouseToDo(PositionNode &pNode);
    bool KeyTodo(KeyNode &kNode);
    qreal m_globalScaleRatio; //全局缩放率
    int m_screenWidth;
    int m_screenHeight;
    QProcess m_process;
};
