//
// This class performances advection calculations.
// It also holds all the particles resulting from an advection.
//

#ifndef ADVECTION_H
#define ADVECTION_H

#include "Particle.h"
#include "Field.h"
#include <string>
#include <vector>
#include <array>

namespace flow
{
class Advection final 
{
public:

    enum class ADVECTION_METHOD {
        EULER = 0,
        RK4 = 1    // Runge-Kutta 4th order
    };

    // Constructor
    Advection();

    //
    // Major action functions
    //
    // Advect all particles as long as they are within spatial and temporal boundary
    // for a specified number if steps.
    auto AdvectSteps(Field *velocityField, double deltaT, size_t maxSteps, 
                     ADVECTION_METHOD method = ADVECTION_METHOD::RK4) -> CODE;

    // Advect as many steps as necessary to reach a certain time: targetT.
    // Note: it only considers particles that have already passed startT.
    auto AdvectTillTime(Field *velocityField, double startT, double deltaT, double targetT, 
                        ADVECTION_METHOD method = ADVECTION_METHOD::RK4) -> CODE;

    // Retrieve field values of a particle based on its location, and put the result in
    // the "value" field or the "properties" field of a particle
    // If "skipNonZero" is true, then this function only overwrites zeros.
    // Otherwise, it will overwrite values anyway.
    auto CalculateParticleValues(Field *scalarField, bool skipNonZero) -> CODE;
    auto CalculateParticleProperties(Field *scalarField) -> CODE;

    // Reset all particle values to zero
    void ResetParticleValues();
    // Clear all existing properties of a particle
    void ClearParticleProperties();
    // Clear particle property with a certain name.
    // If the the specified property name does not exist, then nothing is done.
    void RemoveParticleProperty(const std::string &);

    void UseSeedParticles(const std::vector<Particle> &seeds);

    // Retrieve the resulting particles as "streams."
    auto GetNumberOfStreams() const -> size_t;
    auto GetStreamAt(size_t i) const -> const std::vector<Particle>&;

    // Retrieve the maximum number of particles in ANY stream
    size_t GetMaxNumOfPart() const;

    // Query properties (most are properties of the velocity field)
    auto SeedsReady() const -> CODE;

    // Specify periodicity, and periodic bounds on each dimension
    void SetXPeriodicity(bool, float min, float max);
    void SetYPeriodicity(bool, float min, float max);
    void SetZPeriodicity(bool, float min, float max);

    // Retrieve the names of value variable and property variables.
    auto GetValueVarName() const -> std::string;
    auto GetPropertyVarNames() const -> std::vector<std::string>;

private:
    std::vector<std::vector<Particle>> _streams;
    std::string                        _valueVarName;
    std::vector<std::string>           _propertyVarNames;

    const float      _lowerAngle, _upperAngle;          // Thresholds for step size adjustment
    float            _lowerAngleCos, _upperAngleCos;    // Cosine values of the threshold angles
    std::vector<size_t> _separatorCount;                // how many separators does each stream have.
                                                        // Useful to determine how many steps are there in a stream.
    // If the advection is performed in a periodic fashion along one or more dimensions.
    // These variables are **not** intended to be decided by Advection, but by someone
    // who's more knowledgeable about the field.
    std::array<bool, 3> _isPeriodic = {false, false, false};
    // Elements [0] [1] will decide bounds along X
    // Elements [2] [3] will decide bounds along Y
    // Elements [4] [5] will decide bounds along Z
    std::array<float, 6> _periodicBounds = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    // Advection methods here could assume all input is valid.
    auto _advectEuler(const Field* f, Particle P, double deltaT) const -> Particle;
    auto _advectRK4(const Field* f, Particle p, double deltaT) const -> Particle;

    // Get an adjust factor for deltaT based on how curvy the past two steps are.
    //   A value in range (0.0, 1.0) means shrink deltaT.
    //   A value in range (1.0, inf) means enlarge deltaT.
    //   A value equals to 1.0 means not touching deltaT.
    float _calcAdjustFactor(const Particle& past2, const Particle& past1, const Particle& current) const;

    // Adjust input "loc" according to the bound specified by min and max.
    // Returns the value after adjustment.
    float _applyPeriodic(float loc, float min, float max) const;
};
};    // namespace flow

#endif
