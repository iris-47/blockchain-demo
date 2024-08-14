#ifndef LEGENDITEM_H
#define LEGENDITEM_H

#include <QGraphicsWidget>
#include "struct.h"

class LegendItem : public QGraphicsWidget
{
    Q_OBJECT
public:
    LegendItem(QGraphicsItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setInitialPos(QPointF pos) { setPos(pos); }

signals:
        void legendClosed();

private slots:
    void toggleCollapse();
    void closeLegend();

private:
    QString typeToString(MessageType type) {
        switch (type) {
        case MessageType::DEFAULT: return "Default";
        case MessageType::PROPOSE: return "Propose";
        case MessageType::PRE_PREPARE: return "Pre-Prepare";
        case MessageType::PREPARE: return "Prepare";
        case MessageType::COMMIT: return "Commit";
        case MessageType::VERIFY_REQUEST: return "Verify Request";
        case MessageType::VERIFIED: return "Verified";
        case MessageType::BLOCK: return "Block";
        default: return "Unknown";
        }
    }

    bool m_isCollapsed;
};

#endif // LEGENDITEM_H
