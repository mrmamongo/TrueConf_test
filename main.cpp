#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <set>

using container_map = std::map<std::string, std::multiset<char>>;
using container_pair = std::pair<std::string, std::multiset<char>>;


const char DELIMITER = ' ';
const std::string BAD_VALUE = "BAD";

std::string sort_string(const std::string& unsorted) {
    std::string sorted = unsorted;
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

struct compare_keys {
    const container_pair& pair_to_compare;
    explicit compare_keys(const container_pair& pair): pair_to_compare(pair){}
    std::string comparison_string_sorted = sort_string(pair_to_compare.first);
    bool operator()(const container_pair& second_pair) const {
        std::string right_sorted = sort_string(second_pair.first);
        return comparison_string_sorted == right_sorted;
    }
};

std::multiset<char> from_string(const std::string& s) {
    std::multiset<char> out;
    for (auto c : s) {
        out.insert(c);
    }
    return out;
}

std::string to_string(const std::multiset<char>& in_set) {
    std::string out;
    for (auto c : in_set){
        out.push_back(c);
    }
    return out;
}

void insert(container_map& map, const container_pair& key_value_pair) {
    auto found_item = std::find_if(std::begin(map), std::end(map), compare_keys(key_value_pair));

    if (found_item == map.end()) {
        map.insert(key_value_pair);
        return;
    }

    std::cout << "Found match in " << key_value_pair.first << ": " << to_string(key_value_pair.second) << '\n';
    map[found_item->first].insert(key_value_pair.second.begin(), key_value_pair.second.end());
}

void show_map(const container_map& map){
    std::cout << "Map output:\n";
    for (const auto& val : map) {
        std::cout << val.first << ": " << to_string(val.second) << '\n';
    }
}

container_pair split_string(std::string string_to_split){
    size_t pos = 0;
    std::string key;
    if ((pos = string_to_split.find(DELIMITER)) == std::string::npos)
        return std::make_pair(BAD_VALUE, from_string(BAD_VALUE));

    key = string_to_split.substr(0, pos);
    // string_to_split - value
    string_to_split.erase(0, pos + 1); // pos + 1 (delimiter length)
    return std::make_pair(key, from_string(string_to_split));
}


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Error: Incorrect CLI args\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) {
        std::cout << "Error: File is not opened\n";
        return 1;
    }

    container_map container;

    std::cout << "Input file:\n";
    while(getline(file, line, '\n')) {
        insert(container, split_string(line));
        std::cout << line << '\n';
    }

    show_map(container);
    return 0;
}
