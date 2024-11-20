#include <iostream>
#include <vector>
#include <string>
#include "../src/matching.h"

int main() {
    limit_order_book lob;
    lob.add_order(LimitOrder(1, 0, 0, 180, 10000, BUY));
    lob.add_order(LimitOrder(2, 0, 0, 190, 10000, SELL));
    std::cout << "Best bid: "<< lob.get_best_bid() << std::endl;
    std::cout << "Best ask: "<< lob.get_best_ask() << std::endl; 
}
