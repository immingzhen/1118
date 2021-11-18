#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QQuickItem>
#include<QtSql/QtSql>
#include<QtSql/QSqlDatabase>
#include<QMessageBox>
#include<QtSql/QSqlQuery>
#include<QDebug>
#include <QSqlQueryModel>
#include <iostream>
#include <QVector>
#include <QVector2D>
#include <QString>
#include <QCompleter>
#include <QtCharts>
#include <QDate>
#include <QTextCodec>
#include <QtCore>


class filter_main_fun{
    public:
    virtual QVector<QStringList> filter_tuna(QVector<QStringList> olddata,QString Filter_target)=0;
    virtual QStringList filter_id(QVector<QStringList> data);
    virtual QVector<QStringList> filter_certain_id(QVector<QStringList> data,QString Id);
};


class data_main_fun{
    public:
    virtual void first_setting()=0;
    virtual QVector<QStringList> get_data()=0;
    virtual bool statue()=0;
    virtual void data_split(QVector<QStringList> input)=0;
};



class database_getdata:public data_main_fun
{
private:
    std::string connect_sql;

    bool connect_statue=false;
    QStringList dataarray={};
    const int datarow = 6 ;

    void set_connect_sql()
    {
        connect_sql = "Driver={SQL Server};";
        connect_sql.append("Server=140.116.85.227,57964;");
        connect_sql.append("Database=NCKUfish2;");
        connect_sql.append("Uid=root;");
        connect_sql.append("Pwd=123456;");
    }

public:
    QSqlDatabase db;
    void first_setting()
    {
        set_connect_sql();
        db =QSqlDatabase::addDatabase("QODBC");

        db.setDatabaseName(QString::fromStdString(connect_sql));
        if (db.open()){
            connect_statue = true;
        }else{
            connect_statue = false;
        }
    }

    QVector<QStringList> get_data()
    {
        QVector<QStringList> array1;
        QSqlTableModel model1;
        QStringList fonts={"","","","","","", ""};
        //               date,ID,fis,len,RF-ID,latitude,longtituude
        model1.setTable("fishdata2");
        model1.select();
        for (int ii=0;ii<model1.rowCount();ii++){
        for (int i=0;i<model1.columnCount();i++)
        {
           fonts[i] = model1.data(model1.index(ii,i)).toString();
        }
           array1.append(fonts);
        }
        return array1;
    }

    bool statue(){
    return connect_statue;
    }

    void data_split(QVector<QStringList> input){

        for (int i=0;i<input.size();i++){
        //qDebug() << input[i]<< endl;
        }
    }

};







class filter_data{
    public:
    void initial_filter(database_getdata ddb,QStringList Data_statue){
        for (int i=0;i<Data_statue.length();i++){
            if(Data_statue[i]=="1"){
                QVector<QStringList> ddata = filter_tuna(ddb.get_data(),Data_target[i]);
                ffilter_data->append(ddata);
            }
        }
        Id_array = filter_id(*ffilter_data);
    }

    void initial_filter2(database_getdata ddb,QStringList Data_statue,QString comboBox_currentText){
        QVector<QStringList> *dfilter_data= new QVector<QStringList>;
        for (int i=0;i<Data_statue.length();i++){
            if(Data_statue[i]=="1"){
                QVector<QStringList> ddata = filter_tuna(ddb.get_data(),Data_target[i]);
                dfilter_data->append(ddata);
            }
        }
        Id_array = filter_id(*dfilter_data);

        if (comboBox_currentText != "Not_Limit"){
            ffilter_data->append(filter_certain_id(*dfilter_data,comboBox_currentText));
        //qDebug() <<"hello"<<f_data << endl;
        }else{
            ffilter_data->append(*dfilter_data);

        }
        delete dfilter_data;
    }

    QVector<QStringList> filter_tuna(QVector<QStringList> olddata,QString Filter_target)
    {
        QVector<QStringList> array;
        for (int i=0 ; i<olddata.size();i++){
            if (_get_statue(olddata[i][2],Filter_target))
            {
                array.append(olddata[i]);
            }
        }
        return array;
    }

