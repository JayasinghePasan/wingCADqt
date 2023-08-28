#include "wingClass.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <sstream>

#include "coolTools.h"



//Constructor
wingClass::wingClass(std::string wingName) {
    this->wingName = wingName;
    if (this->readFiles())
    {
        //############## QT Console ##################
        std::cout << "Wing Object " << this->wingName << " created successfully." << std::endl;
        //############################################
    }
}




//read the wing data file located at AeroFoilDat directory
//and add all the (x,y) coordinates to the vector<> dataVector
int wingClass::readFiles()
{
    std::string fileLocation = "AeroFoilDat/" + this->wingName + ".txt";
    std::ifstream file(fileLocation);

    if (!file.is_open())
    {
        //############## QT Console ##################
        std::cout << "Error opening the file at " << fileLocation << std::endl;
        //############################################
        return 0;
    }



    //Recheck the name at the 1st line of the txt file
    //return 0 and print error msg
    std::string line;
    getline(file, line);
    if (coolTools::trim(line) != this->wingName)
    {
        //############## QT Console ##################
        std::cout << "Text file name mismatch with the content." << std::endl;
        //############################################
        return 0;
    }

    // Read and process the remaining lines
    while (getline(file, line))
    {
        std::istringstream iss(line);
        double x, y;
        if (iss >> x >> y)
        {
            this->dataVector.push_back({ x, y });
        }
    }
    file.close();
    return 1;
}
