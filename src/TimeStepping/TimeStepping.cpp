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
//+++ Date   : 2020.12.29
//+++ Purpose: Define time stepping system for transient analysis
//+++          in AsFem
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "TimeStepping/TimeStepping.h"

TimeStepping::TimeStepping(){
    _Dt=1.0e-5;
    _FinalT=1.0e-3;
    _Adaptive=false;
    _TotalSteps=-1;
    _TimeSteppingType=TimeSteppingType::BACKWARDEULER;
    _TimeSteppingTypeName="backward-euler";
    _LinearSolverName="ksp";
    _GrowthFactor=1.1;
    _CutBackFactor=0.85;
    _OptIters=3;
    _DtMax=1.0e2;
    _DtMin=1.0e-12;
    //**********************************
    //*** for nonlinear solver
    //**********************************
    _Rnorm0=1.0;_Rnorm=1.0;
    _Enorm0=1.0;_Enorm=1.0;
    _RAbsTol=4.0e-8;_RRelTol=1.0e-9;
    _EAbsTol=1.0e-19;_ERelTol=1.0e-20;
    _MaxIters=25;_Iters=0;
    _STol=1.0e-16;
    _SolverType=NonlinearSolverType::NEWTONLS;
    _PCTypeName="lu";
    _LinearSolverName="petsc";
}

//****************************************************
void TimeStepping::SetOpitonsFromTimeSteppingBlock(TimeSteppingBlock &timeSteppingBlock){
    _Dt=timeSteppingBlock._Dt;
    _DtMin=timeSteppingBlock._DtMin;
    _DtMax=timeSteppingBlock._DtMax;
    _FinalT=timeSteppingBlock._FinalT;
    _TotalSteps=static_cast<long int>(_FinalT/_Dt);
    _TimeSteppingType=timeSteppingBlock._TimeSteppingType;
    _TimeSteppingTypeName=timeSteppingBlock._TimeSteppingTypeName;
    _Adaptive=timeSteppingBlock._Adaptive;
    _GrowthFactor=timeSteppingBlock._GrowthFactor;
    _CutBackFactor=timeSteppingBlock._CutBackFactor;
    _OptIters=timeSteppingBlock._OptIters;
}
void TimeStepping::SetOptionsFromNonlinearSolverBlock(NonlinearSolverBlock &nonlinearsolverblock){
    _SolverType=nonlinearsolverblock._SolverType;
    _MaxIters=nonlinearsolverblock._MaxIters;
    _RAbsTol=nonlinearsolverblock._RAbsTol;
    _RRelTol=nonlinearsolverblock._RRelTol;
    _STol=nonlinearsolverblock._STol;

    _SolverType=nonlinearsolverblock._SolverType;
    _PCTypeName=nonlinearsolverblock._PCTypeName;
    _LinearSolverName=nonlinearsolverblock._LinearSolverName;
}
//*******************************************************
void TimeStepping::PrintTimeSteppingInfo()const{
    MessagePrinter::PrintNormalTxt("Time stepping system information summary:");
    MessagePrinter::PrintNormalTxt("  stepping method ="+_TimeSteppingTypeName);
    if(_Adaptive){
        MessagePrinter::PrintNormalTxt("  adaptive is enabled, optimal iters ="+to_string(_OptIters));
        MessagePrinter::PrintNormalTxt("  adaptive growth factor="+to_string(_GrowthFactor)+", cut factor="+to_string(_CutBackFactor));
    }
    else{
        MessagePrinter::PrintNormalTxt("  adaptive is disabled");
    }
    char buff[20];
    snprintf(buff,20,"%14.5e",_Dt);
    string str;
    str="  init delta T="+string(buff);
    snprintf(buff,20,"%14.5e",_FinalT);
    str+=", final T="+string(buff);
    MessagePrinter::PrintNormalTxt(str);

    snprintf(buff,20,"%14.5e",_DtMax);
    str="  max delta T="+string(buff);
    snprintf(buff,20,"%14.5e",_DtMin);
    str+=", min delta T="+string(buff);
    MessagePrinter::PrintNormalTxt(str);
    MessagePrinter::PrintDashLine();
}
//****************************************
void TimeStepping::ReleaseMem(){
    TSDestroy(&_ts);
}