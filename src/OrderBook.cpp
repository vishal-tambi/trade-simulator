#include "OrderBook.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>

OrderBook::OrderBook(const std::string& symbol) 
    : symbol_(symbol), last_update_(std::chrono::system_clock::now()) {}

void OrderBook::update(const json& data) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Update bids and asks
    updateSide(bids_, data["bids"]);
    updateSide(asks_, data["asks"]);
    
    // Sort bids in descending order and asks in ascending order
    std::sort(bids_.begin(), bids_.end(), 
        [](const OrderBookLevel& a, const OrderBookLevel& b) { return a.price > b.price; });
    std::sort(asks_.begin(), asks_.end(), 
        [](const OrderBookLevel& a, const OrderBookLevel& b) { return a.price < b.price; });
    
    last_update_ = std::chrono::system_clock::now();
}

void OrderBook::updateSide(std::deque<OrderBookLevel>& side, const json& levels) {
    side.clear();
    for (const auto& level : levels) {
        side.emplace_back(std::stod(level[0].get<std::string>()), 
                         std::stod(level[1].get<std::string>()));
    }
}

double OrderBook::getBestBid() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return bids_.empty() ? 0.0 : bids_.front().price;
}

double OrderBook::getBestAsk() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return asks_.empty() ? 0.0 : asks_.front().price;
}

double OrderBook::getSpread() const {
    return getBestAsk() - getBestBid();
}

double OrderBook::getMidPrice() const {
    return (getBestAsk() + getBestBid()) / 2.0;
}

double OrderBook::calculateMarketImpact(double quantity) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Almgren-Chriss model parameters
    const double eta = 0.1;  // Temporary impact parameter
    const double gamma = 0.1;  // Permanent impact parameter
    const double sigma = 0.2;  // Volatility
    
    // Calculate market impact
    double temp_impact = eta * quantity;
    double perm_impact = gamma * quantity;
    
    return temp_impact + perm_impact;
}

double OrderBook::calculateSlippage(double quantity) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (asks_.empty() || bids_.empty()) return 0.0;
    
    double remaining_quantity = quantity;
    double weighted_price = 0.0;
    
    // Calculate weighted average price for the given quantity
    for (const auto& level : asks_) {
        if (remaining_quantity <= 0) break;
        
        double executed = std::min(remaining_quantity, level.quantity);
        weighted_price += executed * level.price;
        remaining_quantity -= executed;
    }
    
    if (remaining_quantity > 0) {
        // Not enough liquidity
        return std::numeric_limits<double>::infinity();
    }
    
    return (weighted_price / quantity) - getMidPrice();
}

double OrderBook::calculateMakerTakerProportion() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Simple logistic regression model
    double spread = getSpread();
    double mid_price = getMidPrice();
    
    // Normalize spread relative to mid price
    double normalized_spread = spread / mid_price;
    
    // Logistic function
    return 1.0 / (1.0 + std::exp(-10.0 * (normalized_spread - 0.001)));
}

double OrderBook::calculateLinearRegression(const std::vector<double>& x, 
                                          const std::vector<double>& y) const {
    if (x.size() != y.size() || x.empty()) return 0.0;
    
    double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
    double sum_xy = 0.0;
    double sum_xx = 0.0;
    
    for (size_t i = 0; i < x.size(); ++i) {
        sum_xy += x[i] * y[i];
        sum_xx += x[i] * x[i];
    }
    
    double n = static_cast<double>(x.size());
    return (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
}

double OrderBook::calculateQuantileRegression(const std::vector<double>& x,
                                            const std::vector<double>& y,
                                            double quantile) const {
    // Simple implementation - in practice, you'd want to use a more sophisticated method
    return calculateLinearRegression(x, y);
}

std::chrono::system_clock::time_point OrderBook::getLastUpdateTime() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return last_update_;
} 