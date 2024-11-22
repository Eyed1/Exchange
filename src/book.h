#ifndef BOOK_H
#define BOOK_H


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include "order.h"

const int INFTY = 1e9;

bool compare_buy(const LimitOrder& a, const LimitOrder& b) {
    if (a.price == b.price) 
        return a.timestamp< b.timestamp;
    return a.price > b.price;
}

bool compare_sell(const LimitOrder& a, const LimitOrder& b) {
    if (a.price == b.price) 
        return a.timestamp < b.timestamp;
    return a.price < b.price;
}

class limit_order_book {

    int num_orders;
    std::set<LimitOrder, decltype(compare_buy)*> buy_orders;
    std::set<LimitOrder, decltype(compare_sell)*> sell_orders;
    std::map<int, LimitOrder> order_map;
    int contract_id;

    public:
    limit_order_book(int contract_id = 0) {
        num_orders = 0;
        buy_orders = std::set<LimitOrder, decltype(compare_buy)*>(compare_buy);
        sell_orders = std::set<LimitOrder, decltype(compare_sell)*>(compare_sell);
        order_map = std::map<int, LimitOrder>();
        this->contract_id = contract_id;
    }

    int get_best_bid() {
        if (buy_orders.size() == 0)
            return -INFTY;
        LimitOrder best_bid = *buy_orders.begin();
        return best_bid.get_price();
    }
    int get_best_ask() {
        if (sell_orders.size() == 0)
            return INFTY;
        LimitOrder best_ask = *sell_orders.begin();
        return best_ask.get_price();
    }
    int get_best_bid_size() {
        if (buy_orders.size() == 0)
            return 0;
        LimitOrder best_bid = *buy_orders.begin();
        return best_bid.quantity;
    }
    int get_best_ask_size() {
        if (sell_orders.size() == 0)
            return 0;
        LimitOrder best_ask = *sell_orders.begin();
        return best_ask.quantity;
    }

    int get_num_bids() {
        return buy_orders.size();
    }
    int get_num_asks() {
        return sell_orders.size();
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
        assert(order.contract_id == contract_id);
        if (order.side == BUY) {
            buy_orders.insert(order);
        } else if (order.side == SELL) {
            sell_orders.insert(order);
        }
        order_map[order.order_id] = order;
        incr_num_orders();
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
        incr_num_orders();
        return 0;
    }

    int update_order(int order_id, int new_quantity) {
        if (order_map.find(order_id) == order_map.end())
            return -1;
        LimitOrder order = order_map[order_id];
        if (order.side == BUY) {
            buy_orders.erase(order);
        } else if (order.side == SELL) {
            sell_orders.erase(order);
        }
        order.quantity = new_quantity;
        if (order.side == BUY) {
            buy_orders.insert(order);
        } else if (order.side == SELL) {
            sell_orders.insert(order);
        }
        order_map[order_id] = order;
        incr_num_orders();
        return 0;
    }

    LimitOrder get_best_bid_order() {
        return *buy_orders.begin();
    }

    LimitOrder get_best_ask_order() {
        return *sell_orders.begin();
    }
};

#endif
