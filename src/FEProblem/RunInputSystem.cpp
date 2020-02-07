//****************************************************************
//* This file is part of the AsFem framework
//* A Simple Finite Element Method program (AsFem)
//* All rights reserved, Yang Bai @ CopyRight 2019
//* https://github.com/walkandthinker/AsFem
//* Licensed under GNU GPLv3, please see LICENSE for details
//* https://www.gnu.org/licenses/gpl-3.0.en.html
//****************************************************************

#include "FEProblem/FEProblem.h"

bool FEProblem::RunInputSystem(){
    return _inputSystem.ReadInputFile(_mesh,
                                      _dofHandler,
                                      _elmtSystem,
                                      _mateSystem,
                                      _bcSystem,
                                      _icSystem,
                                      _solution,
                                      _fe,
                                      _nonlinearsolverblock,
                                      _timesteppingblock,
                                      _jobBlock);
}