#include "mainwindow.h"
#include "ui_mainwindow.h"
#define URL2 "http://gthom.btsinfogap.org/newWorld/jsonListeDesCdes.php"
#define URLENRPREPARED "http://gthom.btsinfogap.org/newWorld/preparerLDC.php"
#include <QUrl>
#include <QUrlQuery>
#include <QJsonArray>
#include<QJsonObject>
#include<QCheckBox>
#include <QJsonDocument>
#include <QNetworkReply>

MainWindow::MainWindow(QNetworkAccessManager *pmyNWM, QString theName, QString theSurname, QString theIdentifiant, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myNWM=pmyNWM;
    name=theName;
    surname=theSurname;
    id=theIdentifiant;
    ui->labelIdentifiant->setText(id);
    ui->labelNom->setText(name);
    ui->labelPrenom->setText(surname);
    /***********/
    //verif du user et du pass
    QUrl serviceUrl(URL2);
    QUrl donnees;
    QUrlQuery query;


    QNetworkRequest request(serviceUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

   QByteArray postData;
   reply = myNWM->post(request,postData);
   connect(reply,SIGNAL(finished()),this,SLOT(afficheLesCommandesAPreparer()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::afficheLesCommandesAPreparer()
{
    qDebug()<<"void MainWindow::afficheLesCommandesAPreparer()";
    QByteArray response_data = reply->readAll();
    //ui->labelReponse->setText(response_data);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

    jsArray=jsonResponse.array();
    int nbLDC=jsArray.count();

    qDebug()<<"nombre de ligne de commandes reçues:"<<nbLDC;
    ui->tableWidgetLDC->setRowCount(nbLDC);
    //affichage du nb de commande
    ui->labelOrderCount->setText(QString::number(nbLDC));
    //affichage de la date du jour ou du lendemain
    if(nbLDC==0)
    {
        ui->labelOrdersDate->setText(QDate::currentDate().addDays(1).toString("dd/MM/yyyy"));
    }

    QStringList listeDesPointDeVente;
    for(unsigned int noLdc=0;noLdc<nbLDC;noLdc++)
    {
        if(noLdc==0)
        {
           ui->labelOrdersDate->setText( jsArray[noLdc].toObject()["dateLivraison"].toString());
        }
        QCheckBox* checkBoxPrepared=new QCheckBox("YES",ui->tableWidgetLDC);
        checkBoxPrepared->setProperty("numeroLDC",jsArray[noLdc].toObject()["numeroLDC"].toString());
        connect(checkBoxPrepared,SIGNAL(clicked(bool)),this,SLOT(preparedChecked(bool)));
        ui->tableWidgetLDC->setCellWidget(noLdc,0,checkBoxPrepared);
        ui->tableWidgetLDC->setItem(noLdc,1,new QTableWidgetItem(jsArray[noLdc].toObject()["heureRetraitPrevue"].toString()));
        QString pdv=jsArray[noLdc].toObject()["PDV"].toString();
        if(!(listeDesPointDeVente.contains(pdv)))listeDesPointDeVente.append(pdv);
        ui->tableWidgetLDC->setItem(noLdc,2,new QTableWidgetItem(pdv));
        ui->tableWidgetLDC->setItem(noLdc,3,new QTableWidgetItem(jsArray[noLdc].toObject()["nom"].toString()));
        ui->tableWidgetLDC->setItem(noLdc,4,new QTableWidgetItem(jsArray[noLdc].toObject()["prenom"].toString()));
        ui->tableWidgetLDC->setItem(noLdc,5,new QTableWidgetItem(jsArray[noLdc].toObject()["libelle"].toString()));
        ui->tableWidgetLDC->setItem(noLdc,6,new QTableWidgetItem(jsArray[noLdc].toObject()["quantite"].toString()));
        ui->tableWidgetLDC->setItem(noLdc,7,new QTableWidgetItem(jsArray[noLdc].toObject()["uniteDeVente"].toString()));
    }
    ui->tableWidgetLDC->resizeColumnsToContents();
    ui->comboBoxSalePoint->clear();
    ui->comboBoxSalePoint->insertItems(0,listeDesPointDeVente);
}
void MainWindow::preparedChecked(bool coche)
{
    if(coche)
    {
        QUrl serviceUrl(URLENRPREPARED);
        QUrl donnees;
        QUrlQuery query;
        query.addQueryItem("numeroLDC", ((QCheckBox*)sender())->property("numeroLDC").toString());
        donnees.setQuery(query);
        QByteArray postData(donnees.toString(QUrl::RemoveFragment).remove("?").toLatin1());
        QNetworkRequest request(serviceUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        reply = myNWM->post(request,postData);
       connect(reply,SIGNAL(finished()),this,SLOT(rendreCompte()));
    }
    else
    {
        //décochée: on annule
    }
}
void MainWindow::rendreCompte()
{
    qDebug()<<"La Préparation est bien enregistrée";
}
void MainWindow::on_pushButtonQuit_clicked()
{
    close();
}

void MainWindow::on_comboBoxSalePoint_activated(const QString &arg1)
{
    qDebug()<<"void MainWindow::on_comboBoxSalePoint_activated(const QString &arg1)";
    //je vide mon tableWidget
    ui->tableWidgetLDC_2->setRowCount(0);
    //je parcours mon jsArray et pour chaque ldc qui correspond je l'affiche
    int nbLDCDuPdv=0;
    for(unsigned int noLigne=0;noLigne<jsArray.size();noLigne++)
    {
        qDebug()<<noLigne;
        if(jsArray[noLigne].toObject()["PDV"].toString()==arg1)
        {
            ui->tableWidgetLDC_2->setRowCount(nbLDCDuPdv+1);
            ui->tableWidgetLDC_2->setCellWidget(nbLDCDuPdv,0,new QCheckBox("Deliver",ui->tableWidgetLDC));
            ui->tableWidgetLDC_2->setItem(nbLDCDuPdv,1,new QTableWidgetItem(jsArray[noLigne].toObject()["nom"].toString()));
            ui->tableWidgetLDC_2->setItem(nbLDCDuPdv,2,new QTableWidgetItem(jsArray[noLigne].toObject()["prenom"].toString()));
            ui->tableWidgetLDC_2->setItem(nbLDCDuPdv,3,new QTableWidgetItem(jsArray[noLigne].toObject()["libelle"].toString()));
            ui->tableWidgetLDC_2->setItem(nbLDCDuPdv,4,new QTableWidgetItem(jsArray[noLigne].toObject()["quantite"].toString()));
            ui->tableWidgetLDC_2->setItem(nbLDCDuPdv,5,new QTableWidgetItem(jsArray[noLigne].toObject()["uniteDeVente"].toString()));
            nbLDCDuPdv++;
        }
    }
    //ajustement des largeurs de colonne au contenu
    ui->tableWidgetLDC_2->setAlternatingRowColors(true);
    ui->tableWidgetLDC_2->resizeColumnsToContents();
}
