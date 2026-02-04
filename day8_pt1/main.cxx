#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ranges>
#include <string_view>

// https://en.cppreference.com/w/cpp/ranges/transform_view.html

int main(void){
  std::cout << "Day 8 part one" << std::endl;
  std::vector<std::tuple<int, int, int>> data;

  // read file
  std::string line;
  std::ifstream fileptr("input.txt");
  
  if (!fileptr.is_open()) {
    std::cerr << "ERR: couldn't open input.txt" << std::endl;
    return 1;
  }
  while(std::getline(fileptr, line)) {
    auto split = line 
      | std::views::split(',')
      | std::views::transform(
          [](auto&& str) {
            return std::string_view(&*str.begin(), std::ranges::distance(str)); 
            }
        );
    std::tuple<int, int, int> insertable = std::make_tuple(0, 0, 0);
    for(auto&& item : split) {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }
  fileptr.close();

  // add each triplet to vector;
  // make min heap
  // get lowest 1000 distances in min heap
  // connect all in min heap into unique sets
  // multiply the sizes of the three largest sets;
  return 0;
}
