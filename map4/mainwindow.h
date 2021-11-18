#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QString>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Change_buttom_statue(int i);
    void plot_map();
    void plot_map(int i);
    void plot_map1();
    void plot_map1(int i);
    void plot_table();
protected:
    Ui::MainWindow *ui;

signals:
    void setCenter(QVariant,QVariant,QVariant);
    void remove();
    void testd(QVariant);

private slots:
    void on_actionreset_triggered();
    void on_actionclear_triggered();
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_Tuna_filter_checkbox_clicked();
    void on_Shark_filter_checkbox_clicked();
    void on_Marlin_filter_checkbox_clicked();
    void on_Allfish_filter_checkbox_clicked();
    void on_dateEdit_dateChanged(const QDate &date);
    void on_dateEdit_2_dateChanged(const QDate &date);
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();

    void on_pushButton_4_clicked();
    void on_pushButton_8_clicked();
};
#endif // MAINWINDOW_H
