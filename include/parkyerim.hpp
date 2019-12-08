#ifndef PARKYERIM_HPP
#define PARKYERIM_HPP

#include <QMainWindow>
#include <QString>

#include "applicationwindow.hpp"
#include "databasemanager.hpp"
#include "User.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class ParkYerim; }
QT_END_NAMESPACE

class ParkYerim : public QMainWindow
{
    Q_OBJECT

public:
    ParkYerim(QWidget *parent = nullptr);
    ~ParkYerim();



private slots:
    void on_pushButton_clicked();

private:
    Ui::ParkYerim *ui;
    DatabaseManager* m_database = nullptr;
    User* m_user = nullptr;

    bool validateLoginInfo();
    void launchProgram();

    const QString m_databasepath = "./assets/database/parkyerimdb.sqlite";

};
#endif // PARKYERIM_HPP