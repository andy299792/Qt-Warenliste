#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private slots:
    void on_add_clicked();

    void on_pushButton_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::mainWindow *ui;

    int aktiveZeile;

    void eingabeclear();
    void addItem();
    void deleteItem();
    void deleteItems();

};
#endif // MAINWINDOW_H
