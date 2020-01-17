#pragma once

#include <iostream>
#include<fstream>
#include<string>
#include "vector_t.h"


using namespace std;

template<typename T>
struct  entry_t {
	int column;
	T coeff;
	entry_t* next;

	entry_t() : column(0), coeff(0) {
		next = nullptr;
	}

	template<typename T>
	T & operator[](int c) {
		entry_t* row = this;
		T s = T();
		while (row != NULL)
		{
			if (c < row->column)
				return s;
			if (c == row->column)
				return row->coeff;
			row = row->next;
		}
		return s;
	}
};

template<class T>
class sparsematrix_t
{
public:
	sparsematrix_t() = default;

	sparsematrix_t(const unsigned _rows, const unsigned _cols) {
		entities = nullptr;
	};

	sparsematrix_t(const sparsematrix_t<T>& m) : rows(m.rows), cols(m.cols)
	{
		this->entities = new entry_t<T>[m.rows];
		for (unsigned i = 0; i < this->rows; i++) {
			entry_t<T> * temp = &this->entities[i];
			entry_t<T> *val = &m.entities[i];
			while (val->next != NULL)
			{
				temp->column = val->column;
				temp->coeff = val->coeff;
				if (val->next != NULL)
				{
					temp->next = new entry_t<T>;
					val = val->next;
				}
				temp = temp->next;
			}
		}
	};

	T getCols() {
		return this->cols;
	}

	T getRows() {
		return this->row;
	}


	sparsematrix_t & operator+=(sparsematrix_t<T> & m)
	{
		for (unsigned i = 0; i < this->rows; i++)
			for (unsigned j = 0; j < this->cols; j++)
				this->set(this->operator[](i).operator[](j) + m.operator[](i).operator[](j), i, j);

		return *this;
	};

	sparsematrix_t<T> & operator-=(sparsematrix_t<T> & m)
	{
		for (unsigned i = 0; i < this->rows; i++)
			for (unsigned j = 0; j < this->cols; j++)
				this->set(this->operator[](i).operator[]<T>(j) - m.operator[](i).operator[]<T>(j), i, j);
		return *this;
	};

	void  WriteMatrixInFile(const string& path)
	{

		ofstream fileMatrix(path);
		fileMatrix << "%% \n";

		fileMatrix << this->rows << " " << this->cols << "\n";

		for (unsigned i = 0; i < this->rows; i++)
		{
			entry_t *tmp = &this->entities[i];
			while (tmp != NULL)
			{
				fileMatrix << i + 1 << " " << tmp->column + 1 << " " << tmp->coeff << "\n";
				tmp = tmp->next;
			}
		}
		fileMatrix.close();
	};
	sparsematrix_t<T> & readMatrixFromFile(string path) {
		entry_t<T>* entry = nullptr;

		ifstream fileMatrix(path);

		string com;
		getline(fileMatrix, com);

		int rows, cols;
		fileMatrix >> rows >> cols;
		this->rows = rows;
		this->cols = cols;
		this->entities = new entry_t<T>[rows];

		int row, col, val, count = -1;
		while (fileMatrix >> row >> col >> val) {
			row--; col--;
			if (count != row) {
				entry = &this->entities[row];
				entry->column = col; entry->coeff = val; entry->next = new entry_t<T>;
			}
			else {
				entry = entry->next;
				entry->column = col; entry->coeff = val; entry->next = new entry_t<T>;
			}
			count = row;
		}
		return *this;
	};

	entry_t<T> & operator[](int r)
	{
		return entities[r];
	};

	sparsematrix_t & operator*=(sparsematrix_t<T> &m) {
		T temp;
		for (unsigned i = 0; i < this->rows; i++) {
			temp = 0;
			for (unsigned j = 0; j < this->cols; j++) {
				temp = 0;
				for (unsigned k = 0; k < this->cols; k++) {
					temp += this->operator[](i).operator[]<T>(k) * m.operator[](k).operator[]<T>(j);
					this->set(temp, i, j);
				}
			}
		}
			return *this;
	};
	sparsematrix_t & operator/=(sparsematrix_t<T> &m) {
		T temp;
		for (unsigned i = 0; i < this->rows; i++) {
			temp = 0;
			for (unsigned j = 0; j < this->cols; j++) {
				temp = 0;
				for (unsigned k = 0; k < this->cols; k++) {
					temp += this->operator[](i).operator[]<T>(k) / m.operator[](k).operator[]<T>(j);
					this->set(temp, i, j);
				}
			}
		}
		return *this;
	};

	sparsematrix_t & operator*(T a) {
		for (int i = 0; i < this->rows; i++) {
			for (int j = 0; j < this->cols; j++) {
				this->operator[](i).template operator[]<T>(j) *= a;
			}
		}
		return *this;
	};

