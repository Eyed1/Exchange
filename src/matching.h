#include <iostream>
#include <string>
#include <vector>
#include "book.h"

class matching_engine {
    public:
    std::string matching_engine_name;

    matching_engine(std::string matching_engine_name) {
        this->matching_engine_name = matching_engine_name;
    }

    limit_order_book engine_update(limit_order_book *book, Order order);
};

class log {
    // Class for logs of transactions
    public:
        int success;
        Order passive_order;
        Order aggressive_order;
};

class price_time_priority : public matching_engine {
    public:
    price_time_priority() : matching_engine("price_time_priority") {}

    int engine_update(limit_order_book *book, Order order) {
        if (order.order_type == "CancelOrder") {

        }
        return -1;
    }
};
