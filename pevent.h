#pragma once
#include <QJsonObject>
#include <QObject>
#include <QPoint>
#include <QProcess>
#include <QKeyEvent>

class EventNode {
public:
    explicit EventNode(int x = 0, int y = 0, double px = 0, double py = 0, int type = 0, int keyType = 0);

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

    //键盘类型
    enum KeyType {
        nullKey = 0,
        key_A = 'a',
        key_B = 'b',
        key_C = 'c',
        key_D = 'd',
        key_E = 'e',
        key_F = 'f',
        key_G = 'g',
        key_H = 'h',
        key_I = 'i',
        key_J = 'j',
        key_K = 'k',
        key_L = 'l',
        key_M = 'm',
        key_N = 'n',
        key_O = 'o',
        key_P = 'p',
        key_Q = 'q',
        key_R = 'r',
        key_S = 's',
        key_T = 't',
        key_U = 'u',
        key_V = 'v',
        key_W = 'w',
        key_X = 'x',
        key_Y = 'y',
        key_Z = 'z',

    };

    int m_type;
    int m_keyType;
    //转换为JSON格式
    QJsonObject toJson() const;
    // 从QJsonObject构造实例
    static EventNode fromJson(const QJsonObject &obj);
};


//用于处理各种事件
class PEvent : public QObject
{
    Q_OBJECT
public:
    explicit PEvent(QObject *parent = nullptr);
    bool mouseToDo(EventNode &eNode);
    bool KeyTodo(EventNode &eNode);
    bool eventToDo(EventNode &eNode);
    qreal m_globalScaleRatio; //全局缩放率
    int m_screenWidth;
    int m_screenHeight;
    QProcess m_process;
};