	sparsematrix_t & operator=(const sparsematrix_t<T> & m)
	{
		this->entities = new entry_t<T>[m.rows];
		for (unsigned i = 0; i < this->rows; i++)
			this->entities[i] = m.entities[i];
		return *this;
	};

	sparsematrix_t & set(const T val, const unsigned r, const unsigned c) {
		entry_t<T>* head = &this->entities[r];

		if (c < head->column) {
			entry_t<T>* tmp = new entry_t<T>;

			tmp->coeff = head->coeff;
			tmp->column = head->column;
			tmp->next = head->next;

			head->coeff = val;
			head->column = c;
			head->next = tmp;
		}
		else
		{
			while (head->next != NULL)
			{
				if (c == head->column)
				{
					head->coeff = val;
					break;
				}
				if (head->column < c && c < head->next->column) {
					entry_t<T>* temp = new entry_t<T>;

					temp->coeff = val;
					temp->column = c;
					temp->next = head->next;

					head->next = temp;

					break;
				}
				if (c > head->next->column)
				{
					if (head->next->column == 0) {
						entry_t<T>* temp = new entry_t<T>;

						temp->coeff = val;
						temp->column = c;
						temp->next = NULL;
						head->next = temp;
						break;
					}
					head = head->next;
				}
				head = head->next;
			}
		}

		return *this;
	};


	template<typename TT>
	friend ostream & operator<<(ostream & os, const sparsematrix_t<TT> S);

	template<typename TT>
	friend sparsematrix_t<TT>  operator+(sparsematrix_t<TT> lhs, sparsematrix_t<TT> rhs);

	template<typename TT>
	friend sparsematrix_t<TT>  operator-(sparsematrix_t<TT> lhs, sparsematrix_t<TT> rhs);

	template<typename TT>
	friend sparsematrix_t<TT>  operator/(sparsematrix_t<TT> lhs, sparsematrix_t<TT> rhs);
	template<typename TT>
	friend sparsematrix_t<TT>  operator*(sparsematrix_t<TT> lhs, sparsematrix_t<TT> rhs);

	template<typename TT>
	friend sparsematrix_t<TT>  operator*(TT lhs, sparsematrix_t<TT> rhs);
	
	template<typename TT>
	friend	vector_t<TT> operator*(sparsematrix_t<TT>&m , vector_t<TT>&v);
	
private:
	int rows{ 0 }, cols{ 0 };
	entry_t<T>* entities;
};

template<typename TT>
sparsematrix_t<TT>  operator-(sparsematrix_t<TT> lhs, sparsematrix_t<TT> rhs) {
	sparsematrix_t<TT> temp(lhs);
	return (temp -= rhs);
}


template<typename TT>
sparsematrix_t<TT>  operator+(sparsematrix_t<TT> lhs, sparsematrix_t<TT> rhs) {
	sparsematrix_t temp(lhs);
	return (temp += rhs);
}

template<typename TT>
sparsematrix_t<TT>  operator*(sparsematrix_t<TT> lhs, sparsematrix_t<TT> rhs) {
	sparsematrix_t<TT> temp(lhs);
	return (temp *= rhs);
}

	template<typename TT>
	inline sparsematrix_t<TT> operator*(TT lhs, sparsematrix_t<TT> rhs)
	{
		return rhs * lhs;
	}

template<typename TT>
	vector_t<TT> operator*(sparsematrix_t<TT>&m, vector_t<TT>&v)
	{
		vector_t<TT>  res(3);
		for (size_t i = 0; i < m.getCols(); i++)
		{
			res[i] = 0.0;
		}
		TT temp;
		for (size_t i = 0; i < m.getCols(); i++)
		{
			temp = 0;
			for (size_t j = 0; j < m.getCols(); j++)
			{
				temp += m.operator[](i).operator[]<TT>(j) * v.operator[](j);
			}
			res.operator[](i) = temp;
		}
		return res;
	}
	

template<typename TT>
sparsematrix_t<TT>  operator/(sparsematrix_t<TT> lhs, sparsematrix_t<TT> rhs) {
	sparsematrix_t temp(lhs);
	return (temp /= rhs);
}

template<typename TT>
ostream & operator<<(ostream & os, const sparsematrix_t<TT> S) {
	for (int i = 0; i < S.rows; i++)
	{
		entry_t<TT> x = S.entities[i];
		for (int j = 0; j < S.cols; j++)
		{
			if (x.column == j) {
				os << x.coeff << " ";
				if (x.next != NULL)
					x = *x.next;
			}
			else
				os << 0 << " ";
		}
		os << endl;
	}
	return os;
}

