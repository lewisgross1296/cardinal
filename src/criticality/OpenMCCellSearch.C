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
    // get cell idx for the current cell_id
    int idx = 0;
    int cell_err = openmc_get_cell_index(cell_id, &idx)
    catchOpenMCError(cell_err, "get index for cell with ID " + std::to_string(cell_id));
    // get the fill for this cell
    int fill_type;
    int32_t * materials = nullptr;
    int n_materials = 0;
    int fill_err = openmc_cell_get_fill(idx, &fill_type, &materials, &n_materials);
    catchOpenMCError(fill_err, "get fill of cell " + printCell(cell_info));
    if( fill_type == openmc::Fill:UNIVERSE)
      _cell_index_list.push_back(idx) // add cell if it's in the cell_id_list and is filled with a universe
    else {
      mooseWarning("You have included " + std::tostring(cell_id) + " in the cell_id_list " +
                   "however it is not filled with an openmc.Universe. You can only specify " +
                   "rotations on cells filled with universes, therefore this cell will not" +
                   "participate in the search.")
    }
  }
}
#endif
