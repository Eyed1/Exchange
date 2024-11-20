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

    Order(int order_id, int user_id, int contract_id, std::string order_type); 
    Order();
    std::string get_order_type(); 
    int get_timestamp();

    virtual ~Order() {}
};

class LimitOrder : public Order {
    public:
    int price;
    int quantity;
    Side side;

    LimitOrder(int order_id, int user_id, int contract_id, int price, int quantity, Side side);
    int get_price();
};

class MarketOrder : public Order {
    public:
    int quantity;
    Side side;

    MarketOrder(int order_id, int user_id, int contract_id, int quantity, Side side);
    int get_price();
};

class CancelOrder : public Order {
    int cancel_order_id;
    public:
    CancelOrder(int order_id, int user_id, int contract_id, int cancel_order_id);
};
