#include <algorithm>
#include "Field.h"

using namespace flow;

size_t Field::GetNumOfValidVelocityNames() const
{
    return std::count_if(VelocityNames.begin(), VelocityNames.end(), 
           [](const std::string &e) { return !e.empty(); });
}
