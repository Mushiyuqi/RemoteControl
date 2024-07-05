#include "view.h"
#include <QDebug>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QMouseEvent>
#include <QPixmap>
#include <QScreen>
#include "csession.h"
#include "viewcontrol.h"
View::View(QWidget *parent)
    : QLabel(parent)
{
    setScaledContents(true);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    setPixmap(pixmap.scaled(pixmap.size(), Qt::IgnoreAspectRatio));
}

void View::setControl(ViewControl *vctrl)
{
    _vctrl = vctrl;
}

void View::mousePressEvent(QMouseEvent *event)
{
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height(), 0, 0);
    if (event->button() == Qt::LeftButton)
        eNode.m_type = EventNode::Type::mouseLeftClick;
    else if (event->button() == Qt::RightButton)
        eNode.m_type = EventNode::Type::mouseRightClick;

    _vctrl->eventAction(eNode);
}

void View::wheelEvent(QWheelEvent *event)
{
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(0, 0, 0, 0, 0, 0);
    // 获取滚动的角度和方向
    int delta = event->angleDelta().y();

    // 判断滚动的方向
    if (delta > 0) {
        qDebug() << "Mouse wheel scrolled up";
        eNode.m_type = EventNode::Type::mouseScrollUp;
    } else {
        qDebug() << "Mouse wheel scrolled down";
        eNode.m_type = EventNode::Type::mouseScrollDown;
    }
    _vctrl->eventAction(eNode);
}

