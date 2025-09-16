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
    // get cell_idx for the current cell_id
    int cell_idx = 0;
    int cell_err = openmc_get_cell_index(cell_id, &cell_idx);
    catchOpenMCError(cell_err, "get index for cell with ID " + std::to_string(cell_id));
    if( openmc::model::cells[cell_idx]->type_ == openmc::Fill::UNIVERSE)
      _cell_index_list.push_back(cell_idx); // add cell if it's in the cell_id_list and is filled with a universe
    else {
      mooseWarning("You have included cell " + std::to_string(cell_id) + " in the cell_id_list " +
                   "however it is not filled with an openmc.Universe. You can only specify rotations " +
                   "on cells filled with universes, therefore this cell will not participate in " +
                   "the search.");
    }
  }
}
#endif
