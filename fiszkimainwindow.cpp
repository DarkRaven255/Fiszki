#include "fiszkimainwindow.h"
#include "ui_fiszkimainwindow.h"
#include "userlistwindow.h"
#include "addnewuserwindow.h"
#include "aboutwindow.h"
#include "addquestionwindow.h"
#include "enums.h"

#include <QDebug>

FiszkiMainWindow::FiszkiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FiszkiMainWindow)
{
    ui->setupUi(this);
    setWindowIndex(StatusMenu);

    ui->availableUsersComboBox->addItems(session->getUserList());
    connect(ui->enterAnwserLineEdit,SIGNAL(returnPressed()),ui->checkBtn,SIGNAL(clicked()));

    ui->questionTextBrowser->setFontPointSize(48);
    ui->questionEnTextBrowser->setFontPointSize(48);
    ui->questionPlTextBrowser->setFontPointSize(48);
    ui->explanationEnTextBrowser->setFontPointSize(10);
    ui->explanationPlTextBrowser->setFontPointSize(10);
    ui->explanationTextBrowser->setFontPointSize(10);

    setBtns();
}

FiszkiMainWindow::~FiszkiMainWindow()
{
    delete ui;
    delete session;
}


void FiszkiMainWindow::setBtns()
{
    session->getButtonStatus(back,remember,next,noQuestionsInDB,noTestQuestions,check,learn,testBtn);

    ui->nextFlashcardBtn->setEnabled(next);
    ui->backFlashcardBtn->setEnabled(back);
    ui->rememberBtn->setEnabled(remember);
    ui->checkBtn->setEnabled(check);
    ui->enterAnwserLineEdit->setEnabled(check);
    ui->testBtn->setEnabled(!noTestQuestions);
    ui->learnBtn->setEnabled(learn);

//    ui->testBtn->setEnabled(!ui->availableUsersComboBox->isEnabled());
//    ui->learnBtn->setEnabled(!ui->availableUsersComboBox->isEnabled());

    ui->learnBtn->setEnabled(learn);
    ui->testBtn->setEnabled(testBtn);

    ui->setUserBtn->setEnabled(!(ui->availableUsersComboBox->count()==0));
}

void FiszkiMainWindow::setWindowIndex(Status status)
{
    ui->stackedWidget->setCurrentIndex(status);
    currStatus=status;
}

/////////////////////////////////////////////MAIN MENU PAGE
void FiszkiMainWindow::on_exitBtn_clicked()
{
    close();
}

void FiszkiMainWindow::on_userListBtn_clicked()
{
    UserListWindow userlistwindow(session,this);
    userlistwindow.exec();
    if(&UserListWindow::destroyed)
    {
        ui->availableUsersComboBox->clear();
        ui->availableUsersComboBox->addItems(session->getUserList());
    }
    setBtns();
}

void FiszkiMainWindow::on_learnBtn_clicked()
{
    setWindowIndex(StatusLearnMode);
    on_nextFlashcardBtn_clicked();
}

void FiszkiMainWindow::on_testBtn_clicked()
{
    setWindowIndex(StatusTestMode);
    setBtns();
    session->testWords();
    test();
}

void FiszkiMainWindow::on_aboutBtn_clicked()
{
    AboutWindow aboutwindow;
    aboutwindow.exec();
}

void FiszkiMainWindow::on_setUserBtn_clicked()
{
    ui->testBtn->setEnabled(ui->availableUsersComboBox->isEnabled());
    ui->learnBtn->setEnabled(ui->availableUsersComboBox->isEnabled());
    if(!ui->availableUsersComboBox->isEnabled())
    {
        ui->setUserBtn->setText("OK");
        session->deleteUser();
    }
    else
    {
        ui->setUserBtn->setText("Zmień");
        session->setUser(ui->availableUsersComboBox->currentText());
        ui->activeUser->setText(session->getUser());
        ui->activeUser_2->setText(session->getUser());
    }
    ui->availableUsersComboBox->setEnabled(!ui->availableUsersComboBox->isEnabled());
    setBtns();
}

////////////////////////////////////LEARN PAGE
void FiszkiMainWindow::on_endLearnBtn_clicked()
{
    session->exportBoxToDB(currStatus);
    setWindowIndex(StatusMenu);
    session->setUserAction(LastActionLearn);
    setBtns();
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
    session->markWord();
    on_nextFlashcardBtn_clicked();
}

/////////////////////////////////////////TEST PAGE
void FiszkiMainWindow::test()
{
    ui->progressBar->setValue(session->getProgressPercent());
    if(check)
    {
        ui->questionTextBrowser->setText(session->question->getQ_en());
        ui->explanationTextBrowser->setText(session->question->getE_en());
    }
    else
    {
        ui->questionTextBrowser->setText("Koniec");
        ui->explanationTextBrowser->setText("Wróć jutro, lub dodaj nowe fiszki do powtórek, w opcji \"Nauka\"");
    }
}

void FiszkiMainWindow::on_stopBtn_clicked()
{
    session->exportBoxToDB(currStatus);
    setWindowIndex(StatusMenu);
    setBtns();
    session->setUserAction(LastActionTest);
}


void FiszkiMainWindow::on_checkBtn_clicked()
{
    session->checkAnswer(ui->enterAnwserLineEdit->text());
    ui->enterAnwserLineEdit->clear();
    setBtns();
    test();
}

void FiszkiMainWindow::on_addWordsBtn_clicked()
{
    AddQuestionWindow addquestionwindow(session, this);
    addquestionwindow.exec();
    setBtns();
}
