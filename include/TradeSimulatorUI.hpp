#pragma once

#include <string>
#include <memory>
#include <functional>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "OrderBook.hpp"
#include "WebSocketClient.hpp"

class TradeSimulatorUI : public QMainWindow {
    Q_OBJECT

public:
    TradeSimulatorUI(QWidget* parent = nullptr);
    ~TradeSimulatorUI();

private slots:
    void onConnectClicked();
    void onDisconnectClicked();
    void onOrderBookUpdate(const json& data);
    void onCalculateClicked();

private:
    // UI Components
    QWidget* central_widget_;
    QHBoxLayout* main_layout_;
    
    // Left Panel (Input)
    QWidget* input_panel_;
    QVBoxLayout* input_layout_;
    QComboBox* exchange_combo_;
    QComboBox* asset_combo_;
    QLineEdit* quantity_input_;
    QLineEdit* volatility_input_;
    QComboBox* fee_tier_combo_;
    QPushButton* connect_button_;
    QPushButton* disconnect_button_;
    
    // Right Panel (Output)
    QWidget* output_panel_;
    QVBoxLayout* output_layout_;
    QLabel* slippage_label_;
    QLabel* fees_label_;
    QLabel* market_impact_label_;
    QLabel* net_cost_label_;
    QLabel* maker_taker_label_;
    QLabel* latency_label_;
    
    // Core components
    std::unique_ptr<OrderBook> order_book_;
    std::unique_ptr<WebSocketManager> ws_manager_;
    
    // Helper methods
    void setupUI();
    void updateOutputs();
    void calculateMetrics();
    void startLatencyMeasurement();
    void stopLatencyMeasurement();
    
    // Performance metrics
    std::chrono::system_clock::time_point last_update_time_;
    double processing_latency_;
}; 