void View::mouseMoveEvent(QMouseEvent *event) {
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height(), 0, 0);
    eNode.m_type = EventNode::Type::mouseMove;

    _vctrl->eventAction(eNode);
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height(), 0, 0);
    eNode.m_type = EventNode::Type::mouseDouble;
    _vctrl->eventAction(eNode);
    qDebug() << "双击";
}
bool View::getKetType(EventNode &eNode, int keyType)
{
    bool flag = false;
    switch (keyType) {
    case Qt::Key_A:
        eNode.m_keyType = EventNode::KeyType::key_A;
        qDebug() << "a键";
        break;
    case Qt::Key_B:
        eNode.m_keyType = EventNode::KeyType::key_B;
        break;
    case Qt::Key_C:
        eNode.m_keyType = EventNode::KeyType::key_C;
        break;
    case Qt::Key_D:
        eNode.m_keyType = EventNode::KeyType::key_D;
        break;
    case Qt::Key_E:
        eNode.m_keyType = EventNode::KeyType::key_E;
        break;
    case Qt::Key_F:
        eNode.m_keyType = EventNode::KeyType::key_F;
        break;
    case Qt::Key_G:
        eNode.m_keyType = EventNode::KeyType::key_G;
        break;
    case Qt::Key_H:
        eNode.m_keyType = EventNode::KeyType::key_H;
        break;
    case Qt::Key_I:
        eNode.m_keyType = EventNode::KeyType::key_I;
        break;
    case Qt::Key_J:
        eNode.m_keyType = EventNode::KeyType::key_J;
        break;
    case Qt::Key_K:
        eNode.m_keyType = EventNode::KeyType::key_K;
        break;
    case Qt::Key_L:
        eNode.m_keyType = EventNode::KeyType::key_L;
        break;
    case Qt::Key_M:
        eNode.m_keyType = EventNode::KeyType::key_M;
        break;
    case Qt::Key_N:
        eNode.m_keyType = EventNode::KeyType::key_N;
        break;
    case Qt::Key_O:
        eNode.m_keyType = EventNode::KeyType::key_O;
        break;
    case Qt::Key_P:
        eNode.m_keyType = EventNode::KeyType::key_P;
        break;
    case Qt::Key_Q:
        eNode.m_keyType = EventNode::KeyType::key_Q;
        break;
    case Qt::Key_R:
        eNode.m_keyType = EventNode::KeyType::key_R;
        break;
    case Qt::Key_S:
        eNode.m_keyType = EventNode::KeyType::key_S;
        break;
    case Qt::Key_T:
        eNode.m_keyType = EventNode::KeyType::key_T;
        break;
    case Qt::Key_U:
        eNode.m_keyType = EventNode::KeyType::key_U;
        break;
    case Qt::Key_V:
        eNode.m_keyType = EventNode::KeyType::key_V;
        break;
    case Qt::Key_W:
        eNode.m_keyType = EventNode::KeyType::key_W;
        break;
    case Qt::Key_X:
        eNode.m_keyType = EventNode::KeyType::key_X;
        break;
    case Qt::Key_Y:
        eNode.m_keyType = EventNode::KeyType::key_Y;
        break;
    case Qt::Key_Z:
        eNode.m_keyType = EventNode::KeyType::key_Z;
        break;
    case Qt::Key_QuoteLeft:
        eNode.m_keyType = EventNode::KeyType::key_QuoteLeft;
        break;
    case Qt::Key_0:
        eNode.m_keyType = EventNode::KeyType::key_0;
        break;
    case Qt::Key_1:
        eNode.m_keyType = EventNode::KeyType::key_1;
        qDebug() << "1键";
        break;
    case Qt::Key_2:
        eNode.m_keyType = EventNode::KeyType::key_2;
        break;
    case Qt::Key_3:
        eNode.m_keyType = EventNode::KeyType::key_3;
        break;
    case Qt::Key_4:
        eNode.m_keyType = EventNode::KeyType::key_4;
        break;
    case Qt::Key_5:
        eNode.m_keyType = EventNode::KeyType::key_5;
        break;
    case Qt::Key_6:
        eNode.m_keyType = EventNode::KeyType::key_6;
        break;
    case Qt::Key_7:
        eNode.m_keyType = EventNode::KeyType::key_7;
        break;
    case Qt::Key_8:
        eNode.m_keyType = EventNode::KeyType::key_8;
        break;
    case Qt::Key_9:
        eNode.m_keyType = EventNode::KeyType::key_9;
        break;
    case Qt::Key_Minus:
        eNode.m_keyType = EventNode::KeyType::key_Minus;
        break;
    case Qt::Key_Equal:
        eNode.m_keyType = EventNode::KeyType::key_Equal;
        break;
    case Qt::Key_Backspace:
        eNode.m_keyType = EventNode::KeyType::key_Backspace;
        break;
    case Qt::Key_BracketLeft:
        eNode.m_keyType = EventNode::KeyType::key_BracketLeft;
        break;
    case Qt::Key_BracketRight:
        eNode.m_keyType = EventNode::KeyType::key_BracketRight;
        break;
    case Qt::Key_Backslash:
        eNode.m_keyType = EventNode::KeyType::key_Backslash;
        break;
    case Qt::Key_Return:
        eNode.m_keyType = EventNode::KeyType::key_Return;
        qDebug() << "type = enter";
        break;
    case Qt::Key_Tab:
        eNode.m_keyType = EventNode::KeyType::key_Tab;
        break;
    case Qt::Key_CapsLock:
        eNode.m_keyType = EventNode::KeyType::key_CapsLock;
        break;
    case Qt::Key_Shift:
        eNode.m_keyType = EventNode::KeyType::key_Shift;
        qDebug() << "type = shift";
        break;
    case Qt::Key_Control:
        eNode.m_keyType = EventNode::KeyType::key_Ctrl;
        qDebug() << "type = control";
        break;
    case Qt::Key_Meta:
        eNode.m_keyType = EventNode::KeyType::key_Meta;
        break;
    case Qt::Key_Alt:
        eNode.m_keyType = EventNode::KeyType::key_Alt;
        break;
    case Qt::Key_Space:
        eNode.m_keyType = EventNode::KeyType::key_Space;
        break;
    case Qt::Key_Semicolon:
        eNode.m_keyType = EventNode::KeyType::key_Semicolon;
        break;
    case Qt::Key_Apostrophe:
        eNode.m_keyType = EventNode::KeyType::key_Apostrophe;
        break;
    case Qt::Key_Comma:
        eNode.m_keyType = EventNode::KeyType::key_Comma;
        break;
    case Qt::Key_Period:
        eNode.m_keyType = EventNode::KeyType::key_Period;
        break;
    case Qt::Key_Slash:
        eNode.m_keyType = EventNode::KeyType::key_Slash;
        break;
    case Qt::Key_Escape:
        eNode.m_keyType = EventNode::KeyType::key_Esc;
        break;
        break;
    case Qt::Key_ScrollLock:
        eNode.m_keyType = EventNode::KeyType::key_ScrollLock;
        break;
    case Qt::Key_Print:
        eNode.m_keyType = EventNode::KeyType::key_Print;
        break;
    case Qt::Key_Pause:
        eNode.m_keyType = EventNode::KeyType::key_PauseBreak;
        break;
    case Qt::Key_Insert:
        eNode.m_keyType = EventNode::KeyType::key_Insert;
        break;
    case Qt::Key_Home:
        eNode.m_keyType = EventNode::KeyType::key_Home;
        break;
    case Qt::Key_PageUp:
        eNode.m_keyType = EventNode::KeyType::key_PageUp;
        break;
    case Qt::Key_PageDown:
        eNode.m_keyType = EventNode::KeyType::key_PageDown;
        break;
    case Qt::Key_Delete:
        eNode.m_keyType = EventNode::KeyType::key_Delete;
        break;
    case Qt::Key_End:
        eNode.m_keyType = EventNode::KeyType::key_End;
        break;
    case Qt::Key_Up:
        eNode.m_keyType = EventNode::KeyType::key_Up;
        break;
    case Qt::Key_Down:
        eNode.m_keyType = EventNode::KeyType::key_Down;
        break;
    case Qt::Key_Left:
        eNode.m_keyType = EventNode::KeyType::key_Left;
        break;
    case Qt::Key_Right:
        eNode.m_keyType = EventNode::KeyType::key_Right;
        break;
    case Qt::Key_AsciiTilde:
        eNode.m_keyType = EventNode::KeyType::key_AsciiTilde;
        qDebug() << "type = 波浪号";
        break;
    case Qt::Key_Exclam:
        eNode.m_keyType = EventNode::KeyType::key_Exclam;
        break;
    case Qt::Key_At:
        eNode.m_keyType = EventNode::KeyType::key_At;
        break;
    case Qt::Key_NumberSign:
        eNode.m_keyType = EventNode::KeyType::key_NumberSign;
        break;
    case Qt::Key_Dollar:
        eNode.m_keyType = EventNode::KeyType::key_Dollar;
        break;
    case Qt::Key_Percent:
        eNode.m_keyType = EventNode::KeyType::key_Percent;
        break;
    case Qt::Key_AsciiCircum:
        eNode.m_keyType = EventNode::KeyType::key_AsciiCircum;
        break;
    case Qt::Key_Ampersand:
        eNode.m_keyType = EventNode::KeyType::key_Ampersand;
        break;
    case Qt::Key_Asterisk:
        eNode.m_keyType = EventNode::KeyType::key_Asterisk;
        break;
    case Qt::Key_ParenLeft:
        eNode.m_keyType = EventNode::KeyType::key_ParenLeft;
        break;
    case Qt::Key_ParenRight:
        eNode.m_keyType = EventNode::KeyType::key_ParenRight;
        break;
        break;
    case Qt::Key_Underscore:
        eNode.m_keyType = EventNode::KeyType::key_Underscore;
        break;
    case Qt::Key_Plus:
        eNode.m_keyType = EventNode::KeyType::key_Plus;
        break;
    case Qt::Key_BraceLeft:
        eNode.m_keyType = EventNode::KeyType::key_BraceLeft;
        break;
    case Qt::Key_BraceRight:
        eNode.m_keyType = EventNode::KeyType::key_BraceLeft;
        break;
    case Qt::Key_Bar:
        eNode.m_keyType = EventNode::KeyType::key_Bar;
        break;
    case Qt::Key_Colon:
        eNode.m_keyType = EventNode::KeyType::key_Colon;
        break;
    case Qt::Key_QuoteDbl:
        eNode.m_keyType = EventNode::KeyType::key_QuoteDbl;
        break;
    case Qt::Key_Less:
        eNode.m_keyType = EventNode::KeyType::key_Less;
        break;
    case Qt::Key_Greater:
        eNode.m_keyType = EventNode::KeyType::key_Greater;
        break;
    case Qt::Key_Question:
        eNode.m_keyType = EventNode::KeyType::key_Question;
        break;

    default:
        flag = true;
        break;
    }

    return flag;
}

void View::keyPressEvent(QKeyEvent *event) {
     if (_vctrl->_session->status() == CSession::SocketStatus::Err)
         return;
    EventNode eNode(0, 0, 0, 0, EventNode::Type::keyPress, 0);
    if (getKetType(eNode, event->key())) {
        return;
    }
    _vctrl->eventAction(eNode);
}
void View::keyReleaseEvent(QKeyEvent *event) {
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(0, 0, 0, 0, EventNode::Type::keyRelease, 0);
    if (getKetType(eNode, event->key())) {
        return;
    }
    _vctrl->eventAction(eNode);
}

