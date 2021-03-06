// PZ12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <iterator>
#include <stdexcept>

using namespace std;

//overriding output operator with displaying vector values
template <typename T>
ostream& operator<<(ostream& out, const vector<T>& v)
{
	out << "{";
	for (auto p = v.begin(); p != v.end(); p++) {
		out << *p;
		if (p != v.end() - 1)
			out << ", ";
	}
	out << "}\n";
	return out;
}

//overriding input operator to input array of values to vector, using '{' and '}' to find the begin and end of input
template <typename T>
istream& operator>>(istream& in, vector<T>& v)
{
	char ch;
	in >> ch;

	if (ch != '{')
	{
		cout << "should start with '{'\n";
		return in;
	}
	T value;
	while (true)
	{
		in >> ch;
		if (in && ch != '}')
		{
			if (ch == ',')
				continue;
			in.unget();
			in >> value;
			v.push_back(value);
		}
		else
			return in;
	}
	return in;
}

class WordCounter
{
public:
	std::string Separator() const { return separator; }
	void Separator(std::string val) 
	{ 
		if (val.empty())
			val = " ";

		separator = val; 
	}
	
	std::string WordList() const { return wordList; }
	void WordList(std::string val) { wordList = val; }
private:
	string wordList;
	string separator;
public:
	WordCounter(string input, string separator = " ") :wordList(input), separator(separator) {};

	//finding amount of words by counting occurrences of separator
	int CountWords(string separator)
	{
		int counter = 0;
		size_t pos = wordList.find(separator);

		if (pos != string::npos)
			counter++;

		while ((pos = wordList.find(separator, pos))!= string::npos)
		{
			counter++;
			pos += separator.length();
		}

		return counter;
	}

	int CountWords()
	{
		return CountWords(separator);
	}
};

//copying values from list to vector
vector<double> ListToVector(list<int>& in)
{
	vector<double> result;
	for (auto p = in.begin(); p != in.end(); p++)
		result.push_back(*p);

	return result;
}


//defining custom iterator with checking on range of iterator
//for initializing we need vector and desired position
//please note as we are using custom iterator for non-custom vector, it is only efficient until vector is changed as its internal array may be reallocated
template <class T>
class MyIterator : public std::iterator<std::random_access_iterator_tag, T>
{
	T* cur;
	T* first;
	T* last;
	int pos;
	int size;
public:
	MyIterator(vector<T>& v, int pos) :pos(pos)
	{
		cur = &v[pos];
		first = &*v.begin();
		last = &*v.begin() + v.size() - 1;
		size = v.size();
	}
	MyIterator(const MyIterator& mit) : cur(mit.cur), first(mit.first), last(mit.last), pos(mit.pos), size(mit.size) {}
	MyIterator& operator++() 
	{ 
		if (cur != last)
		{	
			cur++;
			pos++;
		}
		else
			throw out_of_range("index out of range");
		return *this; 
	}
	MyIterator& operator--()
	{
		if (cur != first)
		{
			cur--;
			pos--;
		}
		else
			throw out_of_range("index out of range");
		return *this;
	}
	MyIterator operator++(int) { MyIterator tmp(*this); operator++(); return tmp; }
	MyIterator operator--(int) { MyIterator tmp(*this); operator--(); return tmp; }
	MyIterator& operator+(int n)
	{
		if (pos + n >= size)
			throw out_of_range("index out of range");
		else
		{
			cur += n;
			pos += n;
		}
		return *this;
	}
	MyIterator& operator-(int n)
	{
		if (pos - n < 0)
			throw out_of_range("index out of range");
		else
		{
			cur -= n;
			pos -= n;
		}
		return *this;
	}
	MyIterator operator+=(int n) { MyIterator tmp(*this); operator+(n); return tmp; }
	MyIterator operator-=(int n) { MyIterator tmp(*this); operator-(n); return tmp; }
	MyIterator operator[](int n) { 
		MyIterator tmp(*this); 
		if (n > pos)
			operator+(n - pos);
		else if (n < pos)
			operator-(pos - n);
		return tmp; 
	}
	bool operator==(const MyIterator& rhs) const { return cur == rhs.cur; }
	bool operator!=(const MyIterator& rhs) const { return cur != rhs->cur; }
	T& operator*() { return *cur; }
};

int main()
{
	//1. override >> and << operators for vector
	vector<int> a;
	cout << "enter vector values separated by comma and surrounded with curve brackes like {1, 2, ... }\n";
	cin >> a;
	cout << a;
	cout << endl;
	
	//2. counting words in provided string using default or custom separator
	cout << "let's count words in 'some test string' and separator is ' '\n";
	WordCounter wc("some test string");
	cout << "the result is " << wc.CountWords() << endl;

	cout << "let's count words in 'what,about,such,string' and separator is ','\n";
	wc.WordList("what,about,such,string");
	cout << "the result is " << wc.CountWords(",") << endl;
	cout << endl;

	//3. copy list of ints to vector of doubles, and sort it
	vector<double> vd;
	cout << "original list of int is {4, 1, 5, 2, 3}\n";
	list<int> li = { 4, 1, 5, 2, 3 };

	vd = ListToVector(li);
	cout << "vector copy is " << vd;

	sort(vd.begin(), vd.end());
	cout << "sorted vector looks like this: " << vd;
	cout << endl;
	
	//4. custom iterator for std vector with range check
	cout << "vector is {1,2,3,4,5}\n";
	vector<int> v({1,2,3,4,5});
	MyIterator<int> mi(v, 0);
	
	cout << "jumping two elements forward\n";
	mi += 2;
	cout << *mi << endl;
	
	cout << "jumping to one element back\n";
	mi--;
	cout << *mi << endl;

	cout << "jumping to tenth element\n";
	try
	{
		mi[10];
	}
	catch (out_of_range& e)
	{
		cerr << e.what() << endl;
	}
	
    return 0;
}