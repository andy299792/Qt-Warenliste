#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QtCore>
//#include <QtGui>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
}

mainWindow::~mainWindow()
{
    delete ui;
}


void mainWindow::on_add_clicked()   // Add Button geklickt
{
    this->addItem();
}

void mainWindow::on_pushButton_clicked()    // Delete Button geklickt
{
    //if(this->ui->tableWidget->rowCount() == 0){return;}               // verhindert Absturz bei leerer Liste
    if(this->ui->tableWidget->selectedItems().count() == 0){return;}    // verhindert Absturz ohne Auswahl
    QString text = "möchten sie die Zeile wirklich löschen?";
    for(int i=0; i < this->ui->tableWidget->selectedItems().count() && i < 10; i++) // anzeigen der zu löschenden Einträge max. 9 St.
    {
        if(i == 9){ text += "\n...";}
        else{text += "\n" + this->ui->tableWidget->selectedItems().at(i)->text();}
    }
    QMessageBox::StandardButton eingabe;
    eingabe = QMessageBox::question(this, "löschen", text, QMessageBox::Yes | QMessageBox::No);

    if(eingabe == QMessageBox::Yes)
    {
        //this->deleteItem();   // nur für einzelne Zeilen
        this->deleteItems();    // für eine oder mehrere Zeilen
    }
}

void mainWindow::on_button_open_clicked()
{
    this->tableOpen();
}

void mainWindow::on_button_save_clicked()
{
   this->tableSave();
}

void mainWindow::on_actionOpen_triggered()
{
    this->tableOpen();
}

void mainWindow::on_actionSave_triggered()
{
    this->tableSave();
}

void mainWindow::on_tableWidget_cellClicked(int row, int column)
{
    this->aktiveZeile = row;
}

void mainWindow::addItem()
{
    this->ui->statusbar->clearMessage();
    auto id = this->ui->idNr->text().trimmed();
    if(id.length() != 4)
    {
        this->ui->statusbar->showMessage("Fehler, ID-Nr. muss genau 4 Zeichen haben", 10000);
        //this->eingabeclear();     // leeren ist hier unpraktisch
        return;
    }
    auto bez = this->ui->bezeichnung->text().trimmed();
    auto preis = this->ui->preis->text().trimmed();

    QTableWidgetItem *item_id = new QTableWidgetItem(" ");
    QTableWidgetItem *item_bez = new QTableWidgetItem(" ");
    QTableWidgetItem *item_preis = new QTableWidgetItem(" ");

    this->ui->tableWidget->setSortingEnabled(false);    // sortieren ausschalten
    this->ui->tableWidget->insertRow(0);                // Zeile einfügen in Zeile 0
    item_id->setText(id);                               // item mit Text füllen
    this->ui->tableWidget->setItem(0, 0, item_id);      // item an Tabelle übergeben (zeile, spalte)
    this->ui->tableWidget->item(0,0)->setFlags(Qt::ItemIsEditable);
    item_bez->setText(bez);
    this->ui->tableWidget->setItem(0, 1, item_bez);
    item_preis->setText(preis);
    this->ui->tableWidget->setItem(0, 2, item_preis);

    this->eingabeclear();
    this->ui->tableWidget->setSortingEnabled(true);     // sortieren einschalten
}

void mainWindow::eingabeclear()
{
    // Eingabefelder leeren
    this->ui->idNr->clear();
    this->ui->bezeichnung->clear();
    this->ui->preis->clear();
}

void mainWindow::deleteItem()   // nur einzelne Zeilen löschen, nicht mehrere
{
    this->ui->tableWidget->removeRow(this->aktiveZeile);
}

void mainWindow::deleteItems()   // mehrere Zeilen löschen
{
    auto auswahlListe = this->ui->tableWidget->selectedItems();

    for(int i=0; i < auswahlListe.count(); i++)
    {
        this->ui->tableWidget->removeRow(auswahlListe.at(i)->row());
    }
}

void mainWindow::tableSave()
{
    // Liste in der Datei speichern
    QFile file("liste.txt");
    // Wenn öffnen der Datei erfolgreich
    // Nur schreibzugriff -> WriteOnly
    // Inhalt der Datei überschreiben -> Truncate
    if(file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
    {
        // Schreibe liste in Datei
        // öffne TextStream
        QTextStream out(&file);
        // Header horizontal einlesen
        out << this->ui->tableWidget->horizontalHeaderItem(0)->text().trimmed() + "\n";
        out << this->ui->tableWidget->horizontalHeaderItem(1)->text().trimmed() + "\n";
        out << this->ui->tableWidget->horizontalHeaderItem(2)->text().trimmed();
        // Tabelle einlesen
        for(int i=0; i < this->ui->tableWidget->rowCount(); ++i)      // an table anpassen
            {
                for(int j=0; j < this->ui->tableWidget->columnCount(); ++j)
                    {
                        out <<  "\n" + this->ui->tableWidget->item(i, j)->text().trimmed();
                    }
            }
        file.flush();
        // Datei schließen  !! Nicht Vergessen !!
        file.close();
        this->ui->statusbar->showMessage(tr("saved"), 3000);
    }
}

void mainWindow::tableOpen()
{
    // Liste aus der Datei laden
    QFile file("liste.txt");
    // Wenn öffnen der Datei erfolgreich
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        this->ui->tableWidget->horizontalHeaderItem(0)->text();
        this->ui->tableWidget->clear(); // Tabelle leeren

        while(this->ui->tableWidget->rowCount())    // leere Zeilen löschen
            {this->ui->tableWidget->removeRow(0);}

        // Lese aus Datei in Tabelle
        QTextStream in(&file);
        this->ui->tableWidget->setSortingEnabled(false);    // sortieren ausschalten
        // Header horizontal schreiben
        for(int i=0; i < 3; ++i)
        {
            QTableWidgetItem *item_0 = new QTableWidgetItem(" ");
            if(!in.atEnd()){item_0->setText(in.readLine().trimmed());}
            this->ui->tableWidget->setHorizontalHeaderItem(i, item_0);
        }

        while (!in.atEnd())
            {
                    //this->ui->tableWidget.add (in.readLine().trimmed());    // für ListWidget
                // in Tabelle schreiben
                auto id = in.readLine().trimmed();
                auto bez = in.readLine().trimmed();
                auto preis = in.readLine().trimmed();

                QTableWidgetItem *item_id = new QTableWidgetItem(" ");
                QTableWidgetItem *item_bez = new QTableWidgetItem(" ");
                QTableWidgetItem *item_preis = new QTableWidgetItem(" ");


                this->ui->tableWidget->insertRow(0);                // Zeile einfügen in Zeile 0
                item_id->setText(id);                               // item mit Text füllen
                this->ui->tableWidget->setItem(0, 0, item_id);      // item an Tabelle übergeben (zeile, spalte)
                this->ui->tableWidget->item(0,0)->setFlags(Qt::ItemIsEditable);
                item_bez->setText(bez);
                this->ui->tableWidget->setItem(0, 1, item_bez);
                item_preis->setText(preis);
                this->ui->tableWidget->setItem(0, 2, item_preis);
            }
        this->ui->tableWidget->setSortingEnabled(true); // Sortieren wieder einschalten
        // File schließen
        file.close();
        this->ui->statusbar->showMessage(tr("loaded"), 3000);
    }
}

