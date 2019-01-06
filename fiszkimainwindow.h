#ifndef FISZKIMAINWINDOW_H
#define FISZKIMAINWINDOW_H

#include <QMainWindow>
#include "session.h"

namespace Ui {
class FiszkiMainWindow;
}

class FiszkiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FiszkiMainWindow(QWidget *parent = nullptr);
    ~FiszkiMainWindow();

private slots:
    void on_checkBtn_clicked();
    void on_exitBtn_clicked();
    void on_userListBtn_clicked();
    void on_learnBtn_clicked();
    void on_endLearnBtn_clicked();
    void on_nextFlashcardBtn_clicked();
    void on_backFlashcardBtn_clicked();
    void on_rememberBtn_clicked();
    void on_stopBtn_clicked();
    void on_testBtn_clicked();
    void on_availableUsersComboBox_currentIndexChanged(const QString &arg1);
    void on_aboutBtn_clicked();

private:
    Ui::FiszkiMainWindow *ui;

    Session *session = new Session();

    void test();
    bool setBtns(infoQuestions info);

    QString user;
};

#endif // FISZKIMAINWINDOW_H
