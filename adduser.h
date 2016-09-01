#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>

#include "datatype.h"

namespace Ui {
class AddUser;
}

class AddUser : public QDialog
{
    Q_OBJECT

public:
    explicit AddUser(QWidget *parent = 0);
    ~AddUser();

signals:
    void SendNewUser(const PersonInfo &);

private slots:

    void on_SaveButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::AddUser *ui;
    PersonInfo PInfo;
};

#endif // ADDUSER_H