    QVector<QStringList> filter_date(QVector<QStringList> old,QDate start,QDate end)
    {
        QVector<QStringList> d_data;
        QString starta = start.toString("yyyy-MM-dd");
        starta +=" 00:00:00";
        QString endb = end.toString("yyyy-MM-dd");
        endb +=" 00:00:00";
        QDateTime sdate=QDateTime::fromString(starta,"yyyy-MM-dd hh:mm:ss");
        QDateTime edate=QDateTime::fromString(endb,"yyyy-MM-dd hh:mm:ss");
        uint stime = sdate.toTime_t();
        uint etime = edate.toTime_t();
        for (int i=0 ; i<old.size();i++){
            QDateTime datenew = QDateTime::fromString(old[i][0],"yyyy-MM-dd hh:mm:ss");
            uint Ttime = datenew.toTime_t();
            if (Ttime > stime && Ttime < etime)
            {
                d_data.append(old[i]);
            }

            /////////////////////////////////////////////////////////////////////////////////////////

            for (int i=0 ; i<d_data.size();i++){
                for (int j=0 ; j<d_data.size();j++){
                QStringList a1_date=d_data[i][0].split('-');
                QStringList b1_date=a1_date[2].split(' ');
                QStringList a2_date=d_data[j][0].split('-');
                QStringList b2_date=a2_date[2].split(' ');
                if(atoi(a1_date[0].toStdString().c_str()) > atoi(a2_date[0].toStdString().c_str())){
                    QStringList h=d_data[i];
                    d_data[i]=d_data[j];
                    d_data[j]=h;
                }
                if(atoi(a1_date[0].toStdString().c_str()) == atoi(a2_date[0].toStdString().c_str()) && atoi(a1_date[1].toStdString().c_str()) > atoi(a2_date[1].toStdString().c_str())){
                    QStringList h=d_data[i];
                    d_data[i]=d_data[j];
                    d_data[j]=h;
                }
                if(atoi(a1_date[0].toStdString().c_str()) == atoi(a2_date[0].toStdString().c_str()) && atoi(a1_date[1].toStdString().c_str()) == atoi(a2_date[1].toStdString().c_str()) && atoi(b1_date[0].toStdString().c_str()) == atoi(b2_date[0].toStdString().c_str())){
                    QStringList h=d_data[i];
                    d_data[i]=d_data[j];
                    d_data[j]=h;
                }

                }
            }
        }
        return d_data;
    }

    QStringList filter_id(QVector<QStringList> data)
    {
        QStringList array;
        for (int i=0 ; i<data.size();i++){
            bool statue = _Search(array , data[i][1]);
            if(statue){
                array.append(data[i][1]);
            }
        }
        if(array.length()>1){
            array.append("Not_Limit");
        }
        return array;
    }

    QVector<QStringList> filter_certain_id(QVector<QStringList> data,QString Id)
    {
        QVector<QStringList> f_data;
        for (int i=0 ; i<data.size();i++){
            if (data[i][1]==Id){
                f_data.append(data[i]);
            }
        }

        return f_data;
    }

    QVector<int> sort_data(QVector<QStringList> oldd)
           {
           QVector<int> sort_tuna(39);


            for (int i=0 ; i<oldd.size();i++){
                for(int j=0;j<12;j++){
                    QStringList a1_date=oldd[i][0].split('-');

                    if(atoi(a1_date[1].toStdString().c_str())==(j+1)){
                        if (oldd[i][2]=="tuna" || oldd[i][2]=="tuna01" || oldd[i][2]=="tuna02"){

                            sort_tuna[j]+=1;
                            sort_tuna[12]+=1;
                        }
                        if (oldd[i][2]=="shark"){
                            sort_tuna[j+13]+=1;
                            sort_tuna[25]+=1;
                        }
                        if (oldd[i][2]=="marlin"){
                            sort_tuna[j+26]+=1;
                            sort_tuna[38]+=1;
                            //qDebug()<<"3"<<endl;
                        }

                    }
                }
           }

            return sort_tuna;
           }

    QVector<QStringList> get_ffilter_data(){
        return *ffilter_data;
    }

    QStringList get_Id_array(){
        return Id_array;
    }
    private:

    QStringList Data_target={"tuna","shark","marlin"};

    QVector<QStringList> *ffilter_data= new QVector<QStringList>;


    QStringList Id_array;

    bool _get_statue(QString old,QString target){
        if (target == "tuna"){
            if (old == target || old == "tuna01" || old == "tuna02"){
               return true;
            }else{
               return false;
            }
        }

        if(target == "marlin"){
            if (old == target){
               return true;
            }else{
               return false;
            }
        }

        if(target == "shark"){
            if (old == target){
               return true;
            }else{
               return false;
            }


        }


        return false;

    }

    bool _Search(QStringList Name_list,QString Target){
        bool statue =true;
        for(int i=0 ; i<Name_list.length();i++){
            if(Name_list[i]==Target){
                statue =false;
                break;
            }
        }
        return statue;
    }
};




class plot_statistic{
public:
    void Plot1(filter_data *temp,QDate start,QDate end){
        QVector<QStringList> ffilter_data = temp->get_ffilter_data();
        d_data->append(filter_date(ffilter_data,start,end));
    }

