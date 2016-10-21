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

feature= x_train[:,3]
feature_test= x_test[:,3]
phi=prh.GetSigmoidPhi(feature,[100,10000],2000.0)
w= prh.GetW(phi,t_train)


rms_train = prh.GetPredictRMS(phi,w,t_train)
phi_test=prh.GetSigmoidPhi(feature_test,[100,10000],2000.0)
rms_test=prh.GetPredictRMS(phi_test,w,t_test)

featureMin=np.min(feature)
featureMax=np.max(feature)
x_fits=np.linspace(featureMin,featureMax)
phi_fits=prh.GetSigmoidPhi(np.reshape(x_fits,(50,1)),[100,10000],2000.0)
predicts_fits= np.dot(np.transpose(w),np.transpose(phi_fits))

#Produce a plot of results.
# plt.plot(train_err.keys(), train_err.values())
# plt.plot(test_err.keys(), test_err.values())
plt.plot(x_fits, np.transpose(predicts_fits))
#plt.plot(range(1,maxDegree+1), rms_train)

#----------------------------------
plt.ylabel('mortality rate')
plt.legend(['mortality rate','Training error'])
plt.title('Fit with sigmoid, no regularization, feature 11 GNI')
plt.xlabel('GNI per capita')
plt.show()

#----------------------------------------------------------G2

plt.bar(0, rms_train,color="r")
plt.bar(1,rms_test)
#plt.plot(range(1,maxDegree+1), rms_train)

#----------------------------------
plt.ylabel('rms')
plt.legend(['training error','testing error'])
plt.title('Fit with sigmoid, no regularization, feature 11 GNI')
plt.xlabel('RMS, training vs testing')
plt.show()