class Unit{
public:
    Unit() {}
    virtual ~Unit(){}
    virtual Unit* clone() const = 0;
};
class Monster: public Unit{
private:
    int hp;
    int atk;
    int def;
    std::string type;
public:
    Monster(int hp, int atk, int def, const std::string &type) : hp(hp), atk(atk), def(def), type(type) {}

    void changeHp(int change){
        hp += change;
    }

    void changeAtk(int change){
        atk += change;
    }

    void changeDef(int change){
        def += change;
    }

    int getHp() const {
        return hp;
    }

    int getAtk() const {
        return atk;
    }

    int getDef() const {
        return def;
    }

    Monster* clone() const override{
        return new Monster(*this);
    }

    void showAttributes() const{
        std::cout << "Type:          " << type << std::endl;
        std::cout << "Health Points: " << hp << std::endl;
        std::cout << "Attack:        " << atk << std::endl;
        std::cout << "Defense:       " << def << std::endl;
    }
};