#include "viewcontrol.h"

#include "centerview.h"
#include "viewwindow.h"
ViewControl::ViewControl()
    : _viewWindow { new ViewWindow(this) }
// , _centerView { new CenterView() }
{
    _viewWindow->show();
    _viewWindow->setWindowTitle("远程控制系统主界面");
}

ViewControl::~ViewControl() noexcept
{
    delete _viewWindow;
    delete _centerView;
}

ViewControl& ViewControl::singleton()
{
    static ViewControl viewControl;
    return viewControl;
}

QSize ViewControl::prefferdSize()
{
    QSize prefferdsize(_centerView->width() * 0.6, _centerView->height() * 0.6 + 25);
    return prefferdsize;
}

// void ViewControl::updatePixmap(const QPixmap& pixmap)
// {
// }
