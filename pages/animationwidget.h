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

    void init();

public slots:
    void onConfigChanged(int type);

private:
    Ui::AnimationWidget *ui;
    void onClickedStart();
    void onClickReset();
    void onClickClear();
    void onClickLegendBtn();

    void initConnections();

    AnimationScene *scene;

    bool m_isRunning;
    bool m_legendVisible;
};

#endif // ANIMATIONWIDGET_H
