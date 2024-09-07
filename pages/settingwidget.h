#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>

namespace Ui {
class SettingWidget;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);
    ~SettingWidget();

signals:
    void settingChanged();

private:
    void applyChanges();

    Ui::SettingWidget *ui;
};

#endif // SETTINGWIDGET_H
