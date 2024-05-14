#ifndef CENTERCONTROL_H
#define CENTERCONTROL_H

#include <QObject>

class CenterControl : public QObject
{
    Q_OBJECT
public:
    explicit CenterControl(QObject *parent = nullptr);

signals:
};

#endif // CENTERCONTROL_H
