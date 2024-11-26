#include <iostream>
#include <vector>
#include <string>
#include "../src/game.h"

int main() {

    std::cout << "Starting Game Test" << std::endl;
    std::string user_names[] = {"Jeff", "Freddie"};
    price_time_priority ptp = price_time_priority();
    Game game = Game(2, 0, "TSLA", user_names, ptp, 185);

    game.send_order(LimitOrder(1, 0, 0, 175, 10000, BUY));



}
