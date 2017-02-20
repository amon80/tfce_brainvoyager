#include "StatisticalMap3D.h"
#include "BinaryString.h"
#include "FindClusters3D.h"
#include <cfloat>
#include <cmath>

//utility functions
int lessThan(const float a, const float b){
    return a < b;
}

int moreThan(const float a, const float b){
    return a > b;
}

int equalTo(const float a, const float b){
    return a == b;
}


float multiply(const float a, const float b){
    return a * b;
}

float divide(const float a, const float b){
    return a / b;
}

float elevate(const float a, const float b){
	return pow(a,b);
}

float subtract(float a, float b) {
	if (a > 0)
		return a - b;
	else
		return a + b;
}


// ---------- FRIEND METHODS ---------------
std::ostream & operator<<(std::ostream &os, const StatisticalMap3D& p){
    os << p.dim << "\n";
    os << p.dimX << "\n";
    os << p.dimY << "\n";
    os << p.dimZ << "\n";
    for (int i = 0; i < p.dim; ++i) {
        os << p.map[i];
        if (i != p.dim - 1)
            os << "\n";
    }
    return os;
}

std::istream& operator>>(std::istream &is, StatisticalMap3D& p){
    int i = 0;
    is >> p.dim;
    is >> p.dimX;
    is >> p.dimY;
    is >> p.dimZ;
    float value;
    if(p.map == nullptr)
        p.map = new float[p.dim];
    while(is >> value){
        p.map[i++] = value;
    }
    return is;
}


void swap(StatisticalMap3D& first, StatisticalMap3D& second){
    using std::swap;

    swap(first.dim, second.dim);
    swap(first.dimX, second.dimX);
    swap(first.dimY, second.dimY);
    swap(first.dimZ, second.dimZ);
    swap(first.map, second.map);

}

StatisticalMap3D operator+(StatisticalMap3D lhs, StatisticalMap3D& rhs){
    lhs += rhs;
    return lhs;
}

//-------------- CONSTRUCTORS ------------------
StatisticalMap3D::StatisticalMap3D()
    :map(nullptr),
     dimX(0),
     dimY(0),
     dimZ(0),
     dim(0)
{

}

StatisticalMap3D::StatisticalMap3D(int dimX, int dimY, int dimZ)
    :map(new float[dimX*dimY*dimZ]),
     dimX(dimX),
     dimY(dimY),
     dimZ(dimZ),
     dim(dimX*dimY*dimZ)
{
    for (int i = 0; i < dim; ++i) {
        this->map[i] = 0;
    }
}

StatisticalMap3D::StatisticalMap3D(float* map, int dimX, int dimY, int dimZ)
    :map(new float[dimX*dimY*dimZ]),
     dimX(dimX),
     dimY(dimY),
     dimZ(dimZ),
     dim(dimX*dimY*dimZ)
{
    for (int i = 0; i < dim; ++i) {
        this->map[i] = map[i];
    }
}

//mask must have length dim
StatisticalMap3D::StatisticalMap3D(const StatisticalMap3D& map, BinaryString& mask)
    :map(new float[dimX*dimY*dimZ]),
     dimX(map.dimX),
     dimY(map.dimY),
     dimZ(map.dimZ),
     dim(map.dim)
{
    for (int i = 0; i < dim; ++i) {
        if(mask[i] == 1)
            this->map[i] = map[i];
        else
            this->map[i] = 0;
    }
}

StatisticalMap3D::StatisticalMap3D(const BinaryString &toConvert, int dimX, int dimY, int dimZ)
    :map(new float[dimX*dimY*dimZ]),
     dimX(dimX),
     dimY(dimY),
     dimZ(dimZ),
     dim(dimX*dimY*dimZ)
{
    float value;
    for (int i = 0; i < dim; ++i) {
        value = (float) toConvert[i];
        map[i] = value;
    }
}

//--------- DESTRUCTOR ----------------
StatisticalMap3D::~StatisticalMap3D(){
    delete [] map;
}


//--------- COPY SEMANTICS --------------
StatisticalMap3D::StatisticalMap3D(const StatisticalMap3D& rhs)
    :map(new float[rhs.dim]),
     dimX(rhs.dimX),
     dimY(rhs.dimY),
     dimZ(rhs.dimZ),
     dim(rhs.dim)
{
    for (int i = 0; i < dim; ++i) {
        this->map[i] = rhs.map[i];
    }
}

StatisticalMap3D& StatisticalMap3D::operator=(const StatisticalMap3D& rhs){
    StatisticalMap3D tmp(rhs);
    swap(*this, tmp);
    return *this;
}


