#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "pages/loggerwidget.h"
#include "utils/loggermanager.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->viewPage);

    init();
}

void MainWindow::init()
{
    // 设置程序图标
    setWindowIcon(QIcon(":/icons/blockchain.png"));
    setWindowTitle("区块链系统动画演示");

    LoggerManager::getInstance().registerManager(ui->loggerTabWidget); // 注册日志管理器
    LoggerWidget *mainLoggerWidget = new LoggerWidget(this); // 创建主日志窗口
    LoggerManager::getInstance().addWidget(mainLoggerWidget, "系统日志"); // 添加主日志窗口
    LoggerManager::getInstance().addLog("初始化完成!");

    connect(ui->animationBtn, &QPushButton::clicked, this, &MainWindow::onAnimationBtnClicked);
    connect(ui->usageBtn, &QPushButton::clicked, this, &MainWindow::onUsageBtnClicked);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::onUsageBtnClicked);
    connect(ui->settingWidget, &SettingWidget::settingChanged, ui->viewPage, &AnimationWidget::onConfigChanged);

    ui->viewPage->init();
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