    void Plot2(QChart *chart){
        sort_data(*d_data);
        chart->addSeries(series);
        chart->setTitle("Total Fish Amount in Every Month");
        QFont font;
        font.setPixelSize(18);
        chart->setTitleFont(font);
        chart->setAnimationOptions(QChart::SeriesAnimations);

        QStringList categories;
        categories <<"Jan"<<"Feb"<<"Mar"<<"Apr"<<"May"<<"Jun"<<"Jul"<<"Aug"<<"Sep"<<"Oct"<<"Nov"<<"Dec";
        QBarCategoryAxis *axis = new QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);
        QValueAxis *axisy =new QValueAxis();
        axisy->setRange(0,*fish_max);
        axisy->setTickCount(*fish_max+1);
        axisy->setLabelFormat("%d");
        chart->setAxisY(axisy,series);
    }

    QVector<int> get_the_fish_sum(){
        return *sort_fish_sum;
    }

    QVector<QStringList> get_the_d_data(){
        return *d_data;
    }
private:

    QVector<QStringList> filter_date(QVector<QStringList> old,QDate start,QDate end)
    {
        QVector<QStringList> d_data;
        QString starta = start.toString("yyyy-MM-dd");
        starta +=" 00:00:00";
        QString endb = end.toString("yyyy-MM-dd");
        endb +=" 00:00:00";
        QDateTime sdate=QDateTime::fromString(starta,"yyyy-MM-dd hh:mm:ss");
        QDateTime edate=QDateTime::fromString(endb,"yyyy-MM-dd hh:mm:ss");
        uint stime = sdate.toTime_t();
        uint etime = edate.toTime_t();
        for (int i=0 ; i<old.size();i++){
            QDateTime datenew = QDateTime::fromString(old[i][0],"yyyy-MM-dd hh:mm:ss");
            uint Ttime = datenew.toTime_t();
            //qDebug() << datenew<< Ttime <<edate<<stime<<endb<<starta<< Qt::endl;
            if (Ttime > stime && Ttime < etime)
            {
                d_data.append(old[i]);
            }

            /////////////////////////////////////////////////////////////////////////////////////////

            for (int i=0 ; i<d_data.size();i++){
                for (int j=0 ; j<d_data.size();j++){
                QStringList a1_date=d_data[i][0].split('-');
                QStringList b1_date=a1_date[2].split(' ');
                QStringList a2_date=d_data[j][0].split('-');
                QStringList b2_date=a2_date[2].split(' ');
                if(atoi(a1_date[0].toStdString().c_str()) > atoi(a2_date[0].toStdString().c_str())){
                    QStringList h=d_data[i];
                    d_data[i]=d_data[j];
                    d_data[j]=h;
                }
                if(atoi(a1_date[0].toStdString().c_str()) == atoi(a2_date[0].toStdString().c_str()) && atoi(a1_date[1].toStdString().c_str()) > atoi(a2_date[1].toStdString().c_str())){
                    QStringList h=d_data[i];
                    d_data[i]=d_data[j];
                    d_data[j]=h;
                }
                if(atoi(a1_date[0].toStdString().c_str()) == atoi(a2_date[0].toStdString().c_str()) && atoi(a1_date[1].toStdString().c_str()) == atoi(a2_date[1].toStdString().c_str()) && atoi(b1_date[0].toStdString().c_str()) == atoi(b2_date[0].toStdString().c_str())){
                    QStringList h=d_data[i];
                    d_data[i]=d_data[j];
                    d_data[j]=h;
                }

                }
            }
        }
        return d_data;
    }

    void sort_data(QVector<QStringList> oldd)
           {
           QBarSet *set_tuna = new QBarSet("Tuna");
           QBarSet *set_shark = new QBarSet("Shark");
           QBarSet *set_marlin = new QBarSet("Marlin");
           QVector<int> sort_tuna(13);
           QVector<int> sort_shark(13);
           QVector<int> sort_marlin(13);

            for (int i=0 ; i<oldd.size();i++){
                for(int j=0;j<12;j++){
                    QStringList a1_date=oldd[i][0].split('-');
                    if(atoi(a1_date[1].toStdString().c_str())==(j+1)){
                        if (oldd[i][2]=="tuna"){

                            sort_tuna[j]+=1;
                            sort_tuna[12]+=1;
                        }
                        if (oldd[i][2]=="shark"){
                            sort_shark[j]+=1;
                            sort_shark[12]+=1;
                        }
                        if (oldd[i][2]=="marlin"){
                            sort_marlin[j]+=1;
                            sort_marlin[12]+=1;
                        }

                    }
                }
           }
           *fish_max=0;
           for (int i=0;i<sort_tuna.size()-1;i++){
               if (sort_tuna[i]>*fish_max){*fish_max=sort_tuna[i];}
               if (sort_shark[i]>*fish_max){*fish_max=sort_shark[i];}
               if (sort_marlin[i]>*fish_max){*fish_max=sort_marlin[i];}
               set_tuna->append(sort_tuna[i]);
               set_shark->append(sort_shark[i]);
               set_marlin->append(sort_marlin[i]);
           }
            series->append(set_tuna);
            series->append(set_shark);
            series->append(set_marlin);
            sort_fish_sum->append(sort_tuna[12]);
            sort_fish_sum->append(sort_shark[12]);
            sort_fish_sum->append(sort_marlin[12]);
           }

    QVector<QStringList> *d_data= new QVector<QStringList>;

    QStringList Data_target={"tuna","shark","marlin"};

    QVector<int> *sort_fish_to_plot= new QVector<int>;

    QBarSeries *series = new QBarSeries();

    QVector<int> *sort_fish_sum= new QVector<int>;


    int *fish_max=new int;

};


