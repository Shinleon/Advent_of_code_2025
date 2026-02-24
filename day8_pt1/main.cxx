#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <functional>
#include <queue>
#include <set>
#include <memory>

// https://en.cppreference.com/w/cpp/ranges/transform_view.html

class DistancePair {
private:
  int first;
  int second;
  double distance;

public:
  
  DistancePair(int f, int s, double d) : first(f), second(s), distance(d) {}

  int getFirst() const {return first;}
  int getSecond() const {return second;}
  double getDistance() const {return distance;}

  bool operator<(const DistancePair& o) const {
    return this->distance < o.distance;
  }
  
};

int main(void){
  std::vector<std::tuple<int, int, int>> data;

  // read file
  std::ifstream fileptr("input.txt");
  
  if (!fileptr.is_open()) {
    std::cerr << "ERR: couldn't open input.txt" << std::endl;
    return 1;
  }
 // add each triplet to vector;
  char c;
  int storage[3] = {0};
  int count = 0;
  while(fileptr.get(c)) {
    if (c >= '0' && c <= '9'){
      // we have a digit we need to store;
      storage[count] = storage[count]*10 + (int)(c - '0');
    } else if (c == ','){
      // we can increment the tuple_index
      count++;
    } else if (c == '\n'){
      // we can set tupe_index to zero and 
      // store the previously created tuple
      std::tuple<int, int, int> _t  = std::make_tuple(storage[0], storage[1], storage[2]);
      data.push_back(_t);
      storage[0] = 0;
      storage[1] = 0;
      storage[2] = 0;
      count = 0;
    }
  }

  fileptr.close();

  // make max heap
  // get lowest 1000 distances in max heap
  count = 0;
  std::priority_queue<DistancePair> maxHeap;
  for(int i = 0; i < data.size(); i++){
    for(int j = i+1; j < data.size(); j++){
      std::tuple<int,int,int> f = data[i];
      std::tuple<int,int,int> s = data[j];
      double distance = sqrt(pow((std::get<0>(f)-std::get<0>(s)), 2) + pow((std::get<1>(f)-std::get<1>(s)), 2) + pow((std::get<2>(f)-std::get<2>(s)), 2));
      maxHeap.push(DistancePair(i, j, distance));
      count++;
      while(maxHeap.size() > 1000){
        maxHeap.pop();
        count--;
      }
    }
  }

  std::priority_queue<DistancePair> minHeap;
  count = 0;
  while(maxHeap.size() > 0){
    const DistancePair& p = maxHeap.top();
    minHeap.push(DistancePair(p.getFirst(), p.getSecond(), p.getDistance()));
    maxHeap.pop();
    count++;
  }

  // connect all in min heap into unique sets
  // multiply the sizes of the three largest sets;
  std::vector<std::shared_ptr<std::set<int>>> clusters;
  while(minHeap.size() > 0) {
    const DistancePair& p = minHeap.top();
    int f_set_index = -1;
    int s_set_index = -1;
    // loop backwards through the array and move f_set and s_set out of the vector
    // combine f_set and s_set if necessary
    // reappend f_set and move back to the vector
    for(int i = 0; i < clusters.size(); i++){
      if (clusters[i]->contains(p.getFirst()) || clusters[i]->contains(p.getSecond())) {
        if(f_set_index == -1){
          f_set_index = i;
        } else {
          s_set_index = i;
          break;
        }
      }
    }
    if(f_set_index != -1){
      if(s_set_index != -1 ) {
        clusters[f_set_index]->insert(clusters[s_set_index]->begin(), clusters[s_set_index]->end());
        clusters.erase(clusters.begin() + s_set_index);
      }
      clusters[f_set_index]->insert(p.getFirst());
      clusters[f_set_index]->insert(p.getSecond());
    } else if(f_set_index == -1 && s_set_index == -1) {
      std::shared_ptr<std::set<int>> pushable = std::make_shared<std::set<int>>();
      pushable->insert(p.getFirst());
      pushable->insert(p.getSecond());
      clusters.push_back(std::move(pushable)); //Need std::move if pushable is a unique_ptr, why?
    }
    minHeap.pop();
  }
  
  for(int i = 0; i < clusters.size(); i++) {
    std::cout << "size: " << clusters[i]->size() << std::endl;
    std::cout << "\t";
    for(auto j: *clusters[i]){
      std::cout << j << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}
