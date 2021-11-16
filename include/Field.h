//
// The base class of all possible fields for flow integration.
//

#ifndef FIELD_H
#define FIELD_H

#include <string>
#include <array>
#include <glm/glm.hpp>

namespace flow
{
class Field
{
public:
    // Constructor and destructor.
    // This class complies with rule of zero.
    Field() = default;
    virtual ~Field() = default;

    //
    // If a given position at a given time is inside of this field
    //
    virtual bool InsideVolumeVelocity(double time, glm::vec3 pos) const = 0;
    virtual bool InsideVolumeScalar(double time, glm::vec3 pos) const = 0;

    //
    // Retrieve the number of time steps in this field
    //
    virtual size_t GetNumberOfTimesteps() const = 0;

    //
    // Get the field value at a certain position, at a certain time.
    // In case of an error, a NAN value will be returned, and it can
    // be detected using `std::isnan()`.
    //
    virtual auto GetScalar(double time, glm::vec3 pos) const -> float = 0;

    //
    // Get the velocity value at a certain position, at a certain time.
    // In case of an error, one or more NAN will be returned, and it can
    // be detected using `glm::isnan()`.
    //
    virtual auto GetVelocity(double time, glm::vec3 pos) const -> glm::vec3 = 0;

    //
    // Returns the number of valid (non-empty) velocity variable names.
    // It is zero in two normal cases:
    // 1. when the object is newly created; 
    // 2. when this field is a scalar field
    //
    size_t GetNumOfValidVelocityNames() const;

    //
    // Provide an option to cache and lock certain parameters.
    // Both functions return 0 on success.
    //
    virtual auto LockParams() -> CODE = 0;
    virtual auto UnlockParams() -> CODE = 0;

    // Class members
    bool                       IsSteady = false;
    std::string                ScalarName = "";
    std::array<std::string, 3> VelocityNames = {{"", "", ""}};
};
};    // namespace flow

#endif
