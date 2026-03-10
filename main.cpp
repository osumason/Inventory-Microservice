//inventorymicroservice.cpp
//maintains a list of items in inventory.txt as comma delimited key-value pairs for easy parsing by the user.

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unistd.h>


std::string get_storage_path(std::string filename) { // sets filepath to the user's documents folder
    const char *drive = std::getenv("HOMEDRIVE");
    const char *path = std::getenv("HOMEPATH");
    if (!drive || !path) return filename;
    return std::string(drive) + std::string(path) + "\\Documents\\" + filename;
}


std::map<std::string, int> load_inventory() {// loads the map from inventory.txt
    std::map<std::string, int> inv;
    std::ifstream file(get_storage_path("inventory.txt"));
    std::string item;
    int qty;
    while (file >> item >> qty) {
        inv[item] = qty;
    }
    file.close();
    return inv;
}

void save_inventory(const std::map<std::string, int>& inv) {// saves the map to inventory.txt
    std::ofstream file(get_storage_path("inventory.txt"));
    for (auto const& [item, qty] : inv) {
        file << item << " " << qty << '\n';
    }
    file.close();
}

void initialize() {// Creates files if they don't exist and initializes invcommand.txt with a value
    std::ofstream inv(get_storage_path("inventory.txt"));
    inv.close();

    std::ofstream cmd(get_storage_path("invcommand.txt"));
    cmd << "running";
    cmd.close();
}

void update_inventory(std::string item, char op, int val) { //adds or removes items from inventory.txt
    auto inv = load_inventory();

    if (op == '+') {
        inv[item] += val;
    } else if (op == '-') {
        inv[item] -= val;
        if (inv[item] <= 0) inv.erase(item); // Remove if empty
    } else if (op == '=') {
        inv[item] = val;
    }

    save_inventory(inv);
}

std::string display () //returns inventory as string to invcommand.txt
{
    auto inv = load_inventory();
    if (inv.empty()) return "EMPTY";

    std::string result = "";
    for (auto const& [item, qty] : inv) {
        result += item + ":" + std::to_string(qty) + ",";
    }
    return result;
}

void process() { //constantly checking invcommand.txt for directions

    std::string check;

    std::cout << "listening..." << '\n';

    try {
        std::ifstream order(get_storage_path("invcommand.txt"));

        if (!order.is_open()){
            return;
        }

        if (!(order >> check)) {
            order.close();
            return;
        }

        order.close();

        if (check == "running") return;

        if (check == "read") {
            std::ofstream output(get_storage_path("invcommand.txt"));
            output << display();
            output.close();
        } else {
            // Parse: ItemName+Quantity (e.g., "HealthPotion+5")
            size_t op_pos = check.find_first_of("+-=");
            if (op_pos != std::string::npos) {
                std::string item = check.substr(0, op_pos);
                char op = check[op_pos];
                int val = std::stoi(check.substr(op_pos + 1));

                update_inventory(item, op, val);

                // Reset to acknowledge
                std::ofstream reset(get_storage_path("invcommand.txt"));
                reset << "running";
                reset.close();
            }
        }
    } catch (const std::exception& e) {
        std::cout << "[Error] Parsing failed: " << e.what() << '\n';
    }
}

int main() {

    initialize();

    while (true) {
        process();
        sleep(1);
    }


    return 0;
}