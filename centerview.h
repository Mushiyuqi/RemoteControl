#ifndef CENTERVIEW_H
#define CENTERVIEW_H

#include <QLabel>
#include <memory>
class Data;
class CSessionThread;

class CenterView : public QLabel {
    Q_OBJECT
public:
    CenterView(QWidget* parent = nullptr);

    // 用于获得CenterView的宽高
    int width();
    int height();

public slots:
    // 处理接收到的数据，并更新界面
    void onReadyForDisplay();

private:
    std::shared_ptr<CSessionThread> _session = nullptr;

    // 屏幕的宽高
    int _width;
    int _height;
};

#endif // CENTERVIEW_H
