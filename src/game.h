#include <iostream>
#include <string>
#include <vector>
#include "user.h"
#include "matching.h"

class Game {

    limit_order_book book;
    std::vector<manual_user> user_list;
    std::vector<log_item> log_list;
    matching_engine matcher;

    int contract_id;
    std::string contract_name;

    public:
    Game(int n, int contract_id, std::string contract_name, std::string user_names[], matching_engine matcher) {
        this->contract_id = contract_id;
        this->contract_name = contract_name;
        book = limit_order_book(contract_id);
        user_list = std::vector<manual_user>();
        for (int i = 0; i < n; ++i) {
            user_list.push_back(manual_user(i, contract_id, user_names[i]));
        }
        log_list = std::vector<log_item>();
        this->matcher = matcher;
    }

    Game(int n, int contract_id, std::string contract_name, super_user superusers[], matching_engine matcher) {
        this->contract_id = contract_id;
        this->contract_name = contract_name;
        book = limit_order_book(contract_id);
        user_list = std::vector<manual_user>();
        for (int i = 0; i < n; ++i) {
            user_list.push_back(superusers[i]);
        }
        log_list = std::vector<log_item>();
        this->matcher = matcher;
    }

    void send_order(Order order) {
        std::vector<log_item> logs = matcher.engine_update(&book, &order);
        for (int i = 0; i < logs.size(); ++i) {
            log_list.push_back(logs[i]);
        }
        for (int i = 0; i < log_list.size(); ++i) {
            std::cout << log_list[i].to_str() << std::endl;
        }
        for (int i = 0; i < log_list.size(); ++i) {
            for (int j = 0; j < user_list.size(); ++j) {
                user_list[j].process_transaction(log_list[i]);
            }
        }
    }

    //TODO: add functionality for superusers - udpating their lob, handling their open orders, and calling handle_log for each user concurrently.

};
