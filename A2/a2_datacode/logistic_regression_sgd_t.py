#!/usr/bin/env python

# Run logistic regression training.

import numpy as np
import scipy.special as sps
import matplotlib.pyplot as plt
import assignment2 as a2


# Maximum number of iterations.  Continue until this limit, or when error change is below tol.
max_iter = 500
tol = 0.00001

# Step size for gradient descent.
eta_arr = [0.5,0.3,0.1,0.05,0.01]



# Load data.
data = np.genfromtxt('data.txt')

# Data matrix, with column of ones at end.
X = data[:,0:3]
# Target values, 0 for class 1, 1 for class 2.
t = data[:,3]
# For plotting data
class1 = np.where(t==0)
X1 = X[class1]
class2 = np.where(t==1)
X2 = X[class2]





DATA_FIG = 1

# Set up the slope-intercept figure
# SI_FIG = 2
# plt.figure(SI_FIG)
# plt.rcParams.update({'font.size': 15})
# plt.title('Separator in slope-intercept space')
# plt.xlabel('slope')
# plt.ylabel('intercept')
# plt.axis([-5, 5, -10, 0])
ws=[]

for eta in eta_arr:
    # Initialize w.
    w = np.array([0.1, 0, 0])

    # Error values over all iterations.
    e_all = []
    for iter in range (0,max_iter):
        index=range(np.size(X,0))
        np.random.shuffle(index)
        for i in range (np.size(X,0)):
            # Compute output using current w on all data X.
            k=np.dot(X[index[i]],w)
            y = sps.expit(k)
            # e is the error, negative log-likelihood (Eqn 4.90)
            e = -np.mean(np.multiply(t[index[i]],np.log(y)) + np.multiply((1-t[index[i]]),np.log(1-y)))

            

            # Gradient of the error, using Eqn 4.91
            grad_e = np.multiply((y - t[index[i]]), X[index[i]].T)

            # Update w, *subtracting* a step in the error derivative since we're minimizing
            w_old = w
            w = w - eta*grad_e
            
            # Plot current separator and data.  Useful for interactive mode / debugging.
            # plt.figure(DATA_FIG)
            # plt.clf()
            # plt.plot(X1[:,0],X1[:,1],'b.')
            # plt.plot(X2[:,0],X2[:,1],'g.')
            # a2.draw_sep(w)
            # plt.axis([-5, 15, -10, 10])

            
            # Add next step of separator in m-b space.
            # plt.figure(SI_FIG)
            # a2.plot_mb(w,w_old)

                
            # Print some information.
            print 'epoch {0:d}, sample {3:d}, negative log-likelihood {1:.4f}, w={2}'.format(iter, e, w.T,i)
        # Add this error to the end of error vector.
        e_all.append(e)
        # Stop iterating if error doesn't change more than tol.
        if iter>0:
            if np.absolute(e-e_all[iter-1]) < tol:
                break


    # Plot error over iterations
    #plt.figure()
    # plt.plot(e_all,label='{0:f}'.format(eta))
    # plt.ylabel('Negative log likelihood')
    # plt.title('Training logistic regression')
    # plt.xlabel('Epoch')
    plt.figure(DATA_FIG)
    plt.clf()
    plt.plot(X1[:,0],X1[:,1],'b.')
    plt.plot(X2[:,0],X2[:,1],'g.')
    a2.draw_sep(w)
    plt.axis([-5, 15, -10, 10])
    plt.show()
    ws.append(w)
for k in ws:
    print k
#plt.show()
