#ifndef ENDFORTODAY_H
#define ENDFORTODAY_H

#include <QDialog>

namespace Ui {
class EndForToday;
}

class EndForToday : public QDialog
{
    Q_OBJECT

public:
    explicit EndForToday(QWidget *parent = nullptr);
    ~EndForToday();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EndForToday *ui;
};

#endif // ENDFORTODAY_H
