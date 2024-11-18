#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "order.h"

bool compare_buy(LimitOrder& a, LimitOrder& b) {
    if (a.get_price() == b.get_price()) 
        return a.get_timestamp() < b.get_timestamp();
    return a.get_price() > b.get_price();
}

bool compare_sell(LimitOrder& a, LimitOrder& b) {
    if (a.get_price() == b.get_price()) 
        return a.get_timestamp() < b.get_timestamp();
    return a.get_price() < b.get_price();
}

class limit_order_book {

    int num_orders;
    std::set<LimitOrder, decltype(compare_buy)*> buy_orders;
    std::set<LimitOrder, decltype(compare_sell)*> sell_orders;
    std::map<int, LimitOrder> order_map;

    public:
    limit_order_book() {
        num_orders = 0;
    }

    int get_best_bid() {
        LimitOrder best_bid = *buy_orders.begin();
        return best_bid.get_price();
    }
    int get_best_ask() {
        LimitOrder best_ask = *sell_orders.begin();
        return best_ask.get_price();
    }

    void incr_num_orders() {
        num_orders++;
    }

    int get_num_orders() {
        return num_orders;
    }

    LimitOrder get_order_from_id(int id) {
        return order_map[id];
    }

    int add_order(LimitOrder order) {
        if (order.side == BUY) {
            buy_orders.insert(order);
        } else if (order.side == SELL) {
            sell_orders.insert(order);
        }
        order_map[order.order_id] = order;
        return 0;
    }

    int cancel_order(int order_id) {
        if (order_map.find(order_id) == order_map.end())
            return -1;
        LimitOrder order = order_map[order_id];
        if (order.side == BUY) {
            buy_orders.erase(order);
        } else if (order.side == SELL) {
            sell_orders.erase(order);
        }
        order_map.erase(order_id);
        return 0;
    }



};