class plot_sum{
public:
    void Plot1(plot_statistic *temp){
        QBarSet *set = new QBarSet("Number");
        QVector<int> fish_sum =temp->get_the_fish_sum();
        *fish_max=0;
        for (int i=0;i<fish_sum.size();i++){
            if(fish_sum[i]>=*fish_max){*fish_max=fish_sum[i];}
            set->append(fish_sum[i]);
        }
        series->append(set);
    }

    void Plot2(QChart *chart){
        chart->addSeries(series);
        chart->setTitle("Fish Amounts");
        QFont font;
        font.setPixelSize(18);
        chart->setTitleFont(font);
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->legend()->setVisible(false);
        QStringList categories;
        categories <<"Tuna"<<"Shark"<<"Marlin";
        QBarCategoryAxis *axis = new QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);
        QValueAxis *axisy =new QValueAxis();
        axisy->setRange(0,*fish_max);
        axisy->setTickCount(*fish_max+1);
        axisy->setLabelFormat("%d");
        chart->setAxisY(axisy,series);
    }
private:
    QBarSeries *series = new QBarSeries();

    int *fish_max=new int;

};

class plot_table{

};



////////////////////////////////////start here/////////////////////////////////////////////////////////////////////////////////////////


database_getdata *ddb = new database_getdata();
QCompleter *completer = new QCompleter();
QChartView *chartaa;
QChartView *chartbb;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //ui->dateEdit->setDate(QDate::currentDate());
    //ui->dateEdit->setMinimumDate(QDate::currentDate().addDays(-3650));
    //ui->dateEdit->setMaximumDate(QDate::currentDate().addDays(365));
    //ui->dateEdit->setDisplayFormat("yyyy.MM.dd");

    //hello *teee = new hello();
    ddb->first_setting();
    QVector<QStringList> ttemp =ddb->get_data();
    //qDebug() << ttemp<< endl;
    if (ddb->statue()){

        //QMessageBox::information(this,"connect","connect");
        ui->setupUi(this);
        ui->quickWidget->setSource(QUrl(QString("qrc:/map.qml")));
        ui->quickWidget->show();
        //ui->widget_2->setVisible(false);





        ui->dateEdit->setDate(QDate::currentDate());
        ui->dateEdit->setMinimumDate(QDate::currentDate().addDays(-3650));
        ui->dateEdit->setMaximumDate(QDate::currentDate().addDays(3650));
        ui->dateEdit->setDisplayFormat("yyyy.MM.dd");

        ui->dateEdit_2->setDate(QDate::currentDate());
        ui->dateEdit_2->setMinimumDate(QDate::currentDate().addDays(-3650));
        ui->dateEdit_2->setMaximumDate(QDate::currentDate().addDays(3650));
        ui->dateEdit_2->setDisplayFormat("yyyy.MM.dd");

    }else{
        QMessageBox::information(this,"unconnect","unconnect");

    }
    //ui->comboBox->addItem("123");
    //ui->comboBox->addItem("124");

    QStringList wordList;
    wordList << "alpha" << "omega" << "omicron" << "zeta";
    QStringList wwordList;


    //QCompleter *completer = new QCompleter(wordList, this);
    completer->setModel(new QStringListModel( wordList, completer ) );
    completer->setCaseSensitivity(Qt::CaseInsensitive);

    //QLineEdit *lineedit =new QLineEdit(this);
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox->setCompleter(completer);
    ui->comboBox->setItemDelegate(new QItemDelegate);
    ui->comboBox->addItems(wordList);
    ui->comboBox->clear();
    completer->setModel(new QStringListModel( wwordList, completer ) );
    ui->comboBox->addItems(wwordList);


    //ui->comboBox->addItem("1230");
    ui->pushButton_7->setVisible(false);
    auto obj = ui->quickWidget->rootObject();
    connect(this,SIGNAL(setCenter(QVariant,QVariant,QVariant)),obj,SLOT(setCenter(QVariant,QVariant,QVariant)));
    connect(this,SIGNAL(remove()),obj,SLOT(remove()));
    connect(this,SIGNAL(testd(QVariant )),obj,SLOT(testd(QVariant)));

    ui->dateEdit->calendarWidget()->setLocale(QLocale::English);
    ui->dateEdit_2->calendarWidget()->setLocale(QLocale::English);
    //teee->test1();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionreset_triggered()
{
    emit setCenter(22.9989,120.2169,0);
}


