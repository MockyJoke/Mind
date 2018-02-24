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

degree = 3
featureRange = range(0,8)
rms_train=range(0,8)
rms_test=range(0,8)
for i in featureRange:
    feature=x_train[:,i]
    feature_test=x_test[:,i]
    phi=prh.GetPhi(feature,degree)
    w= prh.GetW(phi,t_train)
    rms_train[i]= prh.GetPredictRMS(phi,w,t_train)
    phi_test=prh.GetPhi(feature_test,degree)
    rms_test[i]=prh.GetPredictRMS(phi_test,w,t_test)

xs=[x+8 for x in featureRange]

plt.bar(xs, rms_test,-0.4, color ="g")
plt.bar(xs, rms_train,0.4, color ="b")

#----------------------------------
plt.ylabel('RMS')
plt.legend(['Test error','Training error'])
plt.title('Training error & Testing error on various features')
plt.xlabel('Feature column')
plt.show()



#-------------------------------P2


degree = 3
featureRange = range(3,6)
rms_train=range(3,6)
rms_test=range(3,6)
for i in featureRange:
    feature=x_train[:,i]
    feature_test=x_test[:,i]
    featureMin=np.min(np.vstack((feature,feature_test)))
    featureMax=np.max(np.vstack((feature,feature_test)))
    

    phi=prh.GetPhi(feature,degree)
    w= prh.GetW(phi,t_train)
    predicts= np.dot(np.transpose(w),np.transpose(phi))
    phi_test=prh.GetPhi(feature_test,degree)
    predicts_test= np.dot(np.transpose(w),np.transpose(phi_test))
    x_fits=np.linspace(featureMin,featureMax)
    phi_fits=prh.GetPhi(np.reshape(x_fits,(50,1)),degree)
    predicts_fits= np.dot(np.transpose(w),np.transpose(phi_fits))

    plt.plot(x_fits, np.transpose(predicts_fits))
    plt.plot(feature,t_train,'bo')
    plt.plot(feature_test,t_test,'bo')
    plt.ylabel('mortality rate')
    plt.legend(['mortality rate','Training error'])
    plt.title('Fit degree '+str(i+8))
    plt.xlabel('x')
    plt.show()