#include "TradeSimulatorUI.hpp"
#include <QMessageBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QDoubleValidator>
#include <iostream>

TradeSimulatorUI::TradeSimulatorUI(QWidget* parent)
    : QMainWindow(parent)
    , processing_latency_(0.0) {
    
    setupUI();
    
    // Initialize components
    order_book_ = std::make_unique<OrderBook>("BTC-USDT-SWAP");
    ws_manager_ = std::make_unique<WebSocketManager>();
    
    // Set up WebSocket message handler
    ws_manager_->setMessageHandler([this](const json& data) {
        onOrderBookUpdate(data);
    });
}

TradeSimulatorUI::~TradeSimulatorUI() = default;

void TradeSimulatorUI::setupUI() {
    // Create central widget and main layout
    central_widget_ = new QWidget(this);
    main_layout_ = new QHBoxLayout(central_widget_);
    setCentralWidget(central_widget_);
    
    // Create input panel
    input_panel_ = new QGroupBox("Input Parameters", central_widget_);
    input_layout_ = new QVBoxLayout(input_panel_);
    
    // Create input fields
    exchange_combo_ = new QComboBox(input_panel_);
    exchange_combo_->addItem("OKX");
    
    asset_combo_ = new QComboBox(input_panel_);
    asset_combo_->addItem("BTC-USDT-SWAP");
    
    quantity_input_ = new QLineEdit(input_panel_);
    quantity_input_->setValidator(new QDoubleValidator(0.0, 1000000.0, 8, quantity_input_));
    quantity_input_->setText("100.0");
    
    volatility_input_ = new QLineEdit(input_panel_);
    volatility_input_->setValidator(new QDoubleValidator(0.0, 1.0, 4, volatility_input_));
    volatility_input_->setText("0.2");
    
    fee_tier_combo_ = new QComboBox(input_panel_);
    fee_tier_combo_->addItems({"Tier 1", "Tier 2", "Tier 3"});
    
    connect_button_ = new QPushButton("Connect", input_panel_);
    disconnect_button_ = new QPushButton("Disconnect", input_panel_);
    disconnect_button_->setEnabled(false);
    
    // Add input fields to layout
    QFormLayout* input_form = new QFormLayout();
    input_form->addRow("Exchange:", exchange_combo_);
    input_form->addRow("Asset:", asset_combo_);
    input_form->addRow("Quantity (USD):", quantity_input_);
    input_form->addRow("Volatility:", volatility_input_);
    input_form->addRow("Fee Tier:", fee_tier_combo_);
    
    input_layout_->addLayout(input_form);
    input_layout_->addWidget(connect_button_);
    input_layout_->addWidget(disconnect_button_);
    
    // Create output panel
    output_panel_ = new QGroupBox("Output Parameters", central_widget_);
    output_layout_ = new QVBoxLayout(output_panel_);
    
    // Create output labels
    slippage_label_ = new QLabel("Expected Slippage: --", output_panel_);
    fees_label_ = new QLabel("Expected Fees: --", output_panel_);
    market_impact_label_ = new QLabel("Expected Market Impact: --", output_panel_);
    net_cost_label_ = new QLabel("Net Cost: --", output_panel_);
    maker_taker_label_ = new QLabel("Maker/Taker Proportion: --", output_panel_);
    latency_label_ = new QLabel("Internal Latency: --", output_panel_);
    
    // Add output labels to layout
    output_layout_->addWidget(slippage_label_);
    output_layout_->addWidget(fees_label_);
    output_layout_->addWidget(market_impact_label_);
    output_layout_->addWidget(net_cost_label_);
    output_layout_->addWidget(maker_taker_label_);
    output_layout_->addWidget(latency_label_);
    
    // Add panels to main layout
    main_layout_->addWidget(input_panel_);
    main_layout_->addWidget(output_panel_);
    
    // Connect signals and slots
    connect(connect_button_, &QPushButton::clicked, this, &TradeSimulatorUI::onConnectClicked);
    connect(disconnect_button_, &QPushButton::clicked, this, &TradeSimulatorUI::onDisconnectClicked);
    
    // Set window properties
    setWindowTitle("Trade Simulator");
    resize(800, 400);
}

void TradeSimulatorUI::onConnectClicked() {
    try {
        ws_manager_->connect("wss://ws.gomarket-cpp.goquant.io/ws/l2-orderbook/okx/BTC-USDT-SWAP");
        connect_button_->setEnabled(false);
        disconnect_button_->setEnabled(true);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Connection Error", e.what());
    }
}

void TradeSimulatorUI::onDisconnectClicked() {
    ws_manager_->disconnect();
    connect_button_->setEnabled(true);
    disconnect_button_->setEnabled(false);
}

void TradeSimulatorUI::onOrderBookUpdate(const json& data) {
    startLatencyMeasurement();
    
    // Update order book
    order_book_->update(data);
    
    // Update UI
    updateOutputs();
    
    stopLatencyMeasurement();
}

void TradeSimulatorUI::updateOutputs() {
    double quantity = quantity_input_->text().toDouble();
    
    // Calculate metrics
    double slippage = order_book_->calculateSlippage(quantity);
    double market_impact = order_book_->calculateMarketImpact(quantity);
    double maker_taker = order_book_->calculateMakerTakerProportion();
    
    // Calculate fees (simplified)
    double fee_rate = 0.001; // 0.1% fee rate
    double fees = quantity * fee_rate;
    
    // Calculate net cost
    double net_cost = slippage + fees + market_impact;
    
    // Update labels
    slippage_label_->setText(QString("Expected Slippage: %1 USD").arg(slippage, 0, 'f', 2));
    fees_label_->setText(QString("Expected Fees: %1 USD").arg(fees, 0, 'f', 2));
    market_impact_label_->setText(QString("Expected Market Impact: %1 USD").arg(market_impact, 0, 'f', 2));
    net_cost_label_->setText(QString("Net Cost: %1 USD").arg(net_cost, 0, 'f', 2));
    maker_taker_label_->setText(QString("Maker/Taker Proportion: %1").arg(maker_taker, 0, 'f', 2));
    latency_label_->setText(QString("Internal Latency: %1 ms").arg(processing_latency_, 0, 'f', 2));
}

void TradeSimulatorUI::startLatencyMeasurement() {
    last_update_time_ = std::chrono::system_clock::now();
}

void TradeSimulatorUI::stopLatencyMeasurement() {
    auto end_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - last_update_time_);
    processing_latency_ = duration.count() / 1000.0; // Convert to milliseconds
} 