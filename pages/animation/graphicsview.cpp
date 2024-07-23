#include "graphicsview.h"

#include <QApplication>
#include <QWheelEvent>
#include <QGraphicsItem>

#include <QDebug>

GraphicsView::GraphicsView(QWidget* parent)
    : m_scale(1){
    setRenderHint(QPainter::Antialiasing, true); // 启用抗锯齿
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 禁用水平滚动条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 禁用垂直滚动条
}

void GraphicsView::wheelEvent(QWheelEvent *event){
    // 按住Ctrl键滚动鼠标滚轮，可以缩放场景
    if(QApplication::keyboardModifiers() & Qt::ControlModifier){
        if(event->angleDelta().y() > 0){
            // scale(1.1, 1.1);
            m_scale *= 1.1;
        }else{
            // scale(0.9, 0.9);
            m_scale *= 0.9;
        }
        setTransform(QTransform::fromScale(m_scale, m_scale));
    }else{
        QGraphicsView::wheelEvent(event);
    }
}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    // 鼠标点击移动背景
    if(event->button() == Qt::LeftButton){
        // Ctrl + 鼠标左键点击节点，可以移动节点
        if(QApplication::keyboardModifiers() == Qt::ControlModifier){
            QGraphicsItem* item = itemAt(event->pos());
            if(item){
                setDragMode(QGraphicsView::NoDrag);
                item->setFlag(QGraphicsItem::ItemIsMovable);
            }
        }else{
            m_lastPos = event->pos();
            m_isPressed = true;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
    // 鼠标移动背景
    if(m_isPressed){
        QPointF movePos = event->pos() - m_lastPos;
        m_lastPos = event->pos();

        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        translate(movePos.x() * m_scale, movePos.y() * m_scale);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    // 鼠标释放
    if(event->button() == Qt::LeftButton){
        m_isPressed = false;
    }

    QGraphicsView::mouseReleaseEvent(event);
}
