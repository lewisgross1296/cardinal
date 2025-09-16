/********************************************************************/
/*                  SOFTWARE COPYRIGHT NOTIFICATION                 */
/*                             Cardinal                             */
/*                                                                  */
/*                  (c) 2021 UChicago Argonne, LLC                  */
/*                        ALL RIGHTS RESERVED                       */
/*                                                                  */
/*                 Prepared by UChicago Argonne, LLC                */
/*               Under Contract No. DE-AC02-06CH11357               */
/*                With the U. S. Department of Energy               */
/*                                                                  */
/*             Prepared by Battelle Energy Alliance, LLC            */
/*               Under Contract No. DE-AC07-05ID14517               */
/*                With the U. S. Department of Energy               */
/*                                                                  */
/*                 See LICENSE for full restrictions                */
/********************************************************************/

#ifdef ENABLE_OPENMC_COUPLING

#include "OpenMCCellRotation.h"
#include "openmc/capi.h"

registerMooseObject("CardinalApp", OpenMCCellRotation);

InputParameters
OpenMCCellRotation::validParams()
{
  auto params = OpenMCCellSearch::validParams();
  params.addRequiredParam<char>("rotation_axis", "Axis about which to rotate the cell. [x, y, z]");
  params.addClassDescription("Searches for criticality using cell rotation in units of degrees");
  return params;
}

OpenMCCellRotation::OpenMCCellRotation(const InputParameters & parameters)
  : OpenMCCellSearch(parameters),
    _rotation_axis_char(getParam<char>("rotation_axis"))
{
  // apply additional checks on the minimum and maximum values - the difference must be less 
  // than 360 degrees to ensure a single root.  Note: this condition is necessary but not 
  // sufficient since some rotations may have multiple roots in a single rotation.
  if (_maxiumum - _minimum > 360.0)
    paramError("maximum",
               "The difference between the 'maximum' and 'minimum' (" +
               std::to_string(_maximum) + " - " + std::to_string(_minumum) +
               " = " + std::to_string(_maximum - _minimum) + 
               ") must be less than or equal to 360 degrees!");

  switch (_rotation_axis_char) {
    case 'x':
      _rotation_axis = {1,0,0};
      break;
    case 'y':
      _rotation_axis = {0,1,0};
      break;
    case 'z':
      _rotation_axis = {0,0,1};
      break;
    default:
      paramError("rotation_axis",
        "The rotation axis (" + _rotation_axis_char + ") must be one of 'x', 'y', 'z'!");
  }

}

void
OpenMCCellRotation::updateOpenMCModel(const Real & rotation_angle)
{
  _console << "Searching for " << quantity() << ": " << rotation_angle << " " << units() 
    << " ..." << std::endl;

  // TODO: this probably needs to be a loop
  std::vector<Real> rotation_tuple;
  for (auto axis : _rotation_axis )
    rotation_tuple.push_back(axis * rotation_angle);

  for (auto cell_idx : _cell_index_list )
    {
      // TODO: what is the right API to change a rotation angle
      //  int err = openmc_material_set_rotation_angle(
      //    _material_index, rotation_angle * _openmc_problem->rotation_angleConversionFactor(), units);

      // TODO: can we get a cell_id from a cell_idx for the error message
      catchOpenMCError(err, "set cell rotation_angle for cell " + std::to_string(cell_idx) +
        " to " + std::to_string(rotation_angle));
}
}

#endif
