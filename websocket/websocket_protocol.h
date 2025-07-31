#ifndef XDX_CPU_WEBSOCKET_PROTOCOL_H
#define XDX_CPU_WEBSOCKET_PROTOCOL_H

#include <mutex>
#include <set>
#include <thread>
#include <iostream>
#include <cstdint>
#include <string>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/connection.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using websocketpp::connection_hdl;
using namespace rapidjson;

typedef enum
{
    MSG_INFO = 1, // 普通信息
    MSG_WARM = 2, // 警告信息
    MSG_ERROR = 3, // 错误信息
    MSG_SUCCESS = 4, // 成功信息
}MsgType;

typedef struct
{
    Document doc;
    int16_t cycle;//周期时间
    Value value_info;//查询信息
}CycleInfo;

typedef struct
{
    //XXX
    CycleInfo cmd_XXX_cycle;//XXX指令周期
    
} SQueryCycleInf;

#define WEBSOCKET_MONITOR_PORT (9099)


class WebsocketProtocol{
public:

/**
 * @brief 解析json命令码
 *
 * 该函数用于解析收到的json命令码，并根据命令类型和命令码更新连接的状态。
 *
 * @param hdl 连接句柄
 * @param payload 收到的json内容
 */
    void unpack_cmd(websocketpp::connection_hdl hdl, std::string payload);

/**
 * @brief 通过websocket返回前端信息
 *
 * 该函数用于通过websocket向前端发送所有信息。
 *
 * @param cmdType 命令类型
 * @param cmd 命令码
 * @param data 数据
 * @param msg 执行信息
 * @param additional 周期
 * @param hdl 连接句柄
 */
    void push_to_websocket(uint8_t cmdType, uint8_t cmd, rapidjson::Value & data, int16_t additional, const websocketpp::connection_hdl& hdl, rapidjson::Value & msg);

/**
 * @brief 打包并返回操作结果响应
 * @param cmdType 命令类型 (0xAA=设置命令，0x55=查询命令)
 * @param cmd 具体命令码 (参考协议定义)
 * @param res 操作结果 (0=成功，非零=错误码)
 * @param hdl 当前连接的句柄
 */
    void pack_back_res(uint8_t cmdType, uint8_t cmd, uint8_t res, const connection_hdl& hdl, rapidjson::Value & msg);

/**
 * @brief 通过websocket返回前端心跳包
 *
 * 该函数用于通过websocket向前端发送心跳包消息。
 */
    void push_heart_to_websocket();

/**
 * @brief 设置消息信息
 *
 * 该函数用于设置消息的类型、级别和文本内容。
 *
 * @param msg 消息对象
 * @param level 消息级别
 * @param text 消息文本内容
 */
    void websocket_set_msg_info(rapidjson::Document & doc_temp, rapidjson::Value & msg, MsgType level, const std::string& text);

    

    static WebsocketProtocol & getInstance(){
        static WebsocketProtocol instance;

        return instance;
    }

private:
    WebsocketProtocol() = default;
};

/**
 * @brief 启动Web服务器
 *
 * 该函数用于启动Web服务器并监听指定端口。
 *
 * @param server 指向web_server对象的指针
 */
void pt_run();

#endif //XDX_CPU_WEBSOCKET_PROTOCOL_H
