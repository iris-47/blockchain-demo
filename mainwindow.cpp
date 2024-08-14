#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "utils/loggermanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(0);

    LoggerManager::getInstance().connectWidget(ui->loggerWidget);
    LoggerManager::getInstance().addLog("MainWindow initial done1!");
    LoggerManager::getInstance().addLog("MainWindow initial done2!");

    // 按钮切换页面
    connect(ui->animationBtn, &QPushButton::clicked, this, &MainWindow::onAnimationBtnClicked);
    connect(ui->usageBtn, &QPushButton::clicked, this, &MainWindow::onUsageBtnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onUsageBtnClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onAnimationBtnClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
