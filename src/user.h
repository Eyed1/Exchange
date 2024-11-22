#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "order.h"
#include "book.h"

class manual_user {
    int user_id;
    int contract_id;
    int current_position;
    std::vector<Order> order_list; // buffer of orders that require processing
    std::vector<LimitOrder> position_list; // list of past transactions
    std::string user_name;
    std::string user_type;

    int add_order(Order order) {
        this->order_list.push_back(order);
        return 0;
    }

    public:
    manual_user(int user_id, int contract_id, std::string user_name = "", std::string user_type = "manual") {
        this->user_id = user_id;
        this->contract_id = contract_id;
        this->user_name = user_name;
        this->current_position = 0;
        this->user_type = user_type;
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

    void add_limit_order(int order_id, int price, int quantity, Side side) {
        LimitOrder order(order_id, this->user_id, this->contract_id, price, quantity, side);
        this->add_order(order);
    }

    void add_market_order(int order_id, int quantity, Side side) {
        MarketOrder order(order_id, this->user_id, this->contract_id, quantity, side);
        this->add_order(order);
    }

    void add_cancel_order(int order_id, int cancel_order_id) {
        CancelOrder order(order_id, this->user_id, this->contract_id, cancel_order_id);
        this->add_order(order);
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
};


class super_user : public manual_user {
    
    public:
    limit_order_book book;
    int num_players;
    int cur_num_orders;

    super_user(int user_id, int contract_id, std::string user_name = "") : manual_user(user_id, contract_id, user_name, "super") {
        this->book = limit_order_book(contract_id);
    }

    void update_lob(limit_order_book book) {
        this->book = book;
        cur_num_orders = std::max(book.get_num_orders(), cur_num_orders);
    }
    void update_num_players(int num_players) {
        this->num_players = num_players;
    }

    void send_limit_order(int price, int quantity, Side side) {
        this->add_limit_order(cur_num_orders*num_players + get_user_id(), price, quantity, side);
        cur_num_orders++;
    }

    void send_market_order(int quantity, Side side) {
        this->add_market_order(cur_num_orders*num_players + get_user_id(),quantity, side);
        cur_num_orders++;
    }

    void send_cancel_order(int order_id) {
        this->add_cancel_order(cur_num_orders*num_players + get_user_id(), order_id);
        cur_num_orders++;
    }

    virtual void handle_log(log_item log);
};
