#ifndef STATISTICALMAP3D_H
#define STATISTICALMAP3D_H

#include <iostream>
class BinaryString;

class StatisticalMap3D
{
public:
    //rule of five
    //constructors
    StatisticalMap3D();
    StatisticalMap3D(float * map, int dimX, int dimY, int dimZ);
    StatisticalMap3D(int dimX, int dimY, int dimZ);
    StatisticalMap3D(const StatisticalMap3D& map, BinaryString& mask);
    StatisticalMap3D(const BinaryString& toConvert, int dimX, int dimY, int dimZ);
    //destructor
    ~StatisticalMap3D();
    //copy semantics
    StatisticalMap3D(const StatisticalMap3D& rhs);
    StatisticalMap3D& operator=(const StatisticalMap3D& rhs);
    //move sematics
    StatisticalMap3D(StatisticalMap3D&& rhs);
    StatisticalMap3D& operator=(StatisticalMap3D&& rhs);

    //modifiers methods
    void zeroMap(int pos_or_neg);
    void flipMap();
    void applyOperation(float (* operation) (const float a, const float b), const float argument);
    void findMinMax(float& min, float& max, float& range);
    void tfce(float E = 0.5, float H = 2.0, float dh = 0.1);
    StatisticalMap3D& operator+=(const StatisticalMap3D& rhs);
    float& operator[](const std::size_t idx);

    //access methods
    const int size() const;
    const int sizeX() const;
    const int sizeY() const;
    const int sizeZ() const;
    const float operator[](const std::size_t idx) const;
private:
    friend std::ostream & operator<<(std::ostream &os, const StatisticalMap3D& p);
    friend void swap(StatisticalMap3D& first, StatisticalMap3D& second);
    friend std::istream& operator>>(std::istream &is, StatisticalMap3D& p);
    friend StatisticalMap3D operator+(StatisticalMap3D lhs, StatisticalMap3D& rhs);
    void computeTfceIteration(StatisticalMap3D& tfce_map, float level, float increment, float E, float H);
    float * map;
    int dimX;
    int dimY;
    int dimZ;
    int dim;
};

#endif // STATISTICALMAP3D_H
