#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "user.h"
#include "matching.h"

class Game {

    public:

    limit_order_book book;
    std::vector<manual_user> user_list;
    std::vector<log_item> log_list;
    matching_engine matcher;
    log_item last_trade;

    std::vector<std::thread> user_threads;
    std::mutex game_lock;

    int contract_id;
    std::string contract_name;

    Game() {}

    Game(int n, int contract_id, std::string contract_name, std::string user_names[], matching_engine matcher, int starting_price) {
        this->contract_id = contract_id;
        this->contract_name = contract_name;
        book = limit_order_book(contract_id);
        user_list = std::vector<manual_user>();
        for (int i = 0; i < n; ++i) {
            user_list.push_back(manual_user(i, contract_id, user_names[i]));
            std::thread thread_obj(&manual_user::update_lob, &user_list[i], book);
            user_threads.push_back(thread_obj);
        }
        log_list = std::vector<log_item>();
        this->matcher = matcher;
        last_trade = log_item(1, "Trade", -1, -1, starting_price, 0);
    }

    /*Game(int n, int contract_id, std::string contract_name, super_user superusers[], matching_engine matcher, int starting_price) {
        this->contract_id = contract_id;
        this->contract_name = contract_name;
        book = limit_order_book(contract_id);
        user_list = std::vector<manual_user>();
        for (int i = 0; i < n; ++i) {
            user_list.push_back(superusers[i]);
        }
        log_list = std::vector<log_item>();
        this->matcher = matcher;
        last_trade = log_item(1, "Trade", -1, -1, starting_price, 0);
    }*/

    void send_order(Order order) {
        std::vector<log_item> logs = matcher.engine_update(&book, &order);
        for (int i = 0; i < logs.size(); ++i) {
            log_list.push_back(logs[i]);
            if (logs[i].message == "Trade") {
                last_trade = logs[i];
            }
        }
        game_lock.unlock();
        for (int i = 0; i < log_list.size(); ++i) {
            std::cout << log_list[i].to_str() << std::endl;
        }
        for (int j = 0; j < user_list.size(); ++j) {
            user_list[j].update_lob(book);
        }
        for (int i = 0; i < log_list.size(); ++i) {
            for (int j = 0; j < user_list.size(); ++j) {
                user_list[j].process_transaction(log_list[i]);
            }
        }
        
        for (int i = 0; i < log_list.size(); ++i) {
            std::vector<std::thread> new_user_threads;
            for (int j = 0; j < user_list.size(); ++j) {
                std::thread thread_obj(&manual_user::handle_log, &user_list[j], log_list[log_list.size()-1]);
                new_user_threads.push_back(thread_obj);
            }
            for (int j = 0; j < new_user_threads.size(); ++j) {
                new_user_threads[j].join();
            }
        }
    }

    void print_stats() {
        std::cout << "Num Trades: " << book.get_num_orders() << std::endl;
        for (int i = 0; i < user_list.size(); ++i) {
            std::cout << "User " << user_list[i].get_user_id() << " Position: " << user_list[i].get_position() << " PnL: " << user_list[i].get_pnl(last_trade.price) << std::endl;
        }
    }

    /*void flush_user_order_list(int user_id) {
        std::vector<Order> order_list = user_list[user_id].get_order_list();
        for (int i = 0; i < order_list.size(); ++i) {
            send_order(order_list[i]);
        }
    }*/

    //TODO: add functionality for superusers - udpating their lob, handling their open orders, and calling handle_log for each user concurrently.
    //static void send_order_to_game(Order order ) { logic };
    //

};
