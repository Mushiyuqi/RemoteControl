#pragma once
#include <QJsonObject>
#include <QObject>
#include <QPoint>
#include <QProcess>
#include <QKeyEvent>

class EventNode {
public:
    explicit EventNode(
        int x = 0, int y = 0, double px = 0, double py = 0, int type = 0, int keyType = 0);

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

        key_QuoteLeft,
        key_0,
        key_1,
        key_2,
        key_3,
        key_4,
        key_5,
        key_6,
        key_7,
        key_8,
        key_9,
        key_Minus,
        key_Equal,
        key_Backspace,

        /*     [ 键对应于 Qt::Key_BracketLeft
               ] 键对应于 Qt::Key_BracketRight
               \ 键对应于 Qt::Key_Backslash*/
        key_BracketLeft,
        key_BracketRgiht,
        key_Backslash,
        key_Enter,
        key_Tab,
        key_CapsLock,
        key_Shift,
        key_Ctrl,
        key_Meta,
        key_Alt,
        key_Space,
        /*     
    ; 键对应于 Qt::Key_Semicolon
    ' 键对应于 Qt::Key_Apostrophe
    , 键对应于 Qt::Key_Comma
    . 键对应于 Qt::Key_Period
    / 键对应于 Qt::Key_Slash*/
        key_Semicolon,
        key_Apostrophe,
        key_Comma,
        key_Period,
        key_Slash,
        key_Esc,
        //Scroll Lock (   Qt::Key_ScrollLock
        //Print (  Qt::Key_Print
        //Pause Break    (  Qt::Key_Pause
        //Insert  (  Qt::Key_Insert
        //Home  (  Qt::Key_Home
        //PageUp  (  Qt::Key_PageUp
        //PageDown  ( Qt::key_PageDown
        //Delete  (  Qt::key_Delete
        //End  (  Qt::Key_End
        //up down left right
        key_F1,
        key_F2,
        key_F3,
        key_F4,
        key_F5,
        key_F6,
        key_F7,
        key_F8,
        key_F9,
        key_F10,
        key_F11,
        key_F12,
        key_Print,
        key_ScrollLock,
        key_PauseBreak,
        key_Insert,
        key_Home,
        key_PageUp,
        key_PageDown,
        key_Delete,
        key_End,
        key_Up,
        key_Down,
        key_Left,
        key_Right,
        //         /*
        // 数字键盘加号 (+): Qt::Key_NumpadPlus
        // 数字键盘减号 (-): Qt::Key_NumpadMinus
        // 数字键盘乘号 (*): Qt::Key_NumpadMul
        // 数字键盘除号 (/): Qt::Key_NumpadDiv
        // 数字键盘点号 (.) 或小数点: Qt::Key_NumpadDecimal
        // 数字键盘Enter键: Qt::Key_NumpadEnter
        // */
        //         key_NumLock,
        //         key_NumPadDiv,
        //         key_NumpadMul,
        //         key_NumpadMinus,
        //         key_NumpadPlus,
        //         key_NumPadDecimal,
        //         key_NumpadEnter,
        //         key_num_0,
        //         key_num_1,
        //         key_num_2,
        //         key_num_3,
        //         key_num_4,
        //         key_num_5,
        //         key_num_6,
        //         key_num_7,
        //         key_num_8,
        //         key_num_9

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
    bool keyBoardToDo(EventNode &eNode);
    bool eventToDo(EventNode &eNode);
    qreal m_globalScaleRatio; //全局缩放率
    int m_screenWidth;
    int m_screenHeight;
    QProcess m_process;
};
