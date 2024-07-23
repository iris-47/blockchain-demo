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

    AnimationScene *scene;
};

#endif // ANIMATIONWIDGET_H
