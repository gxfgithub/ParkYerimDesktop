#include "applicationwindow.hpp"
#include "ui_applicationwindow.h"

#include <QMessageBox>
#include <QTimer>

#include "parkyerim.hpp"


ApplicationWindow::ApplicationWindow(DatabaseManager* dbmanager, User* user, Logger* logger, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplicationWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::CustomizeWindowHint);
    this->setAttribute( Qt::WA_DeleteOnClose, true );
    m_parent = static_cast<ParkYerim*>(parent);
    m_dbmanager = dbmanager;
    m_user = user;
    m_logger = logger;
    initializeAssetPaths();
    setupIcons();
    setupCustomComponents();
}

ApplicationWindow::~ApplicationWindow()
{
    delete ui;
}

void ApplicationWindow::ClearVehicleInStats()
{
    ui->lineEdit_in_plate->setText("");
    ui->lineEdit_in_color->setText("");
    ui->lineEdit_in_model->setText("");
    ui->lineEdit_in_type->setText("");
}

void ApplicationWindow::ClearVehicleOutStats()
{
    ui->lineEdit_out_plate->setText("");
    ui->lineEdit_out_color->setText("");
    ui->lineEdit_out_model->setText("");
    ui->lineEdit_out_type->setText("");
    ui->lineEdit_out_parkLocation->setText("");
    ui->lineEdit_out_price->setText("");
    ui->dateTimeEdit_in->setDateTime(QDateTime(QDate(2000,1,1),QTime(0,0)));
    ui->dateTimeEdit_out->setDateTime(QDateTime(QDate(2000,1,1),QTime(0,0)));
}

QMap<QString, QString> ApplicationWindow::GetAssetPaths()
{
    return m_assetPaths;
}

DatabaseManager *ApplicationWindow::GetDBManager()
{
    return m_dbmanager;
}

float ApplicationWindow::calculatePrice(qint64 minutes, bool isNight, QString& currentplan)
{
    if(currentPricingPlan){
        if(isNight) currentplan = currentPricingPlan->GetPlanName() + " (GECE)";
        else currentplan = currentPricingPlan->GetPlanName() + " (GÜNDÜZ)";
    }
    return this->getPricePlanCalculation(minutes,isNight);
}


QList<PricingPlan *> ApplicationWindow::GetPricingPlanList()
{
    return m_pricingPlans;
}

void ApplicationWindow::on_toolButton_quit_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Çıkış", "Programı kapatmak istediğinizden emin misiniz?", QMessageBox::Yes|QMessageBox::Cancel);
    if(reply == QMessageBox::Yes){
        QApplication::quit();
    }
}

void ApplicationWindow::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("HH:mm");
    if ((time.second() % 2) == 0) text[2] = ' ';
    ui->lcdNumber->display(text);
    // calculate if its day or night, then update m_isNight
}

void ApplicationWindow::on_toolButton_vehicle_in_clicked()
{
    m_window_vehicle_in = new ManualVehicleEntry(m_dbmanager,m_isNight,this);
    m_window_vehicle_in->exec();
}

void ApplicationWindow::on_toolButton_vehicle_out_clicked()
{
    m_window_vehicle_out = new ManualVehicleExit(m_dbmanager,this);
    m_window_vehicle_out->exec();
}

void ApplicationWindow::initializeAssetPaths()
{
    // setting up asset paths
    for(QString s : m_params)
    {
        QList<QString> param = s.split(',' , QString::SkipEmptyParts);
        QString pName = param[0].trimmed();
        QString pValue = param[1].trimmed();
        m_assetPaths[pName] = pValue;
    }
}

void ApplicationWindow::setupIcons()
{
    // setting up icons
    ui->toolButton_quit->setIcon(QIcon(m_assetPaths["icon_close"]));
    ui->toolButton_settings->setIcon(QIcon(m_assetPaths["icon_settings"]));
    ui->toolButton_adminpanel->setIcon(QIcon(m_assetPaths["icon_adminpanel"]));
    ui->toolButton_vehicle_in->setIcon(QIcon(m_assetPaths["icon_vehicle_in"]));
    ui->toolButton_vehicle_out->setIcon(QIcon(m_assetPaths["icon_vehicle_out"]));
}

void ApplicationWindow::setupCustomComponents()
{
    // setting up digital clock
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ApplicationWindow::showTime);
    timer->start(1000);
    showTime();

    // setting up admin panel button
    ui->label_user->setText(m_user->getFirstName() + " " + m_user->getLastName());
    if(m_user->getUserType() < 2) ui->toolButton_adminpanel->setEnabled(false);

    // setting up pricing plans
    QString errmsg;
    if(!m_dbmanager->GetPricingPlans(m_pricingPlans,errmsg)) ui->label_status->setText(errmsg);
    else{
        for(PricingPlan* plan : m_pricingPlans){
            if(m_currentPlanID == plan->GetPlanID()){
                currentPricingPlan = plan;
                ui->label_currentPlan->setText(currentPricingPlan->GetPlanName());
                break;
            }
        }
        if(!currentPricingPlan) ui->label_status->setText("Geçerli ücretlendirme planı veritabanında bulunamadı.");
        else{
            connect(this,&ApplicationWindow::getPricePlanCalculation,currentPricingPlan,&PricingPlan::CalculatePrice);
        }
    }
}

void ApplicationWindow::on_toolButton_adminpanel_clicked()
{
    m_window_admin = new AdminPanel(m_dbmanager,this);
    m_window_admin->show();
}

void ApplicationWindow::on_toolButton_settings_clicked()
{
    m_window_settings = new SettingsPanel(m_dbmanager,m_pricingPlans,this);
    m_window_settings->exec();
}

void ApplicationWindow::on_toolButton_currentPlanDetails_clicked()
{

}

void ApplicationWindow::on_toolButton_parkingSpots_clicked()
{

}

void ApplicationWindow::on_toolButton_securityCams_clicked()
{

}

void ApplicationWindow::on_toolButton_help_clicked()
{

}
