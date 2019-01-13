#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User (QString setName, QObject *parent = nullptr);
    ~User();

    QString getUserName();
    int getNoBox();

signals:

public slots:

private:
    QString name;
    int noBox;
};

#endif // USER_H
