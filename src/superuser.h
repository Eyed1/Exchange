#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "game.h"

class super_user : public manual_user {

    Game game; 

    int add_order(Order order) {
        game.game_lock.lock();
        game.send_order(order);
        return 0;
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

    public:
    super_user(int user_id, int contract_id, Game game, std::string user_name = "") : manual_user(user_id, contract_id, user_name, "super") {
        this->book = limit_order_book(contract_id);
        this->game = game;
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

class SuperGame {

    Game game;



};


