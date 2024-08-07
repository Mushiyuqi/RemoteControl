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
    //键盘的单独处理
    if (eNode.m_type == EventNode::Type::keyPress || eNode.m_type == EventNode::Type::keyRelease) {
        bool flag = keyBoardToDo(eNode);
        return flag;
    }
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

    return true;
}

bool PEvent::keyBoardToDo(EventNode &eNode)
{
    if (eNode.m_keyType >= 'a' && eNode.m_keyType <= 'z') {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << QString(QChar(static_cast<uchar>(eNode.m_keyType))));
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList()
                                << "keyup" << QString(QChar(static_cast<uchar>(eNode.m_keyType))));
        }
        m_process.waitForFinished();
        return true;
    }

    switch (eNode.m_keyType) {
        //`
    case EventNode::KeyType::key_QuoteLeft: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "quoteleft");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "quoteleft");
        }
        m_process.waitForFinished();
        return true;
    }
    //0
    case EventNode::KeyType::key_0: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "0");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "0");
        }
        m_process.waitForFinished();
        return true;
    }
    //1
    case EventNode::KeyType::key_1: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "1");
            qDebug() << "按下了1键";
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "1");
            qDebug() << "松开了1键";
        }
        m_process.waitForFinished();
        return true;
    }
    //2
    case EventNode::KeyType::key_2: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "2");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "2");
        }
        m_process.waitForFinished();
        return true;
    }
    //3
    case EventNode::KeyType::key_3: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "3");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "3");
        }
        m_process.waitForFinished();
        return true;
    }
    //4
    case EventNode::KeyType::key_4: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "4");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "4");
        }
        m_process.waitForFinished();
        return true;
    }
    //5
    case EventNode::KeyType::key_5: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "5");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "5");
        }
        m_process.waitForFinished();
        return true;
    }
    //6
    case EventNode::KeyType::key_6: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "6");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "6");
        }
        m_process.waitForFinished();
        return true;
    }
    //7
    case EventNode::KeyType::key_7: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "7");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "7");
        }
        m_process.waitForFinished();
        return true;
    }
    //8
    case EventNode::KeyType::key_8: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "8");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "8");
        }
        m_process.waitForFinished();
        return true;
    }
    //9
    case EventNode::KeyType::key_9: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "9");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "9");
        }
        m_process.waitForFinished();
        return true;
    }
    //-
    case EventNode::KeyType::key_Minus: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "minus");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "minus");
        }
        m_process.waitForFinished();
        return true;
    }
    //=
    case EventNode::KeyType::key_Equal: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "equal");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "equal");
        }
        m_process.waitForFinished();
        return true;
    }
    //Backspace
    case EventNode::KeyType::key_Backspace: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "BackSpace");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "BackSpace");
        }
        m_process.waitForFinished();
        return true;
    }
    //bracketleft
    case EventNode::KeyType::key_BracketLeft: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "bracketleft");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "bracketleft");
        }
        m_process.waitForFinished();
        return true;
    }
    //bracketright

    case EventNode::KeyType::key_BracketRight: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "bracketright");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "bracketright");
        }
        m_process.waitForFinished();
        return true;
    }
    //  反斜杠
    case EventNode::KeyType::key_Backslash: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "backslash");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "backslash");
        }
        m_process.waitForFinished();
        return true;
    }

    //Return
    case EventNode::KeyType::key_Return: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Enter");
            qDebug() << "按下Enter键";
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Enter");
            qDebug() << "松开Enter键";
        }
        m_process.waitForFinished();
        return true;
    }
    //Tab
    case EventNode::KeyType::key_Tab: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Tab");
            qDebug() << "按下tab";
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Tab");
            qDebug() << "松开tab";
        }
        m_process.waitForFinished();
        return true;
    }
    //Caps_Lock
    case EventNode::KeyType::key_CapsLock: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Caps_Lock");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Caps_Lock");
        }
        m_process.waitForFinished();
        return true;
    }
    //Shift_L
    case EventNode::KeyType::key_Shift: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Shift_L");
            qDebug() << "按下了shift键";
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Shift_L");
            qDebug() << "松开了shift键";
        }
        m_process.waitForFinished();
        return true;
    }
    //Control_L
    case EventNode::KeyType::key_Ctrl: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Control_L");
            qDebug() << "按下control";
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Control_L");
            qDebug() << "松开control";
        }
        m_process.waitForFinished();
        return true;
    }
    //Win
    case EventNode::KeyType::key_Meta: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Win");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Win");
        }
        m_process.waitForFinished();
        return true;
    }
    //Alt_L
    case EventNode::KeyType::key_Alt: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Alt_L");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Alt_L");
        }
        m_process.waitForFinished();
        return true;
    }
    //space
    case EventNode::KeyType::key_Space: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "space");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "space");
        }
        m_process.waitForFinished();
        return true;
    }
    //semicolon
    case EventNode::KeyType::key_Semicolon: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "semicolon");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "semicolon");
        }
        m_process.waitForFinished();
        return true;
    }
    //'
    case EventNode::KeyType::key_Apostrophe: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "apostrophe");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "apostrophe");
        }
        m_process.waitForFinished();
        return true;
    }
    //comma
    case EventNode::KeyType::key_Comma: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "comma");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "comma");
        }
        m_process.waitForFinished();
        return true;
    }
    //period
    case EventNode::KeyType::key_Period: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "period");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "period");
        }
        m_process.waitForFinished();
        return true;
    }
    // /
    case EventNode::KeyType::key_Slash: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "slash");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "slash");
        }
        m_process.waitForFinished();
        return true;
    }
    //Escape
    case EventNode::KeyType::key_Esc: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Escape");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Escape");
        }
        m_process.waitForFinished();
        return true;
    }
    //Scroll_Lock
    case EventNode::KeyType::key_ScrollLock: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Scroll_Lock");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Scroll_Lock");
        }
        m_process.waitForFinished();
        return true;
    }
    //Print
    case EventNode::KeyType::key_Print: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Print");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Print");
        }
        m_process.waitForFinished();
        return true;
    }
    //Pause_Break
    case EventNode::KeyType::key_PauseBreak: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Pause_Break");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Pause_Break");
        }
        m_process.waitForFinished();
        return true;
    }
    //Insert
    case EventNode::KeyType::key_Insert: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Insert");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Insert");
        }
        m_process.waitForFinished();
        return true;
    }
    //Home
    case EventNode::KeyType::key_Home: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Home");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Home");
        }
        m_process.waitForFinished();
        return true;
    }
    //Page_Up
    case EventNode::KeyType::key_PageUp: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Page_Up");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Page_Up");
        }
        m_process.waitForFinished();
        return true;
    }
    //Page_Down
    case EventNode::KeyType::key_PageDown: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Page_Down");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Page_Down");
        }
        m_process.waitForFinished();
        return true;
    }
    //Delete
    case EventNode::KeyType::key_Delete: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Delete");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Delete");
        }
        m_process.waitForFinished();
        return true;
    }
    //End
    case EventNode::KeyType::key_End: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "End");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "End");
        }
        m_process.waitForFinished();
        return true;
    }
    //Up
    case EventNode::KeyType::key_Up: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Up");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Up");
        }
        m_process.waitForFinished();
        return true;
    }
    //Down
    case EventNode::KeyType::key_Down: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Down");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Down");
        }
        m_process.waitForFinished();
        return true;
    }
    //Left
    case EventNode::KeyType::key_Left: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Left");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Left");
        }
        m_process.waitForFinished();
        return true;
    }
    //Right
    case EventNode::KeyType::key_Right: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "Right");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "Right");
        }
        m_process.waitForFinished();
        return true;
    }

    case EventNode::KeyType::key_Exclam: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "exclam");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "exclam");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_At: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "at");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "at");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_NumberSign: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "numbersign");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "numbersign");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Dollar: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "dollar");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "dollar");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Percent: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "percent");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "percent");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_AsciiCircum: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "asciicircum");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "asciicircum");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Ampersand: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "ampersand");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "ampersand");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Asterisk: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "asterisk");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "asterisk");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_ParenLeft: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "parenleft");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "parenleft");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_ParenRight: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "parenright");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "parenright");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Underscore: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "underscore");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "underscore");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Plus: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "plus");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "plus");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_BraceLeft: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "braceleft");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "braceleft");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_BraceRight: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "braceright");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "braceright");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Bar: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "bar");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "bar");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Colon: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "colon");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "colon");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_QuoteDbl: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "quotedbl");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "quotedbl");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Less: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "less");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "less");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Greater: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "greater");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "greater");
        }
        m_process.waitForFinished();
        return true;
    }
    case EventNode::KeyType::key_Question: {
        if (eNode.m_type == EventNode::Type::keyPress) {
            m_process.start("xdotool",
                            QStringList() << "keydown"
                                          << "question");
        } else if (eNode.m_type == EventNode::Type::keyRelease) {
            m_process.start("xdotool",
                            QStringList() << "keyup"
                                          << "question");
        }
        m_process.waitForFinished();
        return true;
    }

    default:
        break;
    }
    return false;
}
