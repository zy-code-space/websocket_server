#include "websocket_protocol.h"

#include <utility>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include "websocket_interface.h"

web_server my_server;

/**
 * @brief 解析json命令码
 *
 * 该函数用于解析收到的json命令码，并根据命令类型和命令码更新连接的状态。
 *
 * @param hdl 连接句柄
 * @param payload 收到的json内容
 */
void WebsocketProtocol::unpack_cmd(websocketpp::connection_hdl hdl, std::string payload)
{
    uint8_t cmdType = 0, cmd = 0;
    int16_t additional;
    rapidjson::Document doc;
    auto & allocator = doc.GetAllocator();    // 取出分配器以备后用
    rapidjson::Value msg;
    
    std::cout << "document Parse...." << std::endl;
    //解析参数.....
    
    WebsocketProtocol::getInstance().websocket_set_msg_info(doc, msg, MSG_SUCCESS, "设置成功");
    WebsocketProtocol::getInstance().pack_back_res(cmdType, cmd, 0, hdl, msg); // 返回成功
    
    
}

/**
 * @brief 启动Web服务器
 *
 * 该函数用于启动Web服务器并监听指定端口。
 *
 * @param server 指向web_server对象的指针
 */
void pt_run()
{
    sleep(1); //延时2s等待设备初始化完成之后，进行websocket连接
    my_server.run(WEBSOCKET_MONITOR_PORT);
}

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
void WebsocketProtocol::push_to_websocket(uint8_t cmdType, uint8_t cmd, rapidjson::Value & data, int16_t additional, const websocketpp::connection_hdl & hdl, rapidjson::Value & msg)
{
    rapidjson::Document doc_push;
    auto & allocator = doc_push.GetAllocator();    // 取出分配器以备后用
    doc_push.SetObject();
    doc_push.AddMember("cmdType", cmdType, allocator);
    doc_push.AddMember("cmd", cmd, allocator);
    doc_push.AddMember("additional", additional, allocator);
    
    rapidjson::Value msg_copy;
    msg_copy.CopyFrom(msg, allocator);
    doc_push.AddMember("msg", msg_copy, allocator);
    
    rapidjson::Value data_copy;
    data_copy.CopyFrom(data, allocator);
    doc_push.AddMember("data", data_copy, allocator);
    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer < StringBuffer > writer(buffer);
    doc_push.Accept(writer);
    std::string sTT = buffer.GetString();
    for (auto iter : my_server.m_connections)
    {
        try
        {
            server::connection_ptr con = my_server.m_server.get_con_from_hdl(iter);
            auto state = con->get_state();
            if (state == websocketpp::session::state::open)
            {
                my_server.m_server.send(iter, sTT, websocketpp::frame::opcode::text);
            }
            else
            {
                printf("websocket++连接不可用\n");
            }
        } catch (websocketpp::lib::error_code & e)
        {
            std::cout << e.message() << std::endl;
        }
    }
    
}

/**
 * @brief 打包并返回操作结果响应
 * @param cmdType 命令类型 (0xAA=设置命令，0x55=查询命令)
 * @param cmd 具体命令码 (参考协议定义)
 * @param res 操作结果 (0=成功，非零=错误码)
 * @param hdl 当前连接的句柄
 */
void WebsocketProtocol::pack_back_res(uint8_t cmdType, uint8_t cmd, uint8_t res, const connection_hdl & hdl, rapidjson::Value & msg)
{
    Value dat;
    dat.SetObject();
    push_to_websocket(cmdType, cmd, dat, res, hdl, msg);
}

/**
 * @brief 通过websocket返回前端心跳包
 *
 * 该函数用于通过websocket向前端发送心跳包消息。
 */
void WebsocketProtocol::push_heart_to_websocket()
{
    std::string sTT = "ws-io-pong";
    for (auto iter : my_server.m_connections)
    {
        try
        {
            server::connection_ptr con = my_server.m_server.get_con_from_hdl(iter);
            auto state = con->get_state();
            if (state == websocketpp::session::state::open)
            {
                // 确认连接状态 可以安全地发送消息
                my_server.m_server.send(iter, sTT, websocketpp::frame::opcode::text);
            }
        } catch (websocketpp::lib::error_code & e)
        {
            std::cout << e.message() << std::endl;
        }
    }
    
}

/**
 * @brief 设置消息信息
 *
 * 该函数用于设置消息的类型、级别和文本内容。
 *
 * @param msg 消息对象
 * @param level 消息级别
 * @param text 消息文本内容
 */
void WebsocketProtocol::websocket_set_msg_info(rapidjson::Document & doc_temp, rapidjson::Value & msg, MsgType level, const std::string & text)
{
    auto & allocator = doc_temp.GetAllocator();
    
    msg.SetObject();
    msg.AddMember("level", level, allocator);
    rapidjson::Value TEXT(text.c_str(), text.size(), allocator);
    msg.AddMember("text", TEXT, allocator);
}