void MainWindow::on_actionclear_triggered()//clear()
{
    emit remove();
}


void MainWindow::on_pushButton_clicked()
{
       emit setCenter(24,121,6);

}

void MainWindow::on_pushButton_3_clicked()
{
       emit setCenter(20,160,3);

}






void MainWindow::on_pushButton_2_clicked()
{
   emit remove();

}

void MainWindow::plot_map1(){
    delete chartaa;
    delete chartbb;
    //chartaa->removeAllSeries();
    ui->ChartFrame->setVisible(false);
    ui->ChartFrame2->setVisible(false);
    emit remove();
    filter_data *f1=new filter_data;
    QStringList Data_statue={QString::number(int(Qt::Checked ==ui->Tuna_filter_checkbox->checkState())),
                             QString::number(int(Qt::Checked ==ui->Shark_filter_checkbox->checkState())),
                             QString::number(int(Qt::Checked ==ui->Marlin_filter_checkbox->checkState()))};
    f1->initial_filter(*ddb,Data_statue);
    plot_statistic *f2 =new plot_statistic;

    f2->Plot1(f1,ui->dateEdit->date(),ui->dateEdit_2->date());
    QChart *chart = new QChart();
    chart->removeAllSeries();
    f2->Plot2(chart);
    plot_sum *f3 =new plot_sum;
    f3->Plot1(f2);
    QChart *chart2 = new QChart();
    chart2->removeAllSeries();
    f3->Plot2(chart2);
////////////////////////////////////////////////////////////////
    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->ChartFrame);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(721,550);
    chartView->update();
////////////////////////////////////////////////////////////////
    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setParent(ui->ChartFrame2);
    chartView2->setRenderHint(QPainter::Antialiasing);
    chartView2->resize(721,550);
    chartView2->update();
    chartaa=chartView;
    chartbb=chartView2;
    ////////////////////////////////////////////////////////////
    QStringList Id_array = f1->get_Id_array();

    QVector<QStringList> d_data=f2->get_the_d_data();
////////////////////////////////////////////////////////////
    ui->twTable->setRowCount(0);

    for(int yy = 0; yy < d_data.size(); ++yy)
    {
        ui->twTable->insertRow(yy) ;
        ui->twTable->setRowCount(d_data.size());
        ui->twTable->setColumnCount((d_data)[yy].size());
       for (int yyy = 0; yyy<(d_data)[yy].size(); ++yyy)
        {
           QTableWidgetItem* tablew = new QTableWidgetItem( (d_data)[yy][yyy]);
           tablew ->setTextAlignment(Qt::AlignCenter);
           ui->twTable->setItem( yy , yyy , tablew  );
           //ui->twTable->setItem( yy , yyy , new QTableWidgetItem( (d_data)[yy][yyy] ) );
        }

    }
    ui->twTable->resizeColumnsToContents();
//////////////////////////////////////////////////////////////
    completer->setModel(new QStringListModel( Id_array, completer ) );
    ui->comboBox->clear();
    ui->comboBox->addItems(Id_array);
    if(Id_array.length()>1){
        ui->comboBox->setCurrentIndex(Id_array.length()-1);
    }
    for (int i=0;i<d_data.length();i++){
        emit testd((d_data)[i]);
    }
    delete f1;
    delete f2;
    delete f3;

     ui->ChartFrame->setVisible(true);
     ui->ChartFrame2->setVisible(true);
}

