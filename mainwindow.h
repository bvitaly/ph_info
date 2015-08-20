#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
//    void ph_analise_();
    void ph_analise_exiv();
    void ph_select();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    void saveSettings();
    void loadSettings();
    QString exivFilter;
   // QTableWidget *tableWidget;
};

#endif // MAINWINDOW_H
