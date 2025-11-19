#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>

class Item {
public:
    Item(std::string name, double weight) 
        : name_(name), weight_(weight) {}

    virtual ~Item() {}

    Item(const Item& rhs) 
        : name_(rhs.name_), weight_(rhs.weight_) 
    {
        std::cout << "[Log] Copy Ctor Item (Baza): " << name_ << "\n";
    }

    Item& operator=(const Item& rhs) {
        if (this == &rhs) return *this;
        name_ = rhs.name_;
        weight_ = rhs.weight_;
        return *this;
    }

    void printBase() const {
        std::cout << "[" << name_ << " | Weight: " << weight_ << "]";
    }

protected:
    std::string name_;
    double weight_;
};

class MagicWeapon : public Item {
public:
    MagicWeapon(std::string name, double weight, int damage, int durability, const char* effect)
        : Item(name, weight),
          damage_(damage), durability_(durability)
    {
        magicEffect_ = new char[std::strlen(effect) + 1];
        std::strcpy(magicEffect_, effect);
    }

    ~MagicWeapon() {
        delete[] magicEffect_;
    }

    MagicWeapon(const MagicWeapon& rhs)
        : Item(rhs),
          damage_(rhs.damage_),
          durability_(rhs.durability_)
    {
        std::cout << "  [Log] MagicWeapon Copy Ctor (Derived)\n";

        magicEffect_ = new char[std::strlen(rhs.magicEffect_) + 1];
        std::strcpy(magicEffect_, rhs.magicEffect_);
    }

    friend void swap(MagicWeapon& first, MagicWeapon& second) noexcept {
        using std::swap; 

        swap(static_cast<Item&>(first), static_cast<Item&>(second));

        swap(first.damage_, second.damage_);
        swap(first.durability_, second.durability_);

        swap(first.magicEffect_, second.magicEffect_);
    }

    MagicWeapon& operator=(MagicWeapon rhs) 
    {
        std::cout << "  [Log] Operator= (Copy-and-Swap) called\n";
        
        swap(*this, rhs);

        return *this;
    }

    void printFull() const {
        printBase(); // Prints Name and Weight
        std::cout << " Stats: [DMG: " << damage_ << " | DUR: " << durability_
                  << " | Effect: " << magicEffect_ << "]\n";
    }

private:
    int damage_;
    int durability_;
    char* magicEffect_;
};

class Character {
public:
    MagicWeapon equippedWeapon_;

    Character(const MagicWeapon& weapon) : equippedWeapon_(weapon) {}

    void equipWeapon(const MagicWeapon& weapon) {
        equippedWeapon_ = weapon;
    }

    void showGear() const {
        std::cout << "\nCharacter Gear: "; equippedWeapon_.printFull();
    }
};

class DualWieldCharacter {
public:
    MagicWeapon leftHandWeapon_;
    MagicWeapon rightHandWeapon_;

    DualWieldCharacter(const MagicWeapon& leftWeapon, const MagicWeapon& rightWeapon)
        : leftHandWeapon_(leftWeapon), rightHandWeapon_(rightWeapon) {}

    void equipLeftWeapon(const MagicWeapon& weapon) {
        leftHandWeapon_ = weapon;
    }

    void equipRightWeapon(const MagicWeapon& weapon) {
        rightHandWeapon_ = weapon;
    }

    void equipSameWeapon(MagicWeapon& weapon) {
        std::cout << "\nAction: Dual Wielder equipping SAME weapon in both hands...\n";
        leftHandWeapon_ = rightHandWeapon_ = weapon;
    }

    void showGear() const {
        std::cout << "Left Hand:  "; leftHandWeapon_.printFull();
        std::cout << "Right Hand: "; rightHandWeapon_.printFull();
    }
};

int main() {
    MagicWeapon excalibur("Excalibur", 12.5, 100, 500, "Holy Light");
    MagicWeapon rustyDagger("Rusty Dagger", 2.0, 5, 20, "Tetanus");
    MagicWeapon voidBlade("Void Blade", 8.0, 999, 100, "Darkness");

    std::cout << "========================================\n";
    std::cout << "TEST 1: Item 10 (Chained Assignment a=b=c)\n";
    std::cout << "========================================\n";
    
    DualWieldCharacter rogue(rustyDagger, rustyDagger);
    rogue.showGear();

    rogue.equipSameWeapon(voidBlade); 

    std::cout << "\n--- After Chained Assignment ---\n";
    rogue.showGear(); 

    std::cout << "\n========================================\n";
    std::cout << "TEST 2: Item 11 (Self-Assignment a=a)\n";
    std::cout << "========================================\n";

    Character knight(excalibur);
    std::cout << "\n--- Initial State ---\n";
    knight.showGear();

    knight.equipWeapon(knight.equippedWeapon_);

    std::cout << "\n--- After Self-Assignment ---\n";
    knight.showGear();

    std::cout << "\n========================================\n";
    std::cout << "TEST 3: Item 12 (Copy All Parts)\n";
    std::cout << "========================================\n";

    knight.equipWeapon(voidBlade);
    std::cout << "\n--- After Equipping New Weapon ---\n";
    knight.showGear();

    return 0;
}