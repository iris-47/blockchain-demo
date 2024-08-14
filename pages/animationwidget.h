#ifndef ANIMATIONWIDGET_H
#define ANIMATIONWIDGET_H

#include <QWidget>

class AnimationScene;
class GraphicsView;

namespace Ui {
class AnimationWidget;
}

class AnimationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationWidget(QWidget *parent = nullptr);
    ~AnimationWidget();

private:
    Ui::AnimationWidget *ui;
    void onClickedStart();
    void onClickReset();

    void initConnections();

    AnimationScene *scene;

    bool m_isRunning;
};

#endif // ANIMATIONWIDGET_H
