#include <iostream>
#include "task1.h"
#include "task2.h"

int main() {
    //*************** task1 ******************

    //В торговый центр поставили кофе-автомат всего, в котором всего две порции кофе
    CoffeeAutomaton coffeeAutomaton("Inside Trade Center", 5.15f, 2);

    auto lambda { [&](){
        std::cout << "Automaton has got ingredients to make " <<
            coffeeAutomaton.getPortionsCount() << " cup(s) of coffee\n\n";
    }};

    //****  Первый стакан кофе ****

    //Автомат может сделать две порции
    lambda();

    coffeeAutomaton.insertCoin();
    coffeeAutomaton.getCoinBack();
    coffeeAutomaton.insertCoin();
    coffeeAutomaton.pressButton();

    //Осталась одна порция
    lambda();

    //****  Делаем второй (последний) стакан ****
    coffeeAutomaton.insertCoin();
    coffeeAutomaton.pressButton();

    //Всё, автомат пуст
    lambda();

    //**** Но мы попытаемся сделать ещё один стакан кофе.. ****
    coffeeAutomaton.insertCoin();
    coffeeAutomaton.pressButton();
    coffeeAutomaton.pressButton();
    coffeeAutomaton.pressButton();

    //Автомат пуст, добавим пятьдесят порций
    coffeeAutomaton.addProduct(std::string(), 50);

    //*************** task2 ******************

    Monster monster(100, 80, 50, "Undead");

    Monster* monster1 = monster.clone();

    monster1->showAttributes();

    delete monster1;

    return 0;
}