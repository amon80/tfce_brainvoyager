#include "BinaryString.h"
#include "StatisticalMap3D.h"
#include <random>

//friend functions
std::ostream & operator<<(std::ostream &os, const BinaryString& p){
    int i;
    for(i = 0; i < p.n_bits; i++){
        os << p.string[i];
    }
    return os;
}

void swap(BinaryString& first, BinaryString& second){
    using std::swap;

    swap(first.n_bits, second.n_bits);
    swap(first.n_bits_set, second.n_bits_set);
    swap(first.string, second.string);

}

bool operator<(const BinaryString& first, const BinaryString& second){
    if(first.n_bits < second.n_bits)
        return true;
    for (int i = 0; i < first.n_bits; i++) {
        if (first.string[i] < second.string[i])
            return true;
        else if(first.string[i] > second.string[i])
            return false;
    }
	return false;
}

bool operator>(const BinaryString& first, const BinaryString& second){
    if(first.n_bits < second.n_bits)
        return false;
    for (int i = 0; i < first.n_bits; i++) {
        if (first.string[i] < second.string[i])
            return false;
        else if(first.string[i] > second.string[i])
            return true;
    }
	return false;
}

bool operator==(const BinaryString& first, const BinaryString& second){
    if(first.n_bits != second.n_bits)
        return false;
    for (int i = 0; i < first.n_bits; i++) {
        if (first.string[i] != second.string[i])
            return false;
    }
    return true;
}

//-----CONSTRUCTORS------------

BinaryString::BinaryString()
    :n_bits(0),
    string(nullptr),
    n_bits_set(0)
{
}

BinaryString::BinaryString(int num_bits, int num_to_convert)
    :n_bits(num_bits),
    string(new int[num_bits]),
    n_bits_set(0)
{
    int mask = 1;
    for (int i = 0; i < num_bits; ++i) {
        if(num_to_convert & mask){
            string[num_bits-i-1] = 1;
            n_bits_set++;
        }
        else
            string[num_bits-i-1] = 0;
        mask <<= 1;
    }
}


BinaryString::BinaryString(int num_bits, bool random)
    :n_bits(num_bits),
    string(new int[num_bits]),
    n_bits_set(0)
{
    int i;
    if(random){
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        for (int i = 0; i < n_bits; ++i) {
            string[i] = dist(e2);
            if(string[i])
                n_bits_set++;
        }
    }
    else{
        for(i = 0; i < n_bits; i++){
            string[i] = 0;
        }
    }
}

BinaryString::BinaryString(const StatisticalMap3D& map, int (*confront)(const float, float), float value)
    :n_bits(map.size()),
    string(new int[n_bits]),
    n_bits_set(0)
{
	for (int i = 0; i < n_bits; ++i) {
		if(confront(map[i],value)){
			string[i] = 1;
            n_bits_set++;
        }
		else
			string[i] = 0;
	}
}

//----------- COPY SEMANTICS --------------
BinaryString::BinaryString(const BinaryString& rhs)
    :n_bits(rhs.n_bits),
    string(new int[rhs.n_bits]),
    n_bits_set(rhs.n_bits_set)
{
	for(int i = 0; i < n_bits; i++){
	    string[i] = rhs.string[i];
	}
}

BinaryString& BinaryString::operator=(const BinaryString& rhs){
    BinaryString tmp(rhs);
    swap(*this, tmp);
    return *this;
}

//---------- MOVE SEMANTICS ----------------
BinaryString::BinaryString(BinaryString &&rhs)
    : BinaryString()
{
    swap(*this, rhs);
}

BinaryString& BinaryString::operator=(BinaryString &&rhs){
    swap(*this, rhs);
    return *this;
}


//----------- DESTRUCTOR ------------
BinaryString::~BinaryString(){
    delete[] string;
}

// ----------- MODIFIERS -------------
int& BinaryString::operator[](std::size_t idx){
    return string[idx];
}

// ------------ ACCESS METHODS ------------

const int BinaryString::operator[](std::size_t idx) const{
    return string[idx];
}
const int BinaryString::size() const{
    return n_bits;
}
    
const int BinaryString::size_active_bits() const{
    return n_bits_set;
}
