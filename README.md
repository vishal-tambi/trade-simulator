# 📈 Trade Simulator  
A high-performance desktop application that simulates cryptocurrency trading by estimating **transaction costs**, **market impact**, and **slippage** in real-time using **Level 2 orderbook data** and advanced quantitative models.

---

## 🚀 Demo  
📦 *Coming soon: packaged build and video walkthrough*

---

## 🧩 Features

✅ **Real-time L2 Orderbook Data**  
📊 Live updates from cryptocurrency exchanges via WebSockets  

✅ **Market Impact Estimation**  
📐 Uses the **Almgren-Chriss model** for both temporary and permanent market impact  

✅ **Slippage Prediction**  
📉 Combines **linear** and **quantile regression** to estimate slippage based on market depth  

✅ **Maker vs Taker Prediction**  
🔄 Predicts trade type proportions using **logistic regression** on historical and live data  

✅ **Performance Monitoring**  
⏱️ Tracks latency, throughput, and system performance  

✅ **Modern Desktop UI**  
🖥️ Built with **Qt 5.15+**, offering a responsive, real-time dashboard  

---

## ⚙️ System Requirements

| Component | Version |
|----------|---------|
| C++      | 17+     |
| CMake    | 3.10+   |
| Qt       | 5.15+   |
| OpenSSL  | Latest  |
| OS       | Cross-platform (Windows/Linux/macOS) |

---

## 📦 Dependencies

- `websocketpp` – WebSocket client for real-time data  
- `nlohmann_json` – JSON parsing  
- `Qt` – GUI framework  
- `OpenSSL` – TLS/SSL encryption  
- `vcpkg` – For managing C++ dependencies  

---

## 🛠️ Building from Source

### 1️⃣ Install vcpkg and Dependencies:
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install qt5-base openssl websocketpp nlohmann-json
2️⃣ Build the Project:
bash
Copy
Edit
mkdir build
cd build
cmake ..
cmake --build .
🧪 Usage Guide
Launch the application

Select exchange & trading pair

Enter quantity and parameters

Connect to the data stream

Observe real-time impact, slippage & performance

⚡ Performance Optimization
🔧 Memory Management
Lock-free structures

Smart pointers

Cache-friendly containers

🌐 Networking
Asynchronous WebSocket handling

Efficient JSON parsing

TLS-secured connections

🧮 Model Optimizations
Cached impact formulas

Incremental regression updates

Real-time event queue

📘 Model Details
📊 Almgren-Chriss Market Impact Model
Temporary Impact: η * Q

Permanent Impact: γ * Q

Q: Order quantity | η, γ: Model constants

📉 Slippage Estimation
Linear regression for trends

Quantile regression for outliers

Based on L2 orderbook state

🔁 Maker/Taker Prediction
Logistic Regression model

Inputs: Spread, Depth, Historical trade data

🧰 Tech Stack
Layer	Tech Stack
Language	C++17
Build System	CMake
GUI	Qt 5.15+
Realtime Data	WebSocket (websocketpp)
Security	OpenSSL
Data Parsing	nlohmann_json
Package Manager	vcpkg

📋 To-Do / Future Enhancements
 Add support for multiple exchanges

 Integrate with real execution APIs for live trading

 Extend modeling to multi-asset simulations

 Export results to CSV/Excel

 Add unit and integration tests

👨‍💻 Author
Vishal Tambi
💻 C++ Developer | Quant Enthusiast | System Optimizer
🔗 LinkedIn
📧 vishaltambi@example.com
📂 GitHub: vishal-tambi

🛡️ License
MIT License. See LICENSE for more details.

css
Copy
Edit

Let me know if you want the author/contact info updated or a badge section added!
