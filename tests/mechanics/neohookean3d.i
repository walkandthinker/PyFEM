*** This is an input file for the compressive neohookean model

[mesh]
  type=asfem
  dim=3
  zmax=10.0
  nx=4
  ny=4
  nz=100
  meshtype=hex8
[end]

[dofs]
name=ux uy uz
[end]

[projection]
scalarmate=vonMises
rank2mate=stress strain
[end]

[elmts]
  [mechanics]
    type=mechanics
    dofs=ux uy uz
    mate=neohookean
    domain=alldomain
  [end]
[end]

[mates]
  [neohookean]
    type=neohookean
    params=100.0 0.3
  [end]
[end]



[bcs]
  [FixUx]
    type=dirichlet
    dof=ux
    boundary=left
    value=0.0
  [end]
  [FixUy]
    type=dirichlet
    dof=uy
    boundary=bottom
    value=0.0
  [end]
  [FixUz]
    type=dirichlet
    dof=uz
    boundary=back
    value=0.0
  [end]
  [loadUz]
    type=dirichlet
    dof=uz
    value=1.0*t
    boundary=front
  [end]
[end]

[timestepping]
  type=be
  dt=1.0e-3
  endtime=2.0e-3
  adaptive=false
  optiters=3
  dtmax=1.0e-1
[end]

[job]
  type=transient
  debug=dep
[end]
