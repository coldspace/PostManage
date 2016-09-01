#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QTableWidgetItem>
#include <QFile>
#include <QTime>
#include <QTimer>
#include <QDateTime>
/*用户头文件*/

#include "adduser.h"
#include "addpost.h"
#include "datatype.h"
#include "serial.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_MAddUser_clicked();
    void GetNewUser(const PersonInfo &);
    void on_MSaveUser_clicked();

    void UpdatePerson();
    void UpdatePost();
    void UpdateHistory();

    bool on_MLoadUser_clicked();
    void on_MDeleteUser_clicked();
    void on_seekserial_clicked();
    void on_SerialSwitch_clicked();
    void on_DAddUser_clicked();
    void DealSerialData(const QString &);
    void on_DDeleteUser_clicked();
    void timeinterreptdone();
    void on_AddPost_clicked();
    void GetNewPost(const PostInfo &);
    void on_LoadPost_clicked();
    void on_RemovePost_clicked();
    void on_PickPost_clicked();

    void on_ClearHistory_clicked();

    void on_LoadHistory_clicked();

    void on_baud_activated(int index);

    void on_pushButton_clicked();

    void on_SearchPost_clicked();

    void on_DClearUser_clicked();

    void on_DClearPost_clicked();

    void on_DclearFinger_clicked();

signals:
    void PersonChange();
    void PostChange();
    void HistoryChange();

private:  
    void sleep(unsigned int msec);
    void addperson(QString &);
    void deleteperson(QString &);
    bool SavetoFile();

    void addpost(QString &);
    void deletepost(QString &);
    bool PostSavetoFile();
    bool PostLoad();

    void AddHistory(QString &);
    bool HistorySavetoFile();
    bool HistoryLoad();

    Ui::MainWindow *ui;
    Serial *ser;
    AddUser *adduser;
    AddPost *addpostdialog;
    QMap<int,PersonInfo> PnInfo;
    QMap<int,PostInfo> PoInfo;
    QMultiMap<int,HistoryInfo> HisInfo;
    int row;
    PersonInfo Pinf;
    PostInfo poinf;
    HistoryInfo HInfo;
    QTimer *timeinterrept;

    /*串口相关*/
};

#endif // MAINWINDOW_H