//----------- MOVE SEMANTICS ------------
StatisticalMap3D::StatisticalMap3D(StatisticalMap3D&& rhs)
    : StatisticalMap3D()
{
    swap(*this, rhs);
}

StatisticalMap3D& StatisticalMap3D::operator=(StatisticalMap3D&& rhs){
    swap(*this, rhs);
    return *this;
}


//------------- MODIFIERS -----------------
void StatisticalMap3D::zeroMap(int pos_or_neg){
    //all zeros
    if(pos_or_neg == 0){
        for(int i = 0; i < dim; i++){
            map[i] = 0;
        }
    }
    //keep the negatives
    else if(pos_or_neg == -1){
        for(int i = 0; i < dim; i++){
            if(map[i] > 0)
                map[i] = 0;
        }
    }
    //keep the negatives
    else if(pos_or_neg == 1){
        for(int i = 0; i < dim; i++){
            if(map[i] < 0)
                map[i] = 0;
        } 
    }
}

void StatisticalMap3D::flipMap(){
    for(int i = 0; i < dim; i++){
        if(map[i] != 0)
            map[i] = -map[i];
    }
}
    
void StatisticalMap3D::applyOperation(float (* operation) (const float a, const float b), const float argument){
    for (int i = 0; i < dim; ++i) {
		if (map[i] != 0){
			map[i] = operation(map[i], argument);
		}
	}
}

StatisticalMap3D& StatisticalMap3D::operator+=(const StatisticalMap3D& rhs){
    for(int i = 0; i < dim; i++){
        map[i] += rhs.map[i];
    }
    return *this;
}

float& StatisticalMap3D::operator[](const std::size_t idx){
    return map[idx];
}

//------------- ACCESS METHODS
const float StatisticalMap3D::operator[](const std::size_t idx) const{
    return map[idx];
}

const int StatisticalMap3D::size() const{
    return dim;
}

const int StatisticalMap3D::sizeX() const{
    return dimX;
}
const int StatisticalMap3D::sizeY() const{
    return dimY;
}
const int StatisticalMap3D::sizeZ() const{
    return dimZ;
}

void StatisticalMap3D::findMinMax(float& min, float& max, float& range){
    min = map[0];
    max = map[0];
    for (int i = 1; i < dim; ++i) {
		if (map[i] < min)
			min = map[i];
		if (map[i] > max)
			max = map[i];
	}
	range = max - min;
}


//-------------- TFCE METHODS -----------------
void StatisticalMap3D::computeTfceIteration(StatisticalMap3D& tfce_map, float h, float increment, float E, float H){
    int i = 0;
    BinaryString clusterMap(*this, moreThan, h);
	int num_clusters = find_clusters_3D(clusterMap, dimX, dimY, dimZ);
    BinaryString extent_map(dim);
    turn_into_extend_map(extent_map, clusterMap, num_clusters);
    StatisticalMap3D clustered_map_float(extent_map, dimX, dimY, dimZ);
    clustered_map_float.applyOperation(elevate, E);
    clustered_map_float.applyOperation(multiply, pow(h,H));
    clustered_map_float.applyOperation(multiply, increment);
	for (i = 0; i < dim; ++i) {
//#pragma omp atomic
			tfce_map[i] += (clustered_map_float[i]);
	}
}

void StatisticalMap3D::tfce(float E, float H, float dh){
	float minData = 0; 
    float maxData = 0; 
    float rangeData = 0;
	StatisticalMap3D tfce_map(dimX, dimY, dimZ);

	this->findMinMax(minData, maxData, rangeData);
    if(minData >= 0){
        int i;
        float precision = rangeData/dh;
        int steps;
        float increment;

        if (precision > 200) {
            increment = rangeData/200;
        } else{
            increment = rangeData/precision;
        }

        steps = (int) ceil(rangeData / increment);
        //#pragma omp parallel for
        for (i = 0; i < steps; i++) {
            (*this).computeTfceIteration(tfce_map, minData + i*increment, increment, E, H);
        }
    }else{
        BinaryString indexPosData(*this, lessThan, 0);
        BinaryString indexNegData(*this, lessThan, 0);
        StatisticalMap3D posData(*this, indexPosData);
        StatisticalMap3D negData(*this, indexNegData);
        negData.flipMap();
        posData.tfce(E, H, dh);
        negData.tfce(E, H, dh);
        negData.flipMap();
        tfce_map = posData + negData;
    }
	(*this) = tfce_map;
}
    


