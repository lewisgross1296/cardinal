#ifdef ENABLE_OPENMC_COUPLING

#include "OpenMCCellRotation.h"
#include "openmc/capi.h"

registerMooseObject("CardinalApp", OpenMCCellRotation);

InputParameters
OpenMCCellRotation::validParams()
{
  auto params = OpenMCCellSearch::validParams();
  params.addRequiredParam<MooseEnum>("rotation_axis", getRotationAxisEnum() ,"Axis about which to rotate the cell. [x, y, z]");
  params.addClassDescription("Searches for criticality using cell rotation in units of degrees");
  return params;
}

OpenMCCellRotation::OpenMCCellRotation(const InputParameters & parameters)  : OpenMCCellSearch(parameters),
    _rotation_axis_char(getParam<MooseEnum>("rotation_axis"))
{
  // apply additional checks on the minimum and maximum values - the difference must be less 
  // than 360 degrees to ensure a single root.  Note: this condition is necessary but not 
  // sufficient since some rotations may have multiple roots in a single rotation.
  if (_maximum - _minimum > 360.0)
    paramError("maximum",
               "The difference between the 'maximum' and 'minimum' (" +
               std::to_string(_maximum) + " - " + std::to_string(_minimum) +
               " = " + std::to_string(_maximum - _minimum) + 
               ") must be less than or equal to 360 degrees!");

  // assign _rotation_axis depending on value specified in the enum
  switch (_rotation_axis_char) {
    case 'x':
      _rotation_axis = {1,0,0};
    case 'y':
      _rotation_axis = {0,1,0};
      break;
    case 'z':
      _rotation_axis = {0,0,1};
      break;
  }
    
}

void
OpenMCCellRotation::updateOpenMCModel(const Real & rotation_angle)
{
  _console << "Searching for " << quantity() << ": " << rotation_angle << " " << units() 
    << " ..." << std::endl;

  size_t rot_len = 9; // rotation matrix length to allocate vector size in openmc_cell_set_rotation

  // rotation tuple will contain two zero and one non-zero values 
  double phi = rotation_angle * _rotation_axis[0]; // signifies rotation around x axis if non-zero
  double theta =  rotation_angle * _rotation_axis[1]; // signifies rotation around y axis if non-zero
  double psi =  rotation_angle * _rotation_axis[2]; // signifies rotation around z axis if non-zero

  // generalized rotation_matrix formula for consistency with OpenMC implementation
  // see openmc.Cell documentation for more
  const double rotation_matrix[rot_len] = {  std::cos(theta) * std::cos(psi),
                                            -std::cos(phi) * std::sin(psi) + std::sin(phi) * std::sin(theta) * std::cos(phi),
                                             std::sin(phi) * std::sin(psi) + std::cos(phi) * std::sin(theta) * std::cos(psi), 
                                             std::cos(theta) * std::sin(psi),
                                             std::cos(phi) * std::cos(psi) + std::sin(phi) * std::sin(theta) * std::sin(psi),
                                            -std::sin(phi) * std::cos(psi) + std::cos(phi) * std::sin(theta) * std::sin(psi), 
                                            -std::sin(theta),
                                             std::sin(phi) * std::cos(theta),
                                             std::cos(phi) * std::cos(theta)};

  for (auto cell_idx : _cell_index_list )
    {
      int err = openmc_cell_set_rotation(cell_idx, rotation_matrix, rot_len);

      catchOpenMCError(err, "set cell rotation_angle for cell " + std::to_string(_cell_id_list.at(cell_idx)) +
        " to " + std::to_string(rotation_angle));
  }
}

#endif
