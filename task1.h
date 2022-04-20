#include <iostream>
#include <unistd.h> //sleep()

class ISellSystem{
public:
    virtual void changePrice(const std::string&, float) = 0;
    virtual void sellProduct(const std::string&) = 0;
    virtual void addProduct(const std::string&, size_t) = 0;
};

//Мы будем наследовать класс CoffeAutomaton сразу от
//интерфейса ISellSystem, поэтому закомментим эту часть ТЗ.
//(Справедливости ради отмечу, что можно наследоваться и от
//Automaton, но тогда производный класс будет содержать в себе
//два контейнера std::map, из которых будет использовать лишь по
//одной ячейке, что есть over-громоздко для него. Конечно, эти
//контейнеры могут быть занулены и незадействованы, но это также
//не есть красиво).
/*
class Automaton: ISellSystem {
private:
    std::string location;
    double received_money;
    double spent_money;
    std::map <std::string, float> goods_price;
    std::map <std::string, size_t> goods_count;

public:
    Automaton(const std::string &location, double receivedMoney) :
        location(location), received_money(receivedMoney) {
            spent_money = 0.0;
    }

    void changePrice(std::string name, float newPrice) override {
        goods_price[name] = newPrice;
    }

    void sellProduct(std::string name){
        if(goods_count[name] > 0){
            received_money += goods_price[name];
        }
        else{
            std::cerr << "Not enough goods" << std::endl;
        }
    }

    void addProduct(std::string name, size_t count){
        goods_count[name] += count;
    }

    double allMoneyReport() const{
        return received_money - spent_money;
    }

    const std::string& getLocationReport() const {
        return location;
    }

    std::map <std::string, size_t> goodsCountReport(){
        return goods_count;
    }

    std::map <std::string, float> goodsPriceReport(){
        return goods_price;
    }
};
*/

class CoffeeAutomaton;

class State{
protected:
    std::string _description;

    State(const std::string& description): _description{ description } {}

public:
    const std::string getStateDescription() const{
        return _description;
    }

    virtual void insertCoin(CoffeeAutomaton*) {
        std::cout << "You are not able to do this.\n";
    }

    virtual void getCoinBack(CoffeeAutomaton*) {
        std::cout << "You tried got money from automaton but failed.\n";
    }

    virtual void pressButton(CoffeeAutomaton*) {
        std::cout << "Pressing the button does nothing.\n";
    }

    virtual void addProduct(CoffeeAutomaton*, size_t);

    virtual ~State(){}
};

class EmptyState: public State{
public:
    EmptyState(): State("Empty coffee container") {}

    void addProduct(CoffeeAutomaton*, size_t) override;
};

class NoCoinState: public State{
public:
    NoCoinState(): State("No coin") {}

    void insertCoin(CoffeeAutomaton*) override;
};

class HasCoinState: public State{
public:
    HasCoinState(): State("Got a coin") {}

    void getCoinBack(CoffeeAutomaton*) override;
    void pressButton(CoffeeAutomaton*) override;
};

class CoffeeSoldState: public State{
public:
    CoffeeSoldState(): State("Cup of coffee has been sold") {}
};

class CoffeeAutomaton: public ISellSystem{
private:
    std::string _location;
    float _price;
    float _receivedMoney;
    size_t _portionsCount;
    State *_state;
public:
    CoffeeAutomaton(const std::string &location, float price, size_t portions = 0):
        _location{ location },
        _price{ price },
        _receivedMoney{},
        _portionsCount{ portions },
        _state{ nullptr }
        {
            if (_portionsCount == 0)
                _state = new EmptyState;
            else
                _state = new NoCoinState;
        }

    void changePrice(const std::string& /* productName */, float newPrice) override{
        _price  = newPrice;
    }

    void sellProduct(const std::string& /* productName */) override{
        _portionsCount--;
        _receivedMoney += _price;
    }

    void addProduct(const std::string& /* productName */, size_t count) override{
        _state->addProduct(this, count);
    }

    void setPortionsCount(size_t count) {
        _portionsCount = count;
    }

    size_t getPortionsCount() const{
        return _portionsCount;
    }

    void setState(State* state){
        std::cout << "State changed from \'" << _state->getStateDescription()
        << "\' to \'" << state->getStateDescription() << "\'\n\n";

        delete _state;

        _state = state;
    }

    void insertCoin() { _state->insertCoin(this); }

    void getCoinBack() { _state->getCoinBack(this); }

    void pressButton() { _state->pressButton(this); }

    ~CoffeeAutomaton(){
        if (_state)
            delete _state;
    }
};

void State::addProduct(CoffeeAutomaton *coffeeAutomaton, size_t count) {
    coffeeAutomaton->setPortionsCount(coffeeAutomaton->getPortionsCount() + count);
    std::cout << "\nAdded " << count << " ingredients.\n";
    std::cout << "Automation can make " << coffeeAutomaton->getPortionsCount()
        << " cups of coffee now.\n\n";
}

void EmptyState::addProduct(CoffeeAutomaton* coffeeAutomaton, size_t count) {
    State::addProduct(coffeeAutomaton, count);

    //Вдруг юзер добавил count = 0 и при этом _portionsCount был равен 0;
    //если это так, то состояние не меняем
    if (coffeeAutomaton->getPortionsCount() > 0)
        coffeeAutomaton->setState(new NoCoinState);
}

void NoCoinState::insertCoin(CoffeeAutomaton* coffeeAutomaton) {
    std::cout << "Coin has been inserted.\n";
    coffeeAutomaton->setState(new HasCoinState());
}

void HasCoinState::getCoinBack(CoffeeAutomaton *coffeeAutomaton) {
    std::cout << "An inserted coin has been got out.\n";
    coffeeAutomaton->setState(new NoCoinState());
}

void HasCoinState::pressButton(CoffeeAutomaton *coffeeAutomaton) {

    if (coffeeAutomaton->getPortionsCount() > 0)
    {
        coffeeAutomaton->sellProduct(std::string());
        std::cout << "Button pressed. Automaton starts making coffee...\n";
        coffeeAutomaton->setState(new CoffeeSoldState());
        sleep(2);

        std::cout << "Coffee is ready, enjoy it!\n";
        coffeeAutomaton->setState(new NoCoinState);
    }
    else
    {
        std::cout << "Coffee machine is empty. Call service.\n";
        getCoinBack(coffeeAutomaton);
        coffeeAutomaton->setState(new EmptyState);
    }
}
