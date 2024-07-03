#ifndef CONFIG_H
#define CONFIG_H

#define PI 3.14159265358979323846f

namespace CONFIG
{
// shard related
const float RADIUS_RATE = 30.f; // 分片最大半径与片内节点个数的关系
const float NODE_RADIUS = 10.f; // 节点半径

// message related
const float MESSAGE_SPEED = 0.3f; // 消息传递速度
} // namespace CONFIG


enum MessageType{
    // DEFAULT
    DEFAULT,

    // PBFT
    PROPOSE,
    PRE_PREPARE,
    PREPARE,
    COMMIT,

    // CShard
    VERIFY_REQUEST,
    VERIFIED,
    BLOCK,
};

class ColorMap{
public:
    static sf::Color getColor(MessageType type){
        static const std::map<MessageType, sf::Color> colorMap = {
            {MessageType::DEFAULT, sf::Color::White},

            {MessageType::PROPOSE, sf::Color(0xFF3300)},  // 红色
            {MessageType::PRE_PREPARE, sf::Color(0xFF9900)}, // 橙色
            {MessageType::PREPARE, sf::Color::Yellow},
            {MessageType::COMMIT, sf::Color::Green},

            {MessageType::VERIFY_REQUEST, sf::Color(0xFF00FF)}, // 紫色
            {MessageType::VERIFIED, sf::Color::Green},
            {MessageType::BLOCK, sf::Color::Blue},
        };

        auto it = colorMap.find(type);
        if(it != colorMap.end()){
            return it->second;
        }

        // default
        return sf::Color::White;
    }
};

#endif