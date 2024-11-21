#include <iostream>
#include <vector>
#include <string>
#include "../src/matching.h"

int main() {

    std::cout << "Starting test" << std::endl;
    limit_order_book lob;
    price_time_priority ptp = price_time_priority();
    std::cout << "Created limit order book" << std::endl;

    LimitOrder buy_orders[] = {LimitOrder(1, 0, 0, 175, 10000, BUY), LimitOrder(2, 0, 0, 177, 10000, BUY), LimitOrder(3, 0, 0, 179, 10000, BUY), LimitOrder(4, 0, 0, 180, 10000, BUY)};
    LimitOrder sell_orders[] = {LimitOrder(5, 0, 0, 183, 10000, SELL), LimitOrder(6, 0, 0, 184, 10000, SELL), LimitOrder(7, 0, 0, 186, 10000, SELL), LimitOrder(8, 0, 0, 190, 10000, SELL)};

    std::cout << "Created orders" << std::endl;
    std::cout << "buy order type" << buy_orders[0].get_order_type() << std::endl;
    std::cout << "buy order 0 pointer: " << &buy_orders[0] << std::endl;
    for (int i = 0; i < 4; i++) {
        ptp.engine_update(&lob, &buy_orders[i]);
        ptp.engine_update(&lob, &sell_orders[i]);
    }
    std::cout << "Added opening orders" << std::endl;
    std::cout << "Best bid: "<< lob.get_best_bid() << std::endl;
    std::cout << "Best ask: "<< lob.get_best_ask() << std::endl; 

    LimitOrder o1 = LimitOrder(9, 0, 0, 183, 5000, BUY);
    std::cout << "Added buy order at 183 of size 5000" << std::endl;
    ptp.engine_update(&lob, &o1);
    std::cout << "Best bid: "<< lob.get_best_bid() << " ; " << lob.get_best_bid_size() << std::endl;
    std::cout << "Best ask: "<< lob.get_best_ask() << " ; " << lob.get_best_ask_size() << std::endl; 

    LimitOrder o2 = LimitOrder(10, 0, 0, 184, 5000, BUY);
    std::cout << "Added buy order at 184 of size 5000" << std::endl;
    ptp.engine_update(&lob, &o2);
    std::cout << "Best bid: "<< lob.get_best_bid() << " ; " << lob.get_best_bid_size() << std::endl;
    std::cout << "Best ask: "<< lob.get_best_ask() << " ; " << lob.get_best_ask_size() << std::endl; 

    MarketOrder o3 = MarketOrder(11, 0, 0, 8000, SELL);
    std::cout << "Added market sell order of size 8000" << std::endl;
    ptp.engine_update(&lob, &o3);
    std::cout << "Best bid: "<< lob.get_best_bid() << " ; " << lob.get_best_bid_size() << std::endl;
    std::cout << "Best ask: "<< lob.get_best_ask() << " ; " << lob.get_best_ask_size() << std::endl; 






}
