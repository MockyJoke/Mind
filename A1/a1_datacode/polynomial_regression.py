#!/usr/bin/env python

import assignment1 as a1
import numpy as np
import matplotlib.pyplot as plt
import pr_helper as prh


(countries, features, values) = a1.load_unicef_data()

targets = values[:,1]
x = values[:,7:]
x = a1.normalize_data(x)

N_TRAIN = 100
x_train = x[0:N_TRAIN,:]
x_test = x[N_TRAIN:,:]
t_train = targets[0:N_TRAIN]
t_test = targets[N_TRAIN:]


#4.2


maxDegree = 6
rms_train=range(0,maxDegree)
rms_test=range(0,maxDegree)
for i in range(0,maxDegree):
    phi=prh.GetPhi(x_train,i+1)
    w= prh.GetW(phi,t_train)
    rms_train[i]= prh.GetPredictRMS(phi,w,t_train)
    phi_test=prh.GetPhi(x_test,i+1)
    rms_test[i]= prh.GetPredictRMS(phi_test,w,t_test)








#Produce a plot of results.
# plt.plot(train_err.keys(), train_err.values())
# plt.plot(test_err.keys(), test_err.values())
plt.plot(range(1,maxDegree+1), rms_test)
plt.plot(range(1,maxDegree+1), rms_train)

#----------------------------------
plt.ylabel('RMS')
plt.legend(['Test error','Training error'])
plt.title('Fit with polynomials, no regularization, normalized')
plt.xlabel('Polynomial degree')
plt.show()
