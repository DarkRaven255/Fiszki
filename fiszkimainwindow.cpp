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

    ui->userListBtn->setEnabled(false);
    ui->availableUsersComboBox->setEnabled(false);

    setBtns();
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
    //setBtns();
}

void FiszkiMainWindow::on_testBtn_clicked()
{
    session = new Session();
    ui->stackedWidget->setCurrentIndex(2);
    //setBtns();
    test();
}

void FiszkiMainWindow::on_aboutBtn_clicked()
{
    AboutWindow aboutwindow;
    aboutwindow.exec();
}

void FiszkiMainWindow::setBtns()
{
    session->getButtonStatus(back,remember,next,noQuestionsInDB,noTestQuestions);

    //qDebug()<<"back:"<<back<<"remember:"<<remember<<"next:"<<next;

    ui->nextFlashcardBtn->setEnabled(next);
    ui->backFlashcardBtn->setEnabled(back);
    ui->rememberBtn->setEnabled(remember);
    ui->checkBtn->setEnabled(!noTestQuestions);
}

////////////////////////////////////LEARN PAGE
void FiszkiMainWindow::on_endLearnBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    delete session;
}

void FiszkiMainWindow::on_nextFlashcardBtn_clicked()
{

    if(!noQuestionsInDB)
    {

        session->nextLearnBtn();

        ui->questionEnTextBrowser->setText(session->question->getQ_en());
        ui->explanationEnTextBrowser->setText(session->question->getE_en());
        ui->questionPlTextBrowser->setText(session->question->getQ_pl());
        ui->explanationPlTextBrowser->setText(session->question->getE_pl());
    }
    else
    {
        ui->questionEnTextBrowser->setText("Zobaczyłeś");
        ui->explanationEnTextBrowser->setText("wszystkie");
        ui->questionPlTextBrowser->setText("fiszki");
        ui->explanationPlTextBrowser->setText("w bazie!");
    }
    setBtns();

}

void FiszkiMainWindow::on_backFlashcardBtn_clicked()
{
    session->backLearnBtn();

    ui->questionEnTextBrowser->setText(session->question->getQ_en());
    ui->explanationEnTextBrowser->setText(session->question->getE_en());
    ui->questionPlTextBrowser->setText(session->question->getQ_pl());
    ui->explanationPlTextBrowser->setText(session->question->getE_pl());

    setBtns();
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
    setBtns();
    if(!noTestQuestions)
    {
        session->testWords();

        ui->questionTextBrowser->setText(session->question->getQ_en());
        ui->explanationTextBrowser->setText(session->question->getE_en());
    }
    else
    {
        ui->questionTextBrowser->setText("");
        ui->explanationTextBrowser->setText("Dodaj nowe fiszki do powtórek, w opcji \"Nauka\"");
    }

}

void FiszkiMainWindow::on_checkBtn_clicked()
{
    session->checkAnswer(ui->enterAnwserLineEdit->text());
    ui->enterAnwserLineEdit->clear();
    ui->progressBar->setValue(session->getProgressPercent());
    test();
}
