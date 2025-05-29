#include "WebSocketClient.hpp"
#include <iostream>
#include <thread>

WebSocketManager::WebSocketManager()
    : connected_(false) {
    client_ = std::make_unique<WebSocketClient>();
    
    // Set up logging
    client_->clear_access_channels(websocketpp::log::alevel::all);
    client_->set_access_channels(websocketpp::log::alevel::connect);
    client_->set_access_channels(websocketpp::log::alevel::disconnect);
    client_->set_access_channels(websocketpp::log::alevel::app);
    
    // Initialize ASIO
    client_->init_asio();
    
    // Set up handlers
    client_->set_open_handler(std::bind(&WebSocketManager::onOpen, this, std::placeholders::_1));
    client_->set_close_handler(std::bind(&WebSocketManager::onClose, this, std::placeholders::_1));
    client_->set_message_handler(std::bind(&WebSocketManager::onMessage, this, std::placeholders::_1, std::placeholders::_2));
    client_->set_fail_handler(std::bind(&WebSocketManager::onError, this, std::placeholders::_1));
}

WebSocketManager::~WebSocketManager() {
    disconnect();
}

void WebSocketManager::connect(const std::string& uri) {
    try {
        websocketpp::lib::error_code ec;
        WebSocketClient::connection_ptr con = client_->get_connection(uri, ec);
        
        if (ec) {
            std::cerr << "Could not create connection: " << ec.message() << std::endl;
            return;
        }
        
        client_->connect(con);
        
        // Start the ASIO io_service run loop
        std::thread([this]() {
            try {
                client_->run();
            } catch (const std::exception& e) {
                std::cerr << "WebSocket run error: " << e.what() << std::endl;
            }
        }).detach();
        
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

void WebSocketManager::disconnect() {
    if (connected_) {
        try {
            client_->close(connection_, websocketpp::close::status::going_away, "Disconnecting");
        } catch (const std::exception& e) {
            std::cerr << "Disconnect error: " << e.what() << std::endl;
        }
        connected_ = false;
    }
}

void WebSocketManager::setMessageHandler(MessageHandler handler) {
    message_handler_ = std::move(handler);
}

bool WebSocketManager::isConnected() const {
    return connected_;
}

void WebSocketManager::onOpen(websocketpp::connection_hdl hdl) {
    connection_ = hdl;
    connected_ = true;
    std::cout << "Connected to WebSocket server" << std::endl;
}

void WebSocketManager::onClose(websocketpp::connection_hdl hdl) {
    connected_ = false;
    std::cout << "Disconnected from WebSocket server" << std::endl;
}

void WebSocketManager::onMessage(websocketpp::connection_hdl hdl, WebSocketClient::message_ptr msg) {
    try {
        if (message_handler_) {
            json data = json::parse(msg->get_payload());
            message_handler_(data);
        }
    } catch (const std::exception& e) {
        std::cerr << "Message processing error: " << e.what() << std::endl;
    }
}

void WebSocketManager::onError(websocketpp::connection_hdl hdl) {
    connected_ = false;
    std::cerr << "WebSocket error occurred" << std::endl;
} 