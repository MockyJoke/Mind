#!/usr/bin/env python

import assignment1 as a1
import numpy as np
import matplotlib.pyplot as plt
import pr_helper as prh
#----------------------
(countries, features, values) = a1.load_unicef_data()

targets = values[:,1]
x = values[:,7:]
#x = a1.normalize_data(x)

N_TRAIN = 100
x_train = x[0:N_TRAIN,:]
x_test = x[N_TRAIN:,:]
t_train = targets[0:N_TRAIN]
t_test = targets[N_TRAIN:]
#-----------------------

fold=10
totalSize=np.size(x,0)
piece=totalSize/fold

for i in range(0,10):
    valStart=piece*i
    trainSet = np.vstack((x[0:valStart,:],x[valStart+piece:,:]))
    validateSet = x[valStart:valStart+piece]
    print(trainSet)
    print(validateSet)

