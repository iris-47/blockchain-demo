#ifndef CONFIG_H
#define CONFIG_H
#include <QtGlobal>

namespace CONFIG
{
// shard related
const qreal RADIUS_RATE = 30.f; // 分片最大半径与片内节点个数的关系
const qreal SHARD_BORDER = 10.f; // 分片边界宽度
const qreal NODE_RADIUS = 10.f; // 节点半径

// message related
const qreal MESSAGE_SIZE = 3.f; // 消息半径
const qreal MESSAGE_SPEED = 5.f; // 片间消息传递速度
const qreal INNER_MESSAGE_SPEED = 3.f; // 片内消息传递速度
const qreal REPLY_ANIMATION_INTERVAL = 800.0f; // 回复动画间隔(ms)
}

#endif // CONFIG_H
