#include "parkyerim.hpp"
#include "ui_parkyerim.h"

#include <QTime>
#include <QDebug>


ParkYerim::ParkYerim(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ParkYerim)
{
    ui->setupUi(this);

    // initialize database instance
    m_database = new DatabaseManager(m_databasepath);
    m_logger = new Logger(m_logfilepath);
    if(!m_database->isConnected()){
        ui->label_status->setStyleSheet("color:red;");
        ui->label_status->setText("Veritabanına bağlanılamadı.");
        ui->pushButton->setEnabled(false);
    }
}

ParkYerim::~ParkYerim()
{
    delete ui;
    delete m_database;
    delete m_logger;
    if(m_user) delete m_user;
}

void ParkYerim::on_pushButton_clicked()
{
    if(validateLoginInfo()) launchProgram();
}

bool ParkYerim::validateLoginInfo()
{
    QString username = ui->lineEdit_user->text();
    QString password = ui->lineEdit_password->text();
    QString errormessage;
    if(!m_database->ValidateUserLogin(username,password,errormessage, &m_user)){
        ui->label_status->setText(errormessage);
        return false;
    }else return true;
}

void ParkYerim::launchProgram()
{
    ApplicationWindow* appwindow = new ApplicationWindow(m_database,m_user,m_logger);
    this->hide();
    appwindow->show();
}
