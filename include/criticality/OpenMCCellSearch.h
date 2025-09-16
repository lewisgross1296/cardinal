#pragma once

#include "CriticalitySearchBase.h"

/**
 * Perform a criticality search based on a material
 */
class OpenMCCellSearch : public CriticalitySearchBase
{
public:
  static InputParameters validParams();

  OpenMCCellSearch(const InputParameters & parameters);

protected:
  /// Material to be modified
  const std::vector<int32_t &> _cell_id_list;

  /// Material index corresponding to the ID
  std::vector<int32_t> _cell_index_list;
};
