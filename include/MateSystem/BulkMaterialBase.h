//****************************************************************
//* This file is part of the AsFem framework
//* A Simple Finite Element Method program (AsFem)
//* All rights reserved, Yang Bai @ CopyRight 2021
//* https://github.com/yangbai90/AsFem.git
//* Licensed under GNU GPLv3, please see LICENSE for details
//* https://www.gnu.org/licenses/gpl-3.0.en.html
//****************************************************************
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++ Author : Yang Bai
//+++ Date   : 2021.04.04
//+++ Purpose: Define the base material abstract class, all the
//+++          materials should inherit this class!
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once


#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// For AsFem's own header files
#include "MateSystem/MateTypeDefine.h"

using namespace std;

class BulkMaterialBase{
public:
    virtual void InitMaterialProperties(const int &nDim,const Vector3d &gpCoord,const vector<double> &InputParams,
                                        const vector<double> &gpU,const vector<double> &gpUdot,
                                        const vector<Vector3d> &gpGradU,const vector<Vector3d> &gpGradUdot,
                                        Materials &Mate)=0;// initial material properties

    virtual void ComputeMaterialProperties(const double &t,const double &dt,const int &nDim,
                                           const Vector3d &gpCoord,const vector<double> &InputParams,
                                           const vector<double> &gpU,const vector<double> &gpUOld,
                                           const vector<double> &gpUdot,const vector<double> &gpUdotOld,
                                           const vector<Vector3d> &gpGradU,const vector<Vector3d> &gpGradUOld,
                                           const vector<Vector3d> &gpGradUdot,const vector<Vector3d> &gpGradUdotOld,
                                           const Materials &MateOld,Materials &Mate)=0;// the calculation of different materials

};