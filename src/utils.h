#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <ctime>
#include <string>

namespace utils {
    // Random number generator between min and max (inclusive)
    inline int randomInt(int min, int max) {
        return min + (std::rand() % (max - min + 1));
    }
    
    // Format a number with commas
    inline std::string formatNumber(int number) {
        std::string numStr = std::to_string(number);
        int len = numStr.length();
        
        if (len <= 3) {
            return numStr;
        }
        
        std::string result;
        int count = 0;
        
        for (int i = len - 1; i >= 0; i--) {
            result = numStr[i] + result;
            count++;
            
            if (count % 3 == 0 && i > 0) {
                result = "," + result;
            }
        }
        
        return result;
    }
}

#endif // UTILS_H