void MainWindow::plot_map1(int i){
    delete chartaa;
    delete chartbb;
    ui->ChartFrame->setVisible(false);
    ui->ChartFrame2->setVisible(false);
    emit remove();
    filter_data *f1=new filter_data;
    QStringList Data_statue={QString::number(int(Qt::Checked ==ui->Tuna_filter_checkbox->checkState())),
                             QString::number(int(Qt::Checked ==ui->Shark_filter_checkbox->checkState())),
                             QString::number(int(Qt::Checked ==ui->Marlin_filter_checkbox->checkState()))};
    f1->initial_filter2(*ddb,Data_statue,ui->comboBox->currentText());
    plot_statistic *f2 =new plot_statistic;

    f2->Plot1(f1,ui->dateEdit->date(),ui->dateEdit_2->date());
    QChart *chart = new QChart();
    chart->removeAllSeries();
    f2->Plot2(chart);
    plot_sum *f3 =new plot_sum;
    f3->Plot1(f2);
    QChart *chart2 = new QChart();
    chart2->removeAllSeries();
    f3->Plot2(chart2);
////////////////////////////////////////////////////////////////
    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->ChartFrame);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(721,550);
    chartView->update();
////////////////////////////////////////////////////////////////
    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setParent(ui->ChartFrame2);
    chartView2->setRenderHint(QPainter::Antialiasing);
    chartView2->resize(721,550);
    chartView2->update();

    ////////////////////////////////////////////////////////////
    QStringList Id_array = f1->get_Id_array();

    QVector<QStringList> d_data=f2->get_the_d_data();
////////////////////////////////////////////////////////////
    ui->twTable->setRowCount(0);

    for(int yy = 0; yy < d_data.size(); ++yy)
    {
        ui->twTable->insertRow(yy) ;
        ui->twTable->setRowCount(d_data.size());
        ui->twTable->setColumnCount((d_data)[yy].size());
       for (int yyy = 0; yyy<(d_data)[yy].size(); ++yyy)
        {
           QTableWidgetItem* tablew = new QTableWidgetItem( (d_data)[yy][yyy]);
           tablew ->setTextAlignment(Qt::AlignCenter);
           ui->twTable->setItem( yy , yyy , tablew  );
        }

    }
    ui->twTable->resizeColumnsToContents();
//////////////////////////////////////////////////////////////
    for (int i=0;i<d_data.length();i++){
        emit testd((d_data)[i]);
    }
    chartaa=chartView;
    chartbb=chartView2;
    delete f1;
    delete f2;
    delete f3;

     ui->ChartFrame->setVisible(true);
     ui->ChartFrame2->setVisible(true);
}

void MainWindow::plot_table(){}

