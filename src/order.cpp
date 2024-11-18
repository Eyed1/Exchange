#include <iostream>
#include <string>
#include "order.h"

Order::Order(int order_id, int user_id, int contract_id, std::string order_type) {
        this->order_id = order_id;
        this->user_id = user_id;
        this->contract_id = contract_id;
        this->timestamp = time(0);
        this->order_type = order_type;
}
std::string Order::get_order_type() {
        return this->order_type;
}
int Order::get_timestamp() {
        return this->timestamp;
}

LimitOrder::LimitOrder(int order_id, int user_id, int contract_id, int price, int quantity, Side side) : Order(order_id, user_id, contract_id, "Limit") {
        this->price = price;
        this->quantity = quantity;
        this->side = side;
}
int LimitOrder::get_price() {
        return this->price;
}

MarketOrder::MarketOrder(int order_id, int user_id, int contract_id, int quantity, Side side) : Order(order_id, user_id, contract_id, "Market") {
        this->quantity = quantity;
        this->side = side;
}

CancelOrder::CancelOrder(int order_id, int user_id, int contract_id, int cancel_order_id) : Order(order_id, user_id, contract_id, "Cancel") {
        this->cancel_order_id = cancel_order_id;
}




