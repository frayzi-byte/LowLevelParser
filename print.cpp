#include <iostream>
#include <ifaddrs.h>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    struct ifaddrs *finterface;

    int result = getifaddrs(&finterface);

    switch (result)
    {
    case 0:
        break;
    
    case -1:
        std::cout << "Error!\n";
        return 1;

    default:
        break;
    }

    std::vector<std::string> unique_interfaces;    

    for (struct ifaddrs *i = finterface; i != nullptr; i= i -> ifa_next)
    {
        if (std::find(unique_interfaces.begin(), unique_interfaces.end(), i -> ifa_name) == unique_interfaces.end()) { 
            unique_interfaces.push_back(i -> ifa_name);
        }
    }

    for (std::string name : unique_interfaces) {
        std::cout << ">>"<< name << "\n";
    }
    
    freeifaddrs(finterface);

    return 1;
}