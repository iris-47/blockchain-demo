#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "utils/loggermanager.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->viewPage);

    LoggerManager::getInstance().connectWidget(ui->loggerWidget);
    LoggerManager::getInstance().addLog("初始化完成!");

    // 按钮切换页面
    connect(ui->animationBtn, &QPushButton::clicked, this, &MainWindow::onAnimationBtnClicked);
    connect(ui->usageBtn, &QPushButton::clicked, this, &MainWindow::onUsageBtnClicked);

    // connect(ui->menuAnimation, &QAction::triggered, this, &MainWindow::onAnimationBtnClicked);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::onUsageBtnClicked);

    connect(ui->settingWidget, &SettingWidget::settingChanged, ui->viewPage, &AnimationWidget::onConfigChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onUsageBtnClicked()
{
    ui->stackedWidget->setCurrentWidget(ui->usagePage);
}

void MainWindow::onAnimationBtnClicked()
{
    ui->stackedWidget->setCurrentWidget(ui->viewPage);
}
