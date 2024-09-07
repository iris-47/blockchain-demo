#ifndef CONFIG_H
#define CONFIG_H
#include <QVector>
#include <QtGlobal>
// namespace CONFIG
// {
// extern int FRAME_RATE; // 帧率

// // shard related
// extern int SHARD_NUM; // 分片个数
// extern qreal RADIUS_RATE; // 分片最大半径与片内节点个数的关系
// extern qreal SHARD_BORDER; // 分片边界宽度
// extern qreal NODE_RADIUS; // 节点半径

// // message related
// extern qreal MESSAGE_SIZE; // 消息半径
// extern qreal MESSAGE_SPEED; // 片间消息传递速度
// extern qreal INNER_MESSAGE_SPEED; // 片内消息传递速度
// extern qreal REPLY_ANIMATION_INTERVAL; // 回复动画间隔(ms)
// }
class CONFIG
{
public:
    static int FRAME_RATE; // 帧率

    // shard related
    static int SHARD_NUM; // 分片个数
    static qreal RADIUS_RATE; // 分片最大半径与片内节点个数的关系
    static qreal SHARD_BORDER; // 分片边界宽度
    static qreal NODE_RADIUS; // 节点半径

    static int START_INDEX;
    static QVector<int> END_INDEX;

    // message related
    static qreal MESSAGE_SIZE; // 消息半径
    static qreal MESSAGE_SPEED; // 片间消息传递速度
    static qreal INNER_MESSAGE_SPEED; // 片内消息传递速度
    static qreal REPLY_ANIMATION_INTERVAL; // 回复动画间隔(ms)
};

#endif // CONFIG_H
