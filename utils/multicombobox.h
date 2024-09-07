#ifndef MULTICOMBOBOX_H
#define MULTICOMBOBOX_H

#include <QVector>
#include <QComboBox>
#include <QListWidget>

// TODO: 赶工期粗制滥造，待重新设计
class MultiComboBox : public QComboBox
{
    Q_OBJECT
public:
    MultiComboBox(QWidget *parent = nullptr);

    //隐藏下拉框
    virtual void hidePopup();

    // 添加选项
    void addItem(const QString& text);
    //返回当前选中选项
    QVector<int> currentChoices() const;
    QStringList currentTexts() const;

    //设置文本框默认文字
    void SetPlaceHolderText(const QString& text);
    //设置文本框文字
    void setCurrentText(const QString& _text);

    void clear();

protected:
    virtual bool eventFilter(QObject *watched,QEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void stateChange();

    //击下拉框选项
    void itemClicked(int index);

private:
    //下拉框
    QListWidget* m_ListWidget;
    //文本框，搜索框
    QLineEdit* m_LineEdit;
    //下拉框显示标志
    bool m_showList;
};

#endif // MULTICOMBOBOX_H
