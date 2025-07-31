#include <vector>
#include "websocket_interface.h"
#include "websocket_protocol.h"

using namespace std;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

web_server::web_server()
{
    m_server.init_asio();
    m_server.set_access_channels(websocketpp::log::alevel::all);
    m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    m_server.clear_access_channels(websocketpp::log::alevel::frame_header);
    m_server.set_open_handler(bind(&web_server::on_open, this, _1));
    m_server.set_close_handler(bind(&web_server::on_close, this, _1));
    m_server.set_message_handler(bind(&web_server::on_message, this, _1, _2));
    m_server.set_fail_handler(bind(&web_server::on_fail, this, _1));
}

web_server::~web_server() = default;

void web_server::on_open(connection_hdl hdl)
{
    std::string IP_Port;
    std::string IP;
    uint16_t port;
    std::lock_guard < std::mutex > lock(m_mutex);
    m_connections.insert(hdl);
    server::connection_ptr con = m_server.get_con_from_hdl(hdl);
    std::cout << con->get_remote_endpoint() << " connect !" << std::endl;
}

void web_server::on_close(connection_hdl hdl)
{
    std::string IP_Port;
    std::string IP;
    uint16_t port;
    std::lock_guard < std::mutex > lock(m_mutex);
    m_connections.erase(hdl);
    server::connection_ptr con = m_server.get_con_from_hdl(hdl);
    std::cout << con->get_remote_endpoint() << " disconnect !" << std::endl;
    
}

void web_server::on_fail(connection_hdl h)
{
    int sss = 0;
}


void web_server::on_message(connection_hdl hdl, server::message_ptr msg)
{
    // 假设我们收到的消息是文本类型的
    std::string payload = msg->get_payload();
    
    // 这里可以添加处理接收到的消息的逻辑
    // 例如，可以将消息打印到控制台或转发给其他客户端
    std::cout << "Received message: " << payload << std::endl;
    
    // （可选）向发送者发送一个确认消息
    try
    {
        WebsocketProtocol::getInstance().unpack_cmd(hdl, payload);//解析json数据函数
    } catch (websocketpp::lib::error_code & e)
    {
        std::cout << payload << " " << e.message() << std::endl;
    }
}

void web_server::run(uint16_t port)
{
    try
    {
        m_server.set_reuse_addr(true);
        m_server.listen(tcp::v4(), port);
        m_server.start_accept();
        m_server.run();
    } catch (websocketpp::lib::error_code & e)
    {
        std::cout << e.message() << std::endl;
    }
}






