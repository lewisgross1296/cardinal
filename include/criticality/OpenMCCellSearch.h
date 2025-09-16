#pragma once

#include "CriticalitySearchBase.h"

/**
 * Perform a criticality search based on a cell transformation
 */
class OpenMCCellSearch : public CriticalitySearchBase
{
public:
  static InputParameters validParams();

  OpenMCCellSearch(const InputParameters & parameters);

protected:
  /// list of cell ids to be transformed
  const std::vector<int32_t> _cell_id_list;

  /// list of indices corresponding to the IDs of cells to be transformed
  std::vector<int32_t> _cell_index_list;
};
