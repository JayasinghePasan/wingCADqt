#pragma once
#include <iostream>
#include <string>
#include <vector>


class wingClass
{
private:
    int readFiles();
    int chordLength;
    int spanLength;

public:
    wingClass(std::string wingName);

    std::string wingName;                                   //stores wing name
    std::vector <std::pair<double, double>> dataVector;     //stores coordinates in 0-1 frame


};

