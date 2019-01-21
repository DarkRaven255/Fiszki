#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User (const QString &setName, const QString &setNoBox, const long long &setStartDate,
                   const long long &setlastUsed, const int &setLastAction, const int &setUnknownQuestions, QObject *parent = nullptr);
    ~User();

    QString getUserName();
    QString getNoBox();

    long long getLastUsed();
    void setLastUsed(const int &setLastUsed);

    long long getStartDate();

    int getLastAction();
    void setLastAction(const int &action);

    int getUnknownQuestions();
    void setUnknownQuestions(const int &setUnknownQuestions);

signals:

public slots:

private:
    QString name;
    QString noBox;
    long long lastUsed;
    long long startDate;
    int lastAction;
    int unknownQuestions;
};

#endif // USER_H
