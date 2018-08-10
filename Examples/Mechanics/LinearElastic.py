#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jul  6 14:03:25 2018

QQ group: 797998860 
@email: walkandthinker@gmail.com
@author: FlyFox
"""

import numpy as np
import matplotlib.pyplot as plt
import sys

from Mesh import *
from GaussPoint import *
from ShapeFuns import *

def Welcome():
    print('********************************************')
    print('*** Welcome to use this simple script!   ***')
    print('*** Simple fem code for laplace equation ***')
    print('*** Author: FlyFox                       ***')
    print('*** Email: walkandthinker@gmail.com      ***')
    print('*** QQ group: 797998860                  ***')
    print('********************************************')
############################################################
### user element for linear elastic problem
############################################################
def elmt(elCoords,elU):
    nNodes=np.size(elCoords,0)
    nDofs=2*nNodes

    ngp=2
    if nNodes>=8:
        ngp=3
    
    gs=Int2D(ngp)
    Lint=ngp*ngp

    K=np.zeros((nDofs,nDofs))
    RHS=np.zeros(nDofs)
    Proj=np.zeros((3+3+2)*Lint)# stress+strain+vonMises+hydrostatic stress

    E=10.0;nu=0.3 # Young's modulus and poisson ratio
    D=np.zeros((3,3)) # Constitutive law
    D[0,0]=1.0;D[0,1]=nu
    D[1,0]=nu ;D[1,1]=1.0
    D[2,2]=0.5*(1-nu)
    D=D*E/(1-nu**2)

    for gpInd in range(Lint):
        xi =gs[gpInd,1]
        eta=gs[gpInd,2]
        shp,xsj=Shp2D(elCoords,xi,eta)
        JxW=gs[gpInd,0]*xsj

        B=np.zeros((3,2*nNodes))
        S=np.zeros((3,2*nNodes))
        for i in range(nNodes):
            B[0,2*i  ]=shp[i,1]
            B[0,2*i+1]=0.0

            B[1,2*i  ]=0.0
            B[1,2*i+1]=shp[i,2]

            B[2,2*i  ]=shp[i,2]
            B[2,2*i+1]=shp[i,1]
        Bt=B.transpose()
        S=np.dot(D,B)
        strain=np.zeros(3)
        stress=np.zeros(3)
        
        for i in range(nDofs):
            strain[0]+=B[0,i]*elU[i]
            strain[1]+=B[1,i]*elU[i]
            strain[2]+=B[2,i]*elU[i]

            stress[0]+=S[0,i]*elU[i]
            stress[1]+=S[1,i]*elU[i]
            stress[2]+=S[2,i]*elU[i]
        #############################
        ### For projection value
        # For stress
        Proj[gpInd*(3+3+2)+1-1]=stress[1-1]
        Proj[gpInd*(3+3+2)+2-1]=stress[2-1]
        Proj[gpInd*(3+3+2)+3-1]=stress[3-1]
        # For strain
        Proj[gpInd*(3+3+2)+4-1]=strain[1-1]
        Proj[gpInd*(3+3+2)+5-1]=strain[2-1]
        Proj[gpInd*(3+3+2)+6-1]=strain[3-1]
        # For von Mises
        Proj[gpInd*(3+3+2)+7-1]=np.sqrt(stress[0]**2+stress[1]**2+3*stress[2]**2-stress[0]*stress[1])
        Proj[gpInd*(3+3+2)+8-1]=(stress[0]+stress[1])/2.0


        C=np.dot(Bt,S)
        for i in range(2*nNodes):
            # For residual
            for k in range(3):
                RHS[i]+=-Bt[i,k]*stress[k]*JxW
            for j in range(2*nNodes):
                K[i,j]+=C[i,j]*JxW
    return K,RHS,Proj
#######################################################
def FormKR(mesh,U):
    nElmts=mesh.nElmts
    nNodesPerElmt=mesh.nNodesPerElmts
    nDofs=mesh.nNodes*2
    AMATRX=np.zeros((nDofs,nDofs)) # [K]{u}=F-->here AMATRX is K
    RHS=np.zeros(nDofs)
    Proj=np.zeros((nElmts,(3+3+2)*8))

    for e in range(nElmts):
        elConn=mesh.Conn[e,:]
        elCoords=mesh.NodeCoords[elConn-1]
        elU=np.zeros(2*nNodesPerElmt)
        for i in range(nNodesPerElmt):
            elU[2*i  ]=U[2*(elConn[i]-1)  ]
            elU[2*i+1]=U[2*(elConn[i]-1)+1]

        k,rhs,proj=elmt(elCoords,elU)

        i=len(proj)
        Proj[e,0:i]=proj[:]

        # Assemble k,rhs to system matrix
        for i in range(nNodesPerElmt):
            iInd=elConn[i]-1
            RHS[2*iInd  ]+=rhs[2*i  ]
            RHS[2*iInd+1]+=rhs[2*i+1]
            for j in range(nNodesPerElmt):
                jInd=elConn[j]-1
                AMATRX[2*iInd  ,2*jInd  ]+=k[2*i  ,2*j  ]
                AMATRX[2*iInd  ,2*jInd+1]+=k[2*i  ,2*j+1]
                AMATRX[2*iInd+1,2*jInd  ]+=k[2*i+1,2*j  ]
                AMATRX[2*iInd+1,2*jInd+1]+=k[2*i+1,2*j+1]

    return AMATRX,RHS,Proj
#######################################################
def ApplyDispBC(sidename,dofname,mesh,U,value):
    if sidename=='left':
        # For left edge
        BCConn=mesh.LeftBCConn
    elif sidename=='right':
        BCConn=mesh.RightBCConn
    elif sidename=='bottom':
        BCConn=mesh.BottomBCConn
    elif sidename=='top':
        BCConn=mesh.TopBCConn
    else:
        sys.exit('Side name=%s is invalid!!!'%(sidename))
    
    if dofname=='ux':
        component=1
    elif dofname=='uy':
        component=2
    else:
        sys.exit('dof name=%s in invalid!!!'%(dofname))
    
    ne=np.size(BCConn,0)
    nNodesPerBCElmt=np.size(BCConn,1)
    for e in range(ne):
        elConn=BCConn[e,:]-1
        for i in range(nNodesPerBCElmt):
            iInd=2*elConn[i]+component-1
            U[iInd]=value

    return U
########################################################
### Apply constrain condition
########################################################
def ApplyConstrainBC(sidename,dofname,mesh,AMATRIX,RHS):
    if sidename=='left':
        # For left edge
        BCConn=mesh.LeftBCConn
    elif sidename=='right':
        BCConn=mesh.RightBCConn
    elif sidename=='bottom':
        BCConn=mesh.BottomBCConn
    elif sidename=='top':
        BCConn=mesh.TopBCConn
    else:
        sys.exit('Side name=%s is invalid!!!'%(sidename))
    
    if dofname=='ux':
        component=1
    elif dofname=='uy':
        component=2
    else:
        sys.exit('dof name=%s in invalid!!!'%(dofname))
    
    ne=np.size(BCConn,0)
    penalty=1.0e10
    for e in range(ne):
        elConn=BCConn[e,:]-1
        for i in range(len(elConn)):
            iInd=2*elConn[i]+component-1
            AMATRIX[iInd,iInd]=penalty
            RHS[iInd]=0.0
    
    return AMATRIX,RHS
##########################################################


if __name__=='__main__':
    Welcome()
    nx=5;ny=1
    W=5.0;H=2.0
    mesh=Mesh2D(nx,ny,0.0,W,0.0,H,'quad4')
    mesh.CreateMesh()
    mesh.SplitBCMesh()
    
    nDofs=2*mesh.nNodes
    U0=np.zeros(nDofs)
    U=np.zeros(nDofs)


    iters=0;MaxIters=2;IsConvergent=False
    atol=1.0e-12;rtol=1.0e-9 # absolute error and relative error
    iters=0;IsConvergent=False
    while iters<MaxIters and (not IsConvergent):
        U=ApplyDispBC('right','ux',mesh,U,1.0)
        AMATRIX,RHS,Proj=FormKR(mesh,U)
        ## For constrain condition
        AMATRIX,RHS=ApplyConstrainBC('left','ux',mesh,AMATRIX,RHS)
        AMATRIX,RHS=ApplyConstrainBC('bottom','uy',mesh,AMATRIX,RHS)
        

        dU=np.linalg.solve(AMATRIX,RHS)
        U[:]+=dU[:]

        if iters==0:
            R0_norm=np.linalg.norm(RHS)
            dU0_norm=np.linalg.norm(dU)
        R_norm=np.linalg.norm(RHS)
        dU_norm=np.linalg.norm(dU)

        print('iteration=%2d, |R0|=%.5e <--->|R|=%.5e, |dU0|=%.5e <--->|dU=|%.5e'%(iters,R0_norm,R_norm,dU0_norm,dU_norm))
        if (R_norm<rtol*R0_norm or R_norm<atol) or (dU_norm<rtol*dU0_norm or dU_norm<atol):
            IsConvergent=True
            break
        iters+=1
        


