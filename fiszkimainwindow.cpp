#include "fiszkimainwindow.h"
#include "ui_fiszkimainwindow.h"
#include "userlistwindow.h"
#include "addnewuserwindow.h"
#include "aboutwindow.h"
#include "question.h"
#include "enums.h"

#include <QDebug>

FiszkiMainWindow::FiszkiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FiszkiMainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    ui->availableUsersComboBox->addItems(session->getUserList());
    connect(ui->enterAnwserLineEdit,SIGNAL(returnPressed()),ui->checkBtn,SIGNAL(clicked()));

    ui->questionTextBrowser->setFontPointSize(48);
    ui->questionEnTextBrowser->setFontPointSize(48);
    ui->questionPlTextBrowser->setFontPointSize(48);
    ui->explanationEnTextBrowser->setFontPointSize(10);
    ui->explanationPlTextBrowser->setFontPointSize(10);
    ui->explanationTextBrowser->setFontPointSize(10);


}

FiszkiMainWindow::~FiszkiMainWindow()
{
    delete ui;
    delete session;
}

/////////////////////////////////////////////MAIN MENU PAGE
void FiszkiMainWindow::on_exitBtn_clicked()
{
    close();
}

void FiszkiMainWindow::on_userListBtn_clicked()
{
    UserListWindow userlistwindow;
    userlistwindow.exec();
    if(&UserListWindow::destroyed)
    {
        ui->availableUsersComboBox->clear();
        ui->availableUsersComboBox->addItems(session->getUserList());
    }
}

void FiszkiMainWindow::on_availableUsersComboBox_currentIndexChanged(const QString &selectedUser)
{
    user=selectedUser;
    ui->activeUser->setText(user);
    ui->activeUser_2->setText(user);
}

void FiszkiMainWindow::on_learnBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    on_nextFlashcardBtn_clicked();
    setBtns(session->infoQuestions());
}

void FiszkiMainWindow::on_testBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    setBtns(session->infoQuestions());
    test();
}

void FiszkiMainWindow::on_aboutBtn_clicked()
{
    AboutWindow aboutwindow;
    aboutwindow.exec();
}

bool FiszkiMainWindow::setBtns(infoQuestions info)
{

    switch (info)
    {
    case Null:
        break;
    case LockNext:
        ui->nextFlashcardBtn->setEnabled(false);
        break;
    case LockBack:
        ui->backFlashcardBtn->setEnabled(false);
        break;
    case LockNextBack:
        ui->nextFlashcardBtn->setEnabled(false);
        ui->backFlashcardBtn->setEnabled(false);
        break;
    case LockRemember:
        ui->rememberBtn->setEnabled(false);
        break;
    case LockAll:
        ui->nextFlashcardBtn->setEnabled(false);
        ui->backFlashcardBtn->setEnabled(false);
        ui->rememberBtn->setEnabled(false);
        ui->questionEnTextBrowser->setText("Zobaczyłeś");
        ui->explanationEnTextBrowser->setText("wszystkie");
        ui->questionPlTextBrowser->setText("fiszki");
        ui->explanationPlTextBrowser->setText("w bazie!");
        return true;
    }
    return false;
}

////////////////////////////////////LEARN PAGE
void FiszkiMainWindow::on_endLearnBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void FiszkiMainWindow::on_nextFlashcardBtn_clicked()
{
    qDebug()<<session->infoQuestions();
    if(session->infoQuestions()!=LockAll)
    {
        session->learnWords();

        ui->questionEnTextBrowser->setText(session->question->getQ_en());
        ui->explanationEnTextBrowser->setText(session->question->getE_en());
        ui->questionPlTextBrowser->setText(session->question->getQ_pl());
        ui->explanationPlTextBrowser->setText(session->question->getE_pl());
    }
    setBtns(session->infoQuestions());
}

void FiszkiMainWindow::on_backFlashcardBtn_clicked()
{
    //LoadQuestion(-1,-1);
    //if(!ui->nextFlashcardBtn->isEnabled())ui->nextFlashcardBtn->setEnabled(true);
}

void FiszkiMainWindow::on_rememberBtn_clicked()
{
    session->markQuestion();
    on_nextFlashcardBtn_clicked();
}

void FiszkiMainWindow::on_stopBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

/////////////////////////////////////////TEST PAGE
void FiszkiMainWindow::test()
{
//    int noBox=6;
//    testCounterQuestions+=i;
//    if(testCounterQuestions>testQuestions-1)
//    {
//        testCounterQuestions=testQuestions-1;
//        ui->questionTextBrowser->clear();
//        ui->explanationTextBrowser->setText("Dodaj nowe fiszki do powtórek, w opcji \"Nauka\"");
//        ui->checkBtn->setEnabled(false);
//        ui->enterAnwserLineEdit->setEnabled(false);
//    }
//    else
//    {
//        dbmanager->returnQuestion(testCounterQuestions,noBox,q_id,q_en,e_en,q_pl,e_pl);
//        ui->questionTextBrowser->setText(q_en);
//        ui->explanationTextBrowser->setText(e_en);
//    }

//    if(session->infoQuestions()!=LockAll)
//    {
        session->testWords();

        ui->questionTextBrowser->setText(session->question->getQ_en());
        ui->explanationTextBrowser->setText(session->question->getE_en());
//    }
//    setBtns(session->infoQuestions());
}

void FiszkiMainWindow::on_checkBtn_clicked()
{
    session->checkAnswer(ui->enterAnwserLineEdit->text());
    ui->enterAnwserLineEdit->clear();
    ui->progressBar->setValue(session->getProgressPercent());
    test();
}
