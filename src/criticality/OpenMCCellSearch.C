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

#include "OpenMCCellSearch.h"
#include "UserErrorChecking.h"
#include "openmc/capi.h"

InputParameters
OpenMCCellSearch::validParams()
{
  auto params = CriticalitySearchBase::validParams();
  params.addRequiredParam<std::vector<int32_t>>("cell_id_list", "Cell IDs to modify");
  params.addClassDescription(
      "Base class for criticality searches using the properties of cells");
  return params;
}

OpenMCCellSearch::OpenMCCellSearch(const InputParameters & parameters)
  : CriticalitySearchBase(parameters), _cell_id_list(getParam<std::vector<int32_t>>("cell_id_list"))
{
  for (auto cell_id : _cell_id_list )
  {
    // TODO: update for cells: int err = openmc_get_material_index(_cell_id_list, &_material_index);
    catchOpenMCError(err, "get index for cell with ID " + std::to_string(cell_id));

    // TODO: do we need to test that the given cell already has a rotation
  }
}

#endif