void MainWindow::plot_map(){
    emit remove();
    filter_data *ss=new filter_data;
    QVector<QStringList> *ffilter_data= new QVector<QStringList>;
    QVector<QStringList> data =ddb->get_data();
    QStringList Data_statue={QString::number(int(Qt::Checked ==ui->Tuna_filter_checkbox->checkState())),
                             QString::number(int(Qt::Checked ==ui->Shark_filter_checkbox->checkState())),
                             QString::number(int(Qt::Checked ==ui->Marlin_filter_checkbox->checkState()))};
    QStringList Data_target={"tuna","shark","marlin"};
    for (int i=0;i<Data_statue.length();i++){
        if(Data_statue[i]=="1"){
            QVector<QStringList> ddata = ss->filter_tuna(data,Data_target[i]);
            ffilter_data->append(ddata);
        }
    }
    //qDebug() <<*ffilter_data<<endl;
    QStringList Id_array=ss->filter_id(*ffilter_data);
    //qDebug() <<Id_array<<endl;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// Qchart bar (for every month per year)                                                  ////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    QVector<QStringList> *d_data= new QVector<QStringList>;//

    d_data->append(ss->filter_date(*ffilter_data,ui->dateEdit->date(),ui->dateEdit_2->date()));

    QVector<int> *sort_tuna= new QVector<int>;

    sort_tuna->append(ss->sort_data(*d_data));

    QBarSet *set0 = new QBarSet("Tuna");
    QBarSet *set1 = new QBarSet("Shark");
    QBarSet *set2 = new QBarSet("Marlin");


    //Tuna
    *set0 << (*sort_tuna)[0]  << (*sort_tuna)[1]  << (*sort_tuna)[2]  << (*sort_tuna)[3]  << (*sort_tuna)[4]  << (*sort_tuna)[5]  << (*sort_tuna)[6] << (*sort_tuna)[7] << (*sort_tuna)[8] << (*sort_tuna)[9] << (*sort_tuna)[10] << (*sort_tuna)[11];
    //Shark
    *set1 << (*sort_tuna)[13] << (*sort_tuna)[14] << (*sort_tuna)[15] << (*sort_tuna)[16] << (*sort_tuna)[17] << (*sort_tuna)[18] << (*sort_tuna)[19] << (*sort_tuna)[20] << (*sort_tuna)[21] << (*sort_tuna)[22] << (*sort_tuna)[23] << (*sort_tuna)[24] ;
    //Marlin
    *set2 << (*sort_tuna)[26] << (*sort_tuna)[27] << (*sort_tuna)[28] << (*sort_tuna)[29] << (*sort_tuna)[30] << (*sort_tuna)[31] << (*sort_tuna)[32] << (*sort_tuna)[33] << (*sort_tuna)[34] << (*sort_tuna)[35] << (*sort_tuna)[36]<< (*sort_tuna)[37];


    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Total Fish Amount in Every Month");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories <<"Jan"<<"Feb"<<"Mar"<<"Apr"<<"May"<<"Jun"<<"Jul"<<"Aug"<<"Sep"<<"Oct"<<"Nov"<<"Dec";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    //chart->setPlotArea(QRectF(10,10,711,481));


    QChartView *chartView = new QChartView(chart);
    chartView->setParent(ui->ChartFrame);


    //chartView->setParent(ui->tab_chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(721,481);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////// total fish chart /////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    QBarSet *set21 = new QBarSet("Number");
    *set21 << (*sort_tuna)[12] << (*sort_tuna)[25] <<(*sort_tuna)[38];

    QBarSeries *series2 = new QBarSeries();
    series2->append(set21);

    QChart *chart2 = new QChart();
    chart2->addSeries(series2);
    chart2->setTitle("Fish Amounts");
    chart2->setAnimationOptions(QChart::SeriesAnimations);
    chart2->legend()->setVisible(false);

    QStringList categories2;
    categories2 <<"Tuna"<<"Shark"<<"Marlin";
    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories2);
    chart2->createDefaultAxes();
    chart2->setAxisX(axis2, series2);

    QChartView *chartView2 = new QChartView(chart2);
    //chartView2->setChart(chart2);
    chartView2->setParent(ui->ChartFrame2);
    chartView2->setRenderHint(QPainter::Antialiasing);
    chartView2->resize(721,481);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Table Set///////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////
    ui->twTable->setRowCount(0);
    int columnnumber = d_data->size();
    //ui->twTable->setColumnCount(columnnumber);


    for(int yy = 0; yy < columnnumber; ++yy)
    {
        ui->twTable->insertRow(yy) ;
        QStringList bbb =(*d_data)[yy];
        int rownumber = bbb.size();
        ui->twTable->setRowCount(columnnumber);
        ui->twTable->setColumnCount(rownumber);
       for (int yyy = 0; yyy<rownumber; ++yyy)
        {
           QTableWidgetItem* tableww = new QTableWidgetItem( bbb[yyy] );
           tableww ->setTextAlignment(Qt::AlignCenter);
           ui->twTable->setItem( yy , yyy , tableww  );


        }

    }
    ui->twTable->resizeColumnsToContents();
////////////////////////////////////////////////////////////////////////////////////////////////////////

    completer->setModel(new QStringListModel( Id_array, completer ) );
    ui->comboBox->clear();
    ui->comboBox->addItems(Id_array);
    if(Id_array.length()>1){
        ui->comboBox->setCurrentIndex(Id_array.length()-1);
    }
    for (int i=0;i<d_data->length();i++){
        emit testd((*d_data)[i]);
    }

//delete chart;
    delete ffilter_data;
    delete d_data;
    delete ss;

}


void MainWindow::plot_map(int i){
    emit remove();
    filter_data *ss=new filter_data;
    QVector<QStringList> *ffilter_data= new QVector<QStringList>;
    QVector<QStringList> data =ddb->get_data();
    QStringList Data_statue={QString::number(int(Qt::Checked ==ui->Tuna_filter_checkbox->checkState())),
                             QString::number(int(Qt::Checked ==ui->Shark_filter_checkbox->checkState())),
                             QString::number(int(Qt::Checked ==ui->Marlin_filter_checkbox->checkState()))};
    QStringList Data_target={"tuna","shark","marlin"};
    for (int i=0;i<Data_statue.length();i++){
        if(Data_statue[i]=="1"){
            QVector<QStringList> ddata = ss->filter_tuna(data,Data_target[i]);
            ffilter_data->append(ddata);
        }
    }
    QVector<QStringList> *f_data=new QVector<QStringList>;
    if (ui->comboBox->currentText() != "Not_Limit"){
        f_data->append(ss->filter_certain_id(*ffilter_data,ui->comboBox->currentText()));
    //qDebug() <<"hello"<<f_data << endl;
    }else{
        f_data->append(*ffilter_data);

    }
    //qDebug() <<"www"<<f_data->length()<< endl;

    QVector<QStringList> *d_data= new QVector<QStringList>;//
    QDate date_a= ui->dateEdit->date();

    QDate date_b= ui->dateEdit_2->date();

    d_data->append(ss->filter_date(*ffilter_data,ui->dateEdit->date(),ui->dateEdit_2->date()));
    //qDebug() <<date_a.toString("yyyy-MM-dd")<<date_b.toString("yyyy-MM-dd")<< endl;

    QVector<int> *sort_tuna= new QVector<int>;
         //qDebug()<<*d_data<<"mingzhennnnnnnnnbbb11"<<endl;
         sort_tuna->append(ss->sort_data(*d_data));
         //qDebug()<<*sort_tuna<<"mingzhennnnnnnnnbffffffffffffffffffffffffffffffffffffffffffffffbb"<<endl;


    for(int i=0;i<d_data->length();i++){
         //qDebug() <<"www"<<i<< endl;
        emit testd((*d_data)[i]);
    }
    delete ss;
    delete f_data;
    delete d_data;
    delete ffilter_data;
}


