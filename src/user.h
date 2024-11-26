#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "order.h"
#include "book.h"

class manual_user {

    protected: 
    int user_id;
    int contract_id;
    int current_position;
    std::vector<Order> order_list; // buffer of orders that require processing
    std::vector<LimitOrder> position_list; // list of past transactions
    std::string user_name;
    std::string user_type;

    public:
    limit_order_book book;
    int num_players;
    int cur_num_orders;

    manual_user(int user_id, int contract_id, std::string user_name = "", std::string user_type = "manual", int num_players = 1) {
        this->user_id = user_id;
        this->contract_id = contract_id;
        this->user_name = user_name;
        this->current_position = 0;
        this->user_type = user_type;
        this->num_players = num_players;
        this->cur_num_orders = 0;
    }

    std::string get_user_name() {
        return this->user_name;
    }

    std::string get_user_type() {
        return this->user_type;
    }

    int get_user_id() {
        return this->user_id;
    }


    void process_transaction(log_item log) {
        if (log.message == "Trade") {
            if (log.buyer_id == this->user_id) {
                this->current_position += log.quantity;
                this->position_list.push_back(LimitOrder(-1, this->user_id, this->contract_id, log.price, log.quantity, BUY));
            }
            if (log.seller_id == this->user_id) {
                this->current_position -= log.quantity;
                this->position_list.push_back(LimitOrder(-1, this->user_id, this->contract_id, log.price, log.quantity, SELL));
            }
        }
    }

    void update_lob(limit_order_book book) {
        this->book = book;
    }
    void update_num_players(int num_players) {
        this->num_players = num_players;
    }

    int get_position() {
        return this->current_position;
    }
    int get_pnl(int cur_price) {
        int tot_pnl = 0;
        for (int i = 0; i < this->position_list.size(); i++) {
            LimitOrder order = this->position_list[i];
            if (order.side == BUY) {
                tot_pnl += (cur_price - order.price) * order.quantity;
            } else {
                tot_pnl += (order.price - cur_price) * order.quantity;
            }
        }
        return tot_pnl;
    }

    virtual void handle_log(log_item log);

};



