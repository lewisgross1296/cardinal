#pragma once

#include "OpenMCCellSearch.h"

/**
 * Perform a criticality search based on a material total density
 */
class OpenMCCellRotation : public OpenMCCellSearch
{
public:
  static InputParameters validParams();

  OpenMCCellRotation(const InputParameters & parameters);

  virtual void updateOpenMCModel(const Real & input) override;

protected:
  virtual std::string quantity() const override
  {
    std::string cell_id_list_str = "rotation for cells:";
    for (auto cell_id : _cell_id_list )
      cell_id_list_str += " " + std::to_string(cell_id);
    return cell_id_list_str;
  }

  virtual std::string units() const override { return "[degrees]"; }

  const char _rotation_axis_char;

  std::vector<Real> _rotation_axis;
};
