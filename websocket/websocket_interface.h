//
// Created by root on 2024/7/8.
//

#ifndef XDX_CPU_WEBSOCKET_INTERFACE_H
#define XDX_CPU_WEBSOCKET_INTERFACE_H

#include <mutex>
#include <set>
#include <thread>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/connection.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class web_server
{
public:
    web_server();

    ~web_server();

    void on_open(connection_hdl hdl);

    void on_close(connection_hdl hdl);

    void on_message(connection_hdl hdl, server::message_ptr msg);

    void on_fail(connection_hdl hdl);

    void run(uint16_t port);



public:
    typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;
    
    server m_server;
    con_list m_connections;
    std::mutex m_mutex;
};

#endif //XDX_CPU_WEBSOCKET_INTERFACE_H
