#pragma once

#include <string>
#include <vector>
#include <deque>
#include <mutex>
#include <memory>
#include <chrono>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct OrderBookLevel {
    double price;
    double quantity;
    OrderBookLevel(double p, double q) : price(p), quantity(q) {}
};

class OrderBook {
public:
    OrderBook(const std::string& symbol);
    ~OrderBook() = default;

    // Process incoming orderbook update
    void update(const json& data);
    
    // Get current best bid and ask
    double getBestBid() const;
    double getBestAsk() const;
    
    // Calculate market impact using Almgren-Chriss model
    double calculateMarketImpact(double quantity) const;
    
    // Calculate expected slippage
    double calculateSlippage(double quantity) const;
    
    // Calculate maker/taker proportion
    double calculateMakerTakerProportion() const;
    
    // Get current spread
    double getSpread() const;
    
    // Get mid price
    double getMidPrice() const;
    
    // Get timestamp of last update
    std::chrono::system_clock::time_point getLastUpdateTime() const;

private:
    std::string symbol_;
    std::deque<OrderBookLevel> bids_;
    std::deque<OrderBookLevel> asks_;
    mutable std::mutex mutex_;
    std::chrono::system_clock::time_point last_update_;
    
    // Helper methods
    void updateSide(std::deque<OrderBookLevel>& side, const json& levels);
    double calculateLinearRegression(const std::vector<double>& x, const std::vector<double>& y) const;
    double calculateQuantileRegression(const std::vector<double>& x, const std::vector<double>& y, double quantile) const;
}; 