#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <cmath>
#include "entryexception.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    int a {0}, b{0}, k{0};
    double lambda {0.0};
    int count_tasks{0}, repeat_probability{0};
    double step_t {0.0};

    try
    {
        getDataFromScreen(a,b,k,lambda,count_tasks,repeat_probability,step_t);
    } catch (const EntryException& ex)
    {
        ui->textBrowser_3->setText(ex.getMessage());
        return;
    }

}
void MainWindow::getDataFromScreen(int&a, int&b, int&k, double&lambda, int&count_tasks, int&repeat_probability, double&step_t)
{
    ui->textBrowser_3->clear();
//enter a, b
    QString a_entry = ui->lineEdit->text();
    a = a_entry.toInt();

    QString b_entry = ui->lineEdit_2->text();
    b = b_entry.toInt();

    if(a_entry == "" || b_entry == "")
    {
//        ui->textBrowser_3->setText("Введите диапазон");
        throw EntryException{"Введите диапазон"};
    }
    if(a == b)
    {
//        ui->textBrowser_3->setText("Введите диапазон");
        throw EntryException{"Введите диапазон"};
    }
    if(a > b)
    {
//        ui->textBrowser_3->setText("Введите диапазон правильно");
        throw EntryException{"Введите диапазон правильно"};
    }
    if(b - a < 0.01)
    {
//        ui->textBrowser_3->setText("Диапазон слишком мал");
        throw EntryException{"Диапазон слишком мал"};
    }
//enter k, lambda
    QString k_entry = ui->lineEdit_3->text();
    QString lambda_entry = ui->lineEdit_4->text();
    k = k_entry.toInt();
    lambda = lambda_entry.toDouble();
    if(k < 1)
    {
//        ui->textBrowser_3->setText("Введите k >= 1, натуральные");
        throw EntryException{"Введите k >= 1, натуральные"};
    }
    if(lambda <= 0.0)
    {
//        ui->textBrowser_3->setText("Введите lambda > 0");
        throw EntryException{"Введите lambda > 0"};
    }
// enter options
    QString ct_entry = ui->lineEdit_5->text();
    count_tasks = ct_entry.toInt();
    if(count_tasks <= 0 )
    {
//        ui->textBrowser_3->setText("Неверно задано количество заявок");
        throw EntryException{"Неверно задано количество заявок"};
    }
    QString rp_entry = ui->lineEdit_6->text();
    repeat_probability = rp_entry.toInt();
    if(repeat_probability < 0 || repeat_probability > 100)
    {
//        ui->textBrowser_3->setText("Неверно задана вероятность возврата заявки");
        throw EntryException{"Неверно задана вероятность возврата заявки"};
    }
    QString step_entry = ui->lineEdit_7->text();
    step_t = step_entry.toDouble();
    if(step_t <= 0.0)
    {
//        ui->textBrowser_3->setText("Неверно задан шаг времени");
        throw EntryException{"Неверно задан шаг времени"};
    }
}
