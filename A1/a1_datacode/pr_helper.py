#!/usr/bin/env python

import assignment1 as a1
import numpy as np
import matplotlib.pyplot as plt

def GetPhi(inputFeatures,maxDegree):
    numRows = np.size(inputFeatures,0)
    phi = np.ones((np.size(inputFeatures,0),1))
    for i in range(1, maxDegree + 1):
        phi = np.append(phi,np.power(inputFeatures,i),1)
    return phi

def GetW(phi, targets):
    w =np.dot(np.linalg.pinv(phi),targets)
    return w

def GetRegW(phi,targets,lmda):
    product=np.dot(np.transpose(phi),phi)
    size=np.size(product,1)
    identity= np.identity(size)
    t = np.add(np.multiply(lmda,identity),product)
    w = np.dot(np.dot(np.linalg.inv(t),np.transpose(phi)),targets)
    return w
    
def GetPredictRMS(phi,w,targets):
    predicts= np.dot(np.transpose(w),np.transpose(phi))
    errors=np.subtract(np.transpose(predicts),targets)
    rms = np.sqrt(np.mean(np.power(errors,2)))
    return rms

def GetSigmoidPhi(inputFeatures,miu,s):
    numRows = np.size(inputFeatures,0)
    phi = np.ones((np.size(inputFeatures,0),1))
    for u in miu:
        extra= np.power( np.add(1,np.exp(np.divide(np.subtract(u,inputFeatures),s))),-1)
        phi=np.append(phi,extra,1)
    return phi