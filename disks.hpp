///////////////////////////////////////////////////////////////////////////////
// disks.hpp
// Jorge & Jacqueline
// Project 1
// CPSC 335
// 20 February 2020
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(dark_count() > 0);

      for (size_t i = 1; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t dark_count() const {
    return total_count() / 2;
  }

  size_t light_count() const {
    return dark_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_alternating() const {
	for(size_t i = 0; i < _colors.size(); i++){
		if(i % 2 == 0){
			if(_colors[i] != DISK_LIGHT)
				return false;
		}
		else
			if(_colors[i] != DISK_DARK)
				return false;
	}
	return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the left (low indices) and all dark disks on the right (high
  // indices).
  bool is_sorted() const {
	for(size_t i = 0; i < _colors.size()/2; i++){
		if(_colors[i] != DISK_LIGHT)
			return false;
	}
   return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
	assert(before.is_alternating());  
	auto disk = before;  
	size_t swap = 0, run = 1, size = disk.total_count(), previous;
	while(run <= size){	
		previous = 0;
		for(size_t i = 1 ; i < size; i++){
			if(disk.get(previous) == DISK_DARK && disk.get(i) == DISK_LIGHT){
				disk.swap(previous);
				swap++;
			}
			previous = i;
		}
		run++;
	}
	return sorted_disks(disk, swap);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  // check that the input is in alternating format
	assert(before.is_alternating());
	auto disk = before;  
	size_t swap = 0, run = 1, size = disk.total_count(), previous;
	while(run <= (size/2)){	
		previous = 0;
		for(size_t i = 1 ; i < size; i++){
			if(disk.get(previous) == DISK_DARK && disk.get(i) == DISK_LIGHT){
				disk.swap(previous);
				swap++;
			}
			previous = i;
		}
		previous = (size - 1);
		for(size_t k = size - 2; k >= 1; k--){
			if(disk.get(previous) == DISK_LIGHT && disk.get(k) == DISK_DARK){
				disk.swap(k);
				swap++;
			}
			previous = k;
		}
		run++;
	}
  return sorted_disks(disk,swap);
}
