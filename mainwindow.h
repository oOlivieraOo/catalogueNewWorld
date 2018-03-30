#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QNetworkAccessManager>
#include<QJsonArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QNetworkAccessManager *pmyNWM,QString theName, QString theSurname, QString theIdentifiant, QWidget *parent = 0 );
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString name,surname,id;
    QNetworkReply *reply;
    QJsonArray jsArray;
    QNetworkAccessManager * myNWM;

public slots:
    void afficheLesCommandesAPreparer();
    void preparedChecked(bool coche);
    void rendreCompte();
private slots:
    void on_pushButtonQuit_clicked();
    void on_comboBoxSalePoint_activated(const QString &arg1);
};

#endif // MAINWINDOW_H
