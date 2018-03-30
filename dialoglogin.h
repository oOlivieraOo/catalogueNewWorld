#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>


namespace Ui {
class DialogLogin;
}

class DialogLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogin(QWidget *parent = 0);
    ~DialogLogin();
    QString identifiant,nom,prenom;
    QNetworkAccessManager myNWM;
    QNetworkCookieJar cookieJar;

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonConnect_clicked();

private:
    Ui::DialogLogin *ui;
};

#endif // DIALOGLOGIN_H
