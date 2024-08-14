#ifndef CONFIG_H
#define CONFIG_H
#include <QtGlobal>

namespace CONFIG
{
static int FRAME_RATE = 30; // 帧率

// shard related
static qreal RADIUS_RATE = 30.f; // 分片最大半径与片内节点个数的关系
static qreal SHARD_BORDER = 10.f; // 分片边界宽度
static qreal NODE_RADIUS = 10.f; // 节点半径

// message related
static qreal MESSAGE_SIZE = 3.f; // 消息半径
static qreal MESSAGE_SPEED = 5.f; // 片间消息传递速度
static qreal INNER_MESSAGE_SPEED = 3.f; // 片内消息传递速度
static qreal REPLY_ANIMATION_INTERVAL = 800.0f; // 回复动画间隔(ms)
}

#endif // CONFIG_H
