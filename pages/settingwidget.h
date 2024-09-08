#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>

namespace Ui {
class SettingWidget;
}

// URGENT TODO: 添加新增分片机制
class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);
    ~SettingWidget();

signals:
    void settingChanged(int type);

private:
    void applyChanges();

    Ui::SettingWidget *ui;
};

#endif // SETTINGWIDGET_H
