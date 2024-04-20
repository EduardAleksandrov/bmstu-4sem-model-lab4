#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "evendistribution.h"
#include "erlangdistribution.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getDataFromScreen(int&a, int&b, int&k, double&lambda, int&count_tasks, int&repeat_probability, double&step_t);
    int stepModel(EvenDistribution& generator, ErlangDistribution& processor, int countTasks, int repeatProbability, double step);
    int randf();
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
