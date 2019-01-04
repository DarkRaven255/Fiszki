#include "fiszkimainwindow.h"
#include "ui_fiszkimainwindow.h"
#include "userlistwindow.h"
#include "addnewuserwindow.h"

QString xD="";

FiszkiMainWindow::FiszkiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FiszkiMainWindow),
    numberOfQuestions(dbmanager->countQuestions()),
    lastBtn(1)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->availableUsersComboBox->addItems(dbmanager->displayAllUsers());
    connect(ui->enterAnwserLineEdit,SIGNAL(returnPressed()),ui->checkBtn,SIGNAL(clicked()));

    ui->questionTextBrowser->setFontPointSize(48);
    ui->questionEnTextBrowser->setFontPointSize(48);
    ui->questionPlTextBrowser->setFontPointSize(48);
    ui->explanationEnTextBrowser->setFontPointSize(10);
    ui->explanationPlTextBrowser->setFontPointSize(10);
    ui->explanationTextBrowser->setFontPointSize(10);

    qDebug()<<"Liczba pytan"<<numberOfQuestions;
}

FiszkiMainWindow::~FiszkiMainWindow()
{
    delete ui;
    dbmanager->closeUserDB();
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
        ui->availableUsersComboBox->addItems(dbmanager->displayAllUsers());
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

    counterOfQuestions=0;
    LoadQuestion(0);

    if(numberOfQuestions>0) ui->nextFlashcardBtn->setEnabled(true);
    else LockBtns();
}


void FiszkiMainWindow::on_testBtn_clicked()
{
    if(ui->availableUsersComboBox->count()==0)
    {
        AddNewUserWindow addnewuserwindow;
        addnewuserwindow.exec();
    }
    ui->stackedWidget->setCurrentIndex(2);
    Test();
}


////////////////////////////////////LEARN PAGE
void FiszkiMainWindow::LoadQuestion(int i)
{
    counterOfQuestions+=i;
    if(counterOfQuestions==numberOfQuestions-1)
    {
        ui->nextFlashcardBtn->setEnabled(false);
    }
    else if(counterOfQuestions==0)
    {
        ui->backFlashcardBtn->setEnabled(false);
    }
    else if(counterOfQuestions>numberOfQuestions-1)
    {
        counterOfQuestions=numberOfQuestions-1;
        ui->nextFlashcardBtn->setEnabled(false);
    }
    else if(counterOfQuestions<0)
    {
        counterOfQuestions=0;
        ui->backFlashcardBtn->setEnabled(false);
    }

    if (numberOfQuestions==1)
    {
        ui->backFlashcardBtn->setEnabled(false);
        ui->nextFlashcardBtn->setEnabled(false);
    }

    if(numberOfQuestions==0)
    {
        LockBtns();
    }
    else
    {
        dbmanager->returnQuestion(counterOfQuestions,q_en,e_en,q_pl,e_pl);

        ui->questionEnTextBrowser->setText(q_en);
        ui->explanationEnTextBrowser->setText(e_en);

        ui->questionPlTextBrowser->setText(q_pl);
        ui->explanationPlTextBrowser->setText(e_pl);
    }
}

void FiszkiMainWindow::RecalculateQuestions()
{
    numberOfQuestions=dbmanager->countQuestions();
}

void FiszkiMainWindow::LockBtns()
{
    ui->questionEnTextBrowser->setText("Zobaczyłeś");
    ui->explanationEnTextBrowser->setText("wszystkie");
    ui->questionPlTextBrowser->setText("fiszki");
    ui->explanationPlTextBrowser->setText("w bazie!");
    ui->nextFlashcardBtn->setEnabled(false);
    ui->backFlashcardBtn->setEnabled(false);
    ui->rememberBtn->setEnabled(false);
}

void FiszkiMainWindow::on_endLearnBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void FiszkiMainWindow::on_nextFlashcardBtn_clicked()
{
    LoadQuestion(1);
    if(!ui->backFlashcardBtn->isEnabled())ui->backFlashcardBtn->setEnabled(true);
}


void FiszkiMainWindow::on_backFlashcardBtn_clicked()
{
    LoadQuestion(-1);
    if(!ui->nextFlashcardBtn->isEnabled())ui->nextFlashcardBtn->setEnabled(true);
}


void FiszkiMainWindow::on_rememberBtn_clicked()
{
    dbmanager->markAsKnown(q_en);
    RecalculateQuestions();
    LoadQuestion(0);
}

void FiszkiMainWindow::on_stopBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

/////////////////////////////////////////TEST PAGE
void FiszkiMainWindow::Test()
{
    //ui->questionTextBrowser->
}


void FiszkiMainWindow::on_checkBtn_clicked()
{
    if(ui->enterAnwserLineEdit->text()==q_pl)
    {
        ui->progressBar->setValue(10);
    }
}
