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
    void on_aboutBtn_clicked();

    void on_setUserBtn_clicked();

private:
    Ui::FiszkiMainWindow *ui;

    Session *session = new Session();

    void test();
    void setBtns();
    void setWindowIndex(Status status);

    bool next;
    bool back;
    bool remember;
    bool noQuestionsInDB;
    bool noTestQuestions;
    bool check;

    QString user;

    Status currStatus;
};

#endif // FISZKIMAINWINDOW_H
