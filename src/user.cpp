#include <iostream>
#include <string>
#include <vector>
#include "order.cpp"

class single_user {
    int user_id;
    int contract_id;
    int current_position;
    std::vector<Order> order_list;
    std::string user_name;

    int add_order(Order order) {
        this->order_list.push_back(order);
        return 0;
    }


    public:
    single_user(int user_id, int contract_id, std::string user_name) {
        this->user_id = user_id;
        this->contract_id = contract_id;
        this->user_name = user_name;
        this->current_position = 0;
    }

    std::string get_user_name() {
        return this->user_name;
    }



};
