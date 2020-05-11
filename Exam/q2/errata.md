Q2 T1.3
Inconsistnet defintion and declaration input member type;

In tensor_core.cpp
vector<size_t> offset_to_index(const vector<size_t> &size, int offset);

In tensor.hpp
vector<size_t> offset_to_index(const vector<size_t> &size, size_t offset);

I have chosen the later for consistency with index_to_offset;