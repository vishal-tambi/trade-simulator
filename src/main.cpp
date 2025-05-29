#include "TradeSimulatorUI.hpp"
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        QApplication app(argc, argv);
        
        TradeSimulatorUI window;
        window.show();
        
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
} 