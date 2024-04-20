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

    srand(time(NULL)); //пишем 1 раз до циклов, чтобы рассчитывалось каждый раз разное число

    try
    {
        getDataFromScreen(a,b,k,lambda,count_tasks,repeat_probability,step_t);
    } catch (const EntryException& ex)
    {
        ui->textBrowser_3->setText(ex.getMessage());
        return;
    }
    EvenDistribution generator{a,b};
    ErlangDistribution processor{k,lambda};

    int result_step = stepModel(generator, processor, count_tasks, repeat_probability, step_t);
//    qDebug() << generator.generate() << " "<< gen.generate();
    ui->textBrowser->setText(QString::number(result_step));

    result_step = eventModel(generator, processor, count_tasks, repeat_probability);
    ui->textBrowser_2->setText(QString::number(result_step));

}


int MainWindow::stepModel(EvenDistribution& generator, ErlangDistribution& processor, int countTasks, int repeatProbability, double step)
{
    int tasksDone = 0;
    double timeCurrent = step;
    double timeGenerated = generator.generate();
    double timeGeneratedPrev = 0.0;
    double timeProcessed = 0.0;

    int curQueueLen = 0;
    int maxQueueLen = 0;
    bool free = true;

    while(tasksDone < countTasks)
    {

        //генератор
        if(timeCurrent > timeGenerated)
        {
            curQueueLen += 1;
            if(curQueueLen > maxQueueLen)
            {
                maxQueueLen = curQueueLen;
            }
            timeGeneratedPrev = timeGenerated;
            timeGenerated += generator.generate();
        }
        //обработчик
        if(timeCurrent > timeProcessed)
        {
            if(curQueueLen > 0)
            {
                bool wasFree = free;
                if(free)
                {
                    free = false;
                } else {
                    tasksDone += 1;
                    curQueueLen -= 1;
                    if(randf() <= repeatProbability)
                    {
                        curQueueLen += 1;
                    }
                }
                if(wasFree)
                {
                    timeProcessed = timeGeneratedPrev + processor.generate();
                } else {
                    timeProcessed += processor.generate();
                }
            } else {
                free = true;
            }
        }
        timeCurrent += step;
    }
    return maxQueueLen;
}

int MainWindow::randf()
{
    int min = 1; // возможно ноль + шаг времени можно увеличить до 1...10 при малом диапазоне a b
    int max = 100;
    return min + rand() % (max - min + 1);
}

int MainWindow::eventModel(EvenDistribution& generator, ErlangDistribution& processor, int countTasks, int repeatProbability)
{
    int tasksDone = 0;
    int curQueueLen = 0;
    int maxQueueLen = 0;
    bool free = true;
    bool processFlag = false;
    QVector<eventStruct*> events;
    events.push_back(new eventStruct(generator.generate(),"g"));
    while(tasksDone < countTasks)
    {
        eventStruct event = *events[0];
        events.erase(events.begin());
        //генератор
        if(event.type == "g")
        {
            curQueueLen += 1;
            if(curQueueLen > maxQueueLen)
            {
                maxQueueLen = curQueueLen;
            }
            addEvent(events, eventStruct(event.data+generator.generate(), "g"));
            if(free)
            {
                processFlag = true;
            }
        //обработчик
        } else if(event.type == "p") {
            tasksDone += 1;
            if(randf() <= repeatProbability)
            {
                curQueueLen += 1;
            }
            processFlag = true;
        }
        if(processFlag)
        {
            if(curQueueLen > 0)
            {
                curQueueLen -= 1;
                addEvent(events, eventStruct(event.data+processor.generate(), "p"));
                free = false;
            } else {
                free = true;
            }
            processFlag = false;
        }
    }
    return maxQueueLen;
}

void MainWindow::addEvent(QVector<eventStruct*>& events, eventStruct event)
{
    int i = 0;
    eventStruct* ev = new eventStruct(event.data,event.type);
    while(i < events.size() && events[i]->data < event.data)
    {
        i += 1;
    }

    if(i > 0 && i < events.size())
    {
//        *events[i-1] = event;
        delete events[i-1];
        events[i-1] = nullptr;
        events[i-1] = ev;
    } else {
        events.push_back(ev);
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
        throw EntryException{"Введите диапазон a b"};
    }
    if(a == b)
    {
//        ui->textBrowser_3->setText("Введите диапазон");
        throw EntryException{"Введите диапазон a b"};
    }
    if(a > b)
    {
//        ui->textBrowser_3->setText("Введите диапазон правильно");
        throw EntryException{"Введите диапазон a b правильно"};
    }
    if(b - a < 0.01)
    {
//        ui->textBrowser_3->setText("Диапазон слишком мал");
        throw EntryException{"Диапазон a b слишком мал"};
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

void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();

    ui->textBrowser->clear();
    ui->textBrowser_2->clear();
    ui->textBrowser_3->clear();
}

