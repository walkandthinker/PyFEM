[mesh]
  type=asfem
  dim=2
  xmax=5.0
  ymax=5.0
  nx=50
  ny=50
  meshtype=quad4
[end]

[dofs]
name=ux uy
[end]

[elmts]
  [mysolid]
    type=mechanics
    dofs=ux uy
    mate=mymate
  [end]
[end]

[mates]
  [mymate]
    type=linearelastic
    params=210.0 0.3
  [end]
[end]

[bcs]
  [fixbottomX]
    type=dirichlet
    dof=ux
    value=0.0
    boundary=bottom
  [end]
  [fixbottomY]
    type=dirichlet
    dof=uy
    value=0.0
    boundary=bottom
  [end]
  [loadY]
    type=dirichlet
    dof=uy
    value=0.1
    boundary=top
  [end]
[end]

[projection]
name=vonMises stress_xx stress_yy stress_xy
[end]

[job]
  type=static
  debug=dep
[end]