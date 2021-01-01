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
//+++ Date   : 2020.07.10
//+++ Purpose: Define [bcs] sub block for our input file
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once


#include <iostream>
#include <string>
#include <vector>


#include "BCSystem/BCType.h"

using namespace std;


class BCBlock{
public:
    BCBlock(){
        _BCBlockName.clear();
        _BCTypeName.clear();
        _BCType=BCType::NULLBC;
        _DofName.clear();
        _DofID=-1;
        _BCValue=0.0;
        _BoundaryNameList.clear();
        _IsTimeDependent=false;
    }

    string         _BCBlockName;
    string         _BCTypeName;
    BCType         _BCType;
    string         _DofName;
    int            _DofID;
    double         _BCValue;
    vector<string> _BoundaryNameList;// it could be either an element set or a node set
    bool           _IsTimeDependent;

    void Init(){
        _BCBlockName.clear();
        _BCTypeName.clear();
        _BCType=BCType::NULLBC;
        _DofName.clear();
        _DofID=-1;
        _BCValue=0.0;
        _BoundaryNameList.clear();
        _IsTimeDependent=false;
    }
    
};