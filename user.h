#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User (QString setName, QString setNoBox, QObject *parent = nullptr);
    ~User();

    QString getUserName();
    QString getNoBox();

signals:

public slots:

private:
    QString name;
    QString noBox;
};

#endif // USER_H
