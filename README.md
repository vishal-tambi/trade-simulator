# Trade Simulator

A high-performance trade simulator that leverages real-time market data to estimate transaction costs and market impact for cryptocurrency exchanges.

## Features

- Real-time L2 orderbook data processing
- Market impact calculation using Almgren-Chriss model
- Slippage estimation using linear and quantile regression
- Maker/Taker proportion prediction
- Performance metrics and latency monitoring
- Modern Qt-based user interface

## Requirements

- C++17 compatible compiler
- CMake 3.10 or higher
- Qt 5.15 or higher
- OpenSSL
- vcpkg (for package management)

## Dependencies

- websocketpp (WebSocket client)
- nlohmann_json (JSON parsing)
- Qt (GUI framework)
- OpenSSL (TLS support)

## Building

1. Install vcpkg and required packages:
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install qt5-base openssl websocketpp nlohmann-json
```

2. Build the project:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

1. Launch the application
2. Select the exchange and trading pair
3. Enter the desired quantity and other parameters
4. Click "Connect" to start receiving real-time market data
5. Monitor the output parameters in real-time

## Performance Optimization

The system implements several optimization techniques:

1. Memory Management
   - Efficient data structures for orderbook management
   - Smart pointers for resource management
   - Lock-free algorithms where possible

2. Network Communication
   - Asynchronous WebSocket handling
   - Efficient message parsing
   - Connection pooling

3. Data Structure Selection
   - Deque for orderbook levels
   - Hash maps for quick lookups
   - Optimized sorting algorithms

4. Thread Management
   - Dedicated thread for WebSocket communication
   - UI updates on main thread
   - Thread-safe data structures

5. Model Efficiency
   - Cached calculations
   - Incremental updates
   - Efficient regression algorithms

## Model Implementation

### Almgren-Chriss Model
The market impact model considers both temporary and permanent impact:
- Temporary impact: η * Q
- Permanent impact: γ * Q
where η and γ are model parameters and Q is the order quantity.

### Slippage Estimation
Uses a combination of:
- Linear regression for trend analysis
- Quantile regression for extreme value prediction
- Real-time orderbook analysis

### Maker/Taker Proportion
Implements a logistic regression model based on:
- Current spread
- Orderbook depth
- Historical trading patterns

## License

This project is licensed under the MIT License - see the LICENSE file for details. 