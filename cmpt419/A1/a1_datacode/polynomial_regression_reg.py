#!/usr/bin/env python

import assignment1 as a1
import numpy as np
import matplotlib.pyplot as plt
import pr_helper as prh
import sys
#----------------------
(countries, features, values) = a1.load_unicef_data()

targets = values[:,1]
x = values[:,7:]
x = a1.normalize_data(x)

N_TRAIN = 100
x_train = x[0:N_TRAIN,:]
x_test = x[N_TRAIN:,:]
t_train = targets[0:N_TRAIN]
t_test = targets[N_TRAIN:]
#-----------------------

fold=10
degree=2
totalSize=N_TRAIN
piece=N_TRAIN/fold
lmdas=[0,0.01,0.1,1,10,100,1000,10000]


lmdas_rms=range(0,len(lmdas))
for lmda_index in range(0,len(lmdas)):
    mean_rms_val= 0

    for i in range(0,10):
        valStart=piece*i
        x_trainSet = np.vstack((x_train[0:valStart,:],x_train[valStart+piece:,:]))
        x_validateSet = x_train[valStart:valStart+piece]
        t_trainSet =np.vstack((t_train[0:valStart,:],t_train[valStart+piece:,:]))
        t_validateSet=t_train[valStart:valStart+piece]
        phi=prh.GetPhi(x_trainSet,degree)
        w= prh.GetRegW(phi,t_trainSet,lmdas[lmda_index])
        phi_val=prh.GetPhi(x_validateSet,degree)
        rms_val = prh.GetPredictRMS(phi_val,w,t_validateSet)
        mean_rms_val += rms_val*0.1
        
    #save this lambda with its best validation rms   
    lmdas_rms[lmda_index] = (lmdas[lmda_index],mean_rms_val)
k=lmdas_rms[1][0]

lmda_list=[i[0] for i in lmdas_rms]
rms_list=[i[1] for i in lmdas_rms]
#Produce a plot of results.
# plt.plot(train_err.keys(), train_err.values())
# plt.plot(test_err.keys(), test_err.values())
plt.semilogx(lmda_list, rms_list)
#plt.plot(range(1,maxDegree+1), rms_train)
best_lmda=-1
best_rms=sys.float_info.max
for i in lmdas_rms:
    if(i[1]<best_rms):
        best_lmda=i[0]
        best_rms=i[1]
print('best lambda is '+str(best_lmda))
#----------------------------------
plt.ylabel('RMS')
plt.legend(['Lambda','Training error'])
plt.title('Fit with polynomials, with regularization')
plt.xlabel('lambda')
plt.show()
