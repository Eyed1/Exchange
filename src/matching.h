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

class log_item {
    // Class for logs of transactions
    public:
        int success;
        std::string message;
        int buyer_id;
        int seller_id;
        int price;
        int quantity;
        int timestamp;

        log_item(int success, std::string message, int buyer_id, int seller_id, int price, int quantity) {
            this->success = success;
            this->message = message;
            this->buyer_id = buyer_id;
            this->seller_id = seller_id;
            this->price = price;
            this->quantity = quantity;
            this->timestamp = time(0);
        }
};

class price_time_priority : public matching_engine {
    public:
    price_time_priority() : matching_engine("price_time_priority") {}

    std::vector<log_item> engine_update(limit_order_book *book, Order order) {
        if (order.order_type == "CANCEL") {
            (*book).cancel_order(order.order_id);
            std::vector<log_item> ret_val;
            ret_val.push_back(log_item(1, "CancelOrder", order.user_id, order.user_id, -1, -1));
            return ret_val;
        }
        if (order.order_type == "LIMIT") {
            LimitOrder lim_order = *dynamic_cast<LimitOrder*>(&order);
            if (lim_order.side == BUY) {
                std::vector<log_item> transactions;
                int cur_total_quantity = lim_order.quantity;
                while (cur_total_quantity > 0 & (*book).get_num_asks() > 0) {
                    LimitOrder best_ask = (*book).get_best_ask_order();
                    if (best_ask.price > lim_order.price) {
                        break;
                    }
                    if (best_ask.quantity > cur_total_quantity) {
                        transactions.push_back(log_item(1, "LimitOrder", lim_order.user_id, best_ask.user_id, best_ask.price, cur_total_quantity));
                        int num_left = best_ask.quantity - cur_total_quantity;
                        cur_total_quantity = 0;
                        (*book).update_order(lim_order.order_id, num_left);
                    } else {
                        transactions.push_back(log_item(1, "LimitOrder", lim_order.user_id, best_ask.user_id, best_ask.price, best_ask.quantity));
                        cur_total_quantity -= best_ask.quantity;
                        (*book).cancel_order(best_ask.order_id);
                    }
                }
                return transactions;
            }
            else if (lim_order.side == SELL) {
                std::vector<log_item> transactions;
                int cur_total_quantity = lim_order.quantity;
                while (cur_total_quantity > 0 & (*book).get_num_bids() > 0) {
                    LimitOrder best_bid = (*book).get_best_bid_order();
                    if (best_bid.price < lim_order.price) {
                        break;
                    }
                    if (best_bid.quantity > cur_total_quantity) {
                        transactions.push_back(log_item(1, "LimitOrder", best_bid.user_id, lim_order.user_id, best_bid.price, cur_total_quantity));
                        int num_left = best_bid.quantity - cur_total_quantity;
                        cur_total_quantity = 0;
                        (*book).update_order(lim_order.order_id, num_left);
                    } else {
                        transactions.push_back(log_item(1, "LimitOrder", best_bid.user_id, lim_order.user_id, best_bid.price, best_bid.quantity));
                        cur_total_quantity -= best_bid.quantity;
                        (*book).cancel_order(best_bid.order_id);
                    }
                }
                return transactions;
            }
            return std::vector<log_item>();
        }
        if (order.order_type == "MARKET") {
            MarketOrder market_order = *dynamic_cast<MarketOrder*>(&order);
            std::vector<log_item> transactions;
            if (market_order.side == BUY) {
                int cur_total_quantity = market_order.quantity;
                while (cur_total_quantity > 0 & (*book).get_num_asks() > 0) {
                    LimitOrder best_ask = (*book).get_best_ask_order();
                    if (best_ask.quantity > cur_total_quantity) {
                        transactions.push_back(log_item(1, "MarketOrder", market_order.user_id, best_ask.user_id, best_ask.price, cur_total_quantity));
                        int num_left = best_ask.quantity - cur_total_quantity;
                        cur_total_quantity = 0;
                        (*book).update_order(best_ask.order_id, num_left);
                    } else {
                        transactions.push_back(log_item(1, "MarketOrder", market_order.user_id, best_ask.user_id, best_ask.price, best_ask.quantity));
                        cur_total_quantity -= best_ask.quantity;
                        (*book).cancel_order(best_ask.order_id);
                    }
                }
            }
            else if (market_order.side == SELL) {
                int cur_total_quantity = market_order.quantity;
                while (cur_total_quantity > 0 & (*book).get_num_bids() > 0) {
                    LimitOrder best_bid = (*book).get_best_bid_order();
                    if (best_bid.quantity > cur_total_quantity) {
                        transactions.push_back(log_item(1, "MarketOrder", best_bid.user_id, market_order.user_id, best_bid.price, cur_total_quantity));
                        int num_left = best_bid.quantity - cur_total_quantity;
                        cur_total_quantity = 0;
                        (*book).update_order(best_bid.order_id, num_left);
                    } else {
                        transactions.push_back(log_item(1, "MarketOrder", best_bid.user_id, market_order.user_id, best_bid.price, best_bid.quantity));
                        cur_total_quantity -= best_bid.quantity;
                        (*book).cancel_order(best_bid.order_id);
                    }
                }
            }
            return transactions;
        }
        return std::vector<log_item>();
    }
};
