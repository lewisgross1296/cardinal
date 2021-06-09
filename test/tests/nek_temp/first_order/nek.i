[Problem]
  type = NekRSProblem
[]

[Mesh]
  type = NekRSMesh
  order = FIRST
  boundary = '1 2 3 4 5 6 7 8'
[]

[AuxVariables]
  [analytic]
  []
  [error_temp]
    order = CONSTANT
    family = MONOMIAL
  []
  [difference]
  []
[]

[ICs]
  [analytic]
    type = FunctionIC
    variable = analytic
    function = analytic
  []
[]

[AuxKernels]
  [error_temp]
    type = ElementL2ErrorFunctionAux
    variable = error_temp
    coupled_variable = temp
    function = analytic
  []
  [difference]
    type = ParsedAux
    variable = difference
    args = 'analytic temp'
    function = 'temp-analytic'
  []
[]

[Executioner]
  type = Transient

  [TimeStepper]
    type = NekTimeStepper
  []
[]

[Functions]
  [analytic]
    type = ParsedFunction
    value = 'exp(x)+sin(y)+x*y*z'
  []
[]

[Postprocessors]
  [l2_error_in_nek_temp]
    type = ElementL2Error
    variable = temp
    function = analytic
  []
  [max_diff]
    type = NodalExtremeValue
    variable = difference
    value_type = max
  []
  [min_diff]
    type = NodalExtremeValue
    variable = difference
    value_type = min
  []
[]

[Outputs]
  exodus = true
  execute_on = 'final'
  hide = 'flux_integral'
[]
