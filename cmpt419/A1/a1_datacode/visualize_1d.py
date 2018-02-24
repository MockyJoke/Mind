#!/usr/bin/env python

import assignment1 as a1
import numpy as np
import matplotlib.pyplot as plt


(countries, features, values) = a1.load_unicef_data()

targets = values[:,1]
x = values[:,7:]
#x = a1.normalize_data(x)

N_TRAIN = 100;
# Select a single feature.
x_train = x[0:N_TRAIN,10]
t_train = targets[0:N_TRAIN]


# Plot a curve showing learned function.
# Use linspace to get a set of samples on which to evaluate
x_ev = np.linspace(np.asscalar(min(x_train)), np.asscalar(max(x_train)), num=500)

# TO DO:: Put your regression estimate here in place of x_ev.
# Evaluate regression on the linspace samples.
y_ev = np.random.random_sample(x_ev.shape)
y_ev = 100*np.sin(x_ev)


plt.plot(x_ev,y_ev,'r.-')
plt.plot(x_train,t_train,'bo')
plt.title('A visualization of a regression estimate using random outputs')
plt.show()
