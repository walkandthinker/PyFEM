//****************************************************************
//* This file is part of the AsFem framework
//* A Simple Finite Element Method program (AsFem)
//* All rights reserved, Yang Bai @ CopyRight 2019
//* https://github.com/walkandthinker/AsFem
//* Licensed under GNU GPLv3, please see LICENSE for details
//* https://www.gnu.org/licenses/gpl-3.0.en.html
//****************************************************************

#include "MessagePrinter/MessagePrinter.h"

void Msg_Input_LineError(const PetscInt &linenum){
    PetscPrintf(PETSC_COMM_WORLD,"*** Error: input file error in line-%4d                        !!!   ***\n",linenum);
    // PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
}