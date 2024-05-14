#ifndef VIEWCONTROL_H
#define VIEWCONTROL_H

#include <QObject>

class ViewControl : public QObject
{
    Q_OBJECT
public:
    explicit ViewControl(QObject *parent = nullptr);

signals:
};

#endif // VIEWCONTROL_H
