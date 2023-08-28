#include "coolTools.h"

namespace coolTools {


    //trim blank spaces in front and back of a string
    //return trimmed string
    std::string trim(const std::string& str)
    {
        size_t start = str.find_first_not_of(" \t");
        size_t end = str.find_last_not_of(" \t");
        if (start == std::string::npos) return "";
        return str.substr(start, end - start + 1);
    }



}