void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() <<ui->comboBox->currentText() << endl;
    qDebug() <<QString::number(int(Qt::Checked ==ui->Tuna_filter_checkbox->checkState())) <<endl;
    plot_map1(0);
}


void MainWindow::Change_buttom_statue(int i){
    ui->Allfish_filter_checkbox->setChecked(i);
//ui->radioButton->setChecked(i);
    ui->Marlin_filter_checkbox->setChecked(i);
    ui->Shark_filter_checkbox->setChecked(i);
    ui->Tuna_filter_checkbox->setChecked(i);
}


void MainWindow::on_Tuna_filter_checkbox_clicked()
{
    plot_map1();
    ui->ChartFrame->update();
    ui->ChartFrame2->update();
    if (Qt::Checked == ui->Tuna_filter_checkbox->checkState()){
        //Change_buttom_statue();
        ui->Tuna_filter_checkbox->setChecked(1);
    }else{
        //Change_buttom_statue();
        //ui->Allfish_filter_checkbox->setChecked(1);
    }

}


void MainWindow::on_Shark_filter_checkbox_clicked()
{
    plot_map1();
    if (Qt::Checked == ui->Shark_filter_checkbox->checkState()){
        //Change_buttom_statue();
        ui->Shark_filter_checkbox->setChecked(1);
    }else{
        //Change_buttom_statue();
        //ui->Allfish_filter_checkbox->setChecked(1);
    }

}


void MainWindow::on_Marlin_filter_checkbox_clicked()
{
    plot_map1();
    if (Qt::Checked == ui->Marlin_filter_checkbox->checkState()){
        //Change_buttom_statue();
        ui->Marlin_filter_checkbox->setChecked(1);
    }else{
        //Change_buttom_statue();
        //ui->Allfish_filter_checkbox->setChecked(1);
    }

}





void MainWindow::on_Allfish_filter_checkbox_clicked()
{
    if (Qt::Checked == ui->Allfish_filter_checkbox->checkState()){
        Change_buttom_statue(1);
    }else{
        Change_buttom_statue(0);
        //Change_buttom_statue();
        //ui->Allfish_filter_checkbox->setChecked(1);
    }
    plot_map1();
}




void MainWindow::on_dateEdit_dateChanged(const QDate &date)
{
    QString str;
    str = date.toString("dd.MM.yyyy");
    str = str.trimmed();
    QDate bb = QDate::fromString(str,"dd.MM.yyyy");

    QDate enddd = ui->dateEdit_2->date();
    //ui->label_3->setText(str);

    if (enddd>bb)
    {
     ui->dateEdit_2->setMinimumDate(bb);
     plot_map1();
    }
    else
    {
    ui->dateEdit_2->setDate(bb);
    ui->dateEdit_2->setMinimumDate(bb);
    plot_map1();
    //ui->label_3->setText(str);
    }
}




void MainWindow::on_dateEdit_2_dateChanged(const QDate &date)
{
    QString str2;
    str2 = date.toString("dd.MM.yyyy");
    plot_map1();
}


void MainWindow::on_pushButton_5_clicked()
{
    QDate date_a= ui->dateEdit->date();
    //changed
    ui->dateEdit_2->setDate(date_a);
    plot_map1();
}


void MainWindow::on_pushButton_6_clicked()
{
    ui->pushButton_6->setVisible(false);
    ui->pushButton_7->setVisible(true);
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_7_clicked()
{
    ui->pushButton_7->setVisible(false);
    ui->pushButton_6->setVisible(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_4_clicked()
{
    emit setCenter(20,-25,3);
}


void MainWindow::on_pushButton_8_clicked()
{
    emit setCenter(-20,80,3);
}

