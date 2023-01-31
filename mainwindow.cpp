#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <calendar/calendarview.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //calendar widget
    connect(ui->toolButton, &QToolButton::clicked, this, [=]() {
        CalendarView *calendar = new CalendarView;
        calendar->PopUp(ui->toolButton);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

