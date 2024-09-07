#include "multicombobox.h"

#include <QEvent>
#include <QLineEdit>
#include <QCheckBox>

MultiComboBox::MultiComboBox(QWidget *parent)
    : QComboBox(parent),
    m_showList(false)
{
    m_ListWidget = new QListWidget(this);
    m_LineEdit = new QLineEdit(this);

    m_LineEdit->setReadOnly(true);
    m_LineEdit->installEventFilter(this);
    // m_LineEdit->setStyleSheet("QLineEdit:disabled{background:rgb(233,233,233);}");
    m_LineEdit->setPlaceholderText(tr("Please select"));

    this->setModel(m_ListWidget->model());
    this->setView(m_ListWidget);
    this->setLineEdit(m_LineEdit);

    connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &MultiComboBox::itemClicked);
}
void MultiComboBox::hidePopup()
{
    m_showList = false;
    int width = this->width();
    int height = this->height();
    int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();
    int y = QCursor::pos().y() - mapToGlobal(geometry().topLeft()).y() + geometry().y();
    if (x >= 0 && x <= width && y >= this->height() && y <= height + this->height())
    {
    }
    else
    {
        QComboBox::hidePopup();
    }
}

void MultiComboBox::addItem(const QString& _text)
{
    QListWidgetItem* item = new QListWidgetItem(m_ListWidget);
    QCheckBox* checkbox = new QCheckBox(this);
    checkbox->setText(_text);
    m_ListWidget->addItem(item);
    m_ListWidget->setItemWidget(item, checkbox);
    connect(checkbox, &QCheckBox::stateChanged, this, &MultiComboBox::stateChange);
}

QVector<int> MultiComboBox::currentChoices() const
{
    QVector<int> choices;
    for (int i = 0; i < m_ListWidget->count(); i++)
    {
        QCheckBox* checkbox = qobject_cast<QCheckBox*>(m_ListWidget->itemWidget(m_ListWidget->item(i)));
        if (checkbox->isChecked())
        {
            choices.push_back(i);
        }
    }
    return choices;
}

void MultiComboBox::SetPlaceHolderText(const QString& _text)
{
    m_LineEdit->setPlaceholderText(_text);
}

void MultiComboBox::setCurrentText(const QString& _text)
{
    m_LineEdit->setText(_text);
}

// 清空所有选项
void  MultiComboBox::clear()
{
    int count = m_ListWidget->count();
    for (int i = 1; i < count; i++)
    {
        QWidget *widget = m_ListWidget->itemWidget(m_ListWidget->item(i));
        QCheckBox *check_box = static_cast<QCheckBox*>(widget);
        check_box->setChecked(false);
    }

    m_LineEdit->clear();
}

bool MultiComboBox::eventFilter(QObject *watched, QEvent *event)
{
    //设置点击输入框也可以弹出下拉框
    if (watched == m_LineEdit && event->type() == QEvent::MouseButtonRelease && this->isEnabled())
    {
        showPopup();
        return true;
    }
    return false;
}

void MultiComboBox::wheelEvent(QWheelEvent *event)
{
    //禁用QComboBox默认的滚轮事件
    Q_UNUSED(event);
}

void MultiComboBox::keyPressEvent(QKeyEvent *event)
{
    QComboBox::keyPressEvent(event);
}

void MultiComboBox::stateChange()
{
    QString selected_data("");
    int count = m_ListWidget->count();
    for (int i = 0; i < count; i++)
    {
        QWidget *widget = m_ListWidget->itemWidget(m_ListWidget->item(i));
        QCheckBox *check_box = static_cast<QCheckBox*>(widget);
        if (check_box->isChecked())
        {
            selected_data.append(check_box->text()).append(";");
        }
    }
    selected_data.chop(1);
    if (!selected_data.isEmpty())
    {
        m_LineEdit->setText(selected_data);
    }
    else
    {
        m_LineEdit->clear();
    }
    m_LineEdit->setToolTip(selected_data);
}

void MultiComboBox::itemClicked(int index)
{
    QCheckBox *check_box = static_cast<QCheckBox*>(m_ListWidget->itemWidget(m_ListWidget->item(index)));
    check_box->setChecked(!check_box->isChecked());
}
