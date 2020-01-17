#pragma once
#include<iostream>

/*класс*/
template<typename T>
class vector_t {
	class sparsematrix_t;
public:
	vector_t(const int size_) : size(size_) {
		this->entities = new double[size];
		for (unsigned i = 0; i < this->size; i++)
			entities[i] = 1;
	}
	vector_t() : size(0) {
		this->entities = nullptr;
	}
	
	T & operator[](int c) {
		return entities[c];
	}
	unsigned GetSize() {
		return size;
	}


	template<typename TT>
	friend 	std::ostream & operator<<(std::ostream & os, const vector_t<TT>& v);

private:
	unsigned size{ 0 };
	T* entities;
};

template<typename TT>
 	std::ostream & operator<<(std::ostream & os, const vector_t<TT>& v) {
	for (int i = 0; i < v.size; i++)
		os << v.entities[i] << " ";
	os << std::endl;
	return os;
}
