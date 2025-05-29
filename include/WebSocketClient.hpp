#pragma once

#include <string>
#include <functional>
#include <memory>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using WebSocketClient = websocketpp::client<websocketpp::config::asio_tls_client>;
using MessageHandler = std::function<void(const json&)>;

class WebSocketManager {
public:
    WebSocketManager();
    ~WebSocketManager();

    // Connect to WebSocket endpoint
    void connect(const std::string& uri);
    
    // Disconnect from WebSocket
    void disconnect();
    
    // Set message handler
    void setMessageHandler(MessageHandler handler);
    
    // Check if connected
    bool isConnected() const;

private:
    std::unique_ptr<WebSocketClient> client_;
    websocketpp::connection_hdl connection_;
    MessageHandler message_handler_;
    bool connected_;
    
    // WebSocket callbacks
    void onOpen(websocketpp::connection_hdl hdl);
    void onClose(websocketpp::connection_hdl hdl);
    void onMessage(websocketpp::connection_hdl hdl, WebSocketClient::message_ptr msg);
    void onError(websocketpp::connection_hdl hdl);
}; 