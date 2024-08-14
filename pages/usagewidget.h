#ifndef USAGEWIDGET_H
#define USAGEWIDGET_H

#include <QWidget>

namespace Ui {
class UsageWidget;
}

class UsageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UsageWidget(QWidget *parent = nullptr);
    ~UsageWidget();

private:
    Ui::UsageWidget *ui;
};

#endif // USAGEWIDGET_H
