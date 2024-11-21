#ifndef ORDER_H
#define ORDER_H

#include <iostream>

enum Side {
    BUY,
    SELL
};

class Order {
    public:
    int user_id;
    int contract_id;
    int order_id;
    int timestamp;
    std::string order_type;

    Order(int order_id, int user_id, int contract_id, std::string order_type) {
        this->order_id = order_id;
        this->user_id = user_id;
        this->contract_id = contract_id;
        this->timestamp = time(0);
        this->order_type = order_type;
    } 
    Order() {}
    std::string get_order_type() {
        return this->order_type;
    }
    int get_timestamp() {
        return this->timestamp;
    }

    virtual ~Order() {}
};

class LimitOrder : public Order {
    public:
    int price;
    int quantity;
    Side side;

    LimitOrder(int order_id, int user_id, int contract_id, int price, int quantity, Side side) : Order(order_id, user_id, contract_id, "LIMIT"){
        this->price = price;
        this->quantity = quantity;
        this->side = side;
    }
    LimitOrder() : Order(-1, -1, -1, "LIMIT") {};
    int get_price() {
        return this->price;
    }
};

class MarketOrder : public Order {
    public:
    int quantity;
    Side side;

    MarketOrder(int order_id, int user_id, int contract_id, int quantity, Side side) : Order(order_id, user_id, contract_id, "MARKET") {
        this->quantity = quantity;
        this->side = side;
    }
};

class CancelOrder : public Order {
    int cancel_order_id;
    public:
    CancelOrder(int order_id, int user_id, int contract_id, int cancel_order_id) {
        this->cancel_order_id = cancel_order_id;
    }
};

#endif
