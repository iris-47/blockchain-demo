#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QObject>
#include <QWidget>

class SettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingWidget(QWidget *parent = nullptr);
};

#endif // SETTINGWIDGET_H
