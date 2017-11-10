import joblib
import numpy as np
from Qlearning import *
from sarsa import *
from env import *
import pylab as pl
import joblib
from datetime import datetime

import matplotlib
import numpy as np
import matplotlib.pyplot as plt
def plot(a):
    plt.plot(np.arange(len(a)), a)

def plot_and_save(fname, mean, std, ind):
    pl.figure(figsize=(7, 7))  # Don't create a humongous figure
    pl.errorbar(ind, mean, std, linestyle='None', color='r', marker='^')
    pl.ylim([-3000,100])
    pl.yticks(list(pl.yticks()[0]) + [-100,-200,-300,-400])
    pl.xlabel("episode")
    pl.ylabel("undiscounted mean rewards")
    pl.savefig(fname, format='png') 
    # pl.show()

def hyperparamtuning(ql):
  alpha = [0.07,0.007,0.0007,0.00007]#np.logspace(-2, -2, num=1)
  gamma = [1]#[0.1, 0.5, 0.8, 0.9, 1]
  epsilon = [0.000001,0.00001,0.0001,0.001,0.01]#[0,0.1,0.3,0.4]
  order = [6,2,4]
  params = [(v1,v2,v3,v4) for v1 in alpha for v2 in gamma for v3 in epsilon for v4 in order]
  trials = 4
  t = np.zeros(200)
  prefix = "sarsa"
  if(ql):
    prefix = "ql"
  testing = {}
  if(ql):
    t = np.zeros(200)
  for p in params:
      a,g,e,o = p
      print p
      r = [None]*trials
      for i in range(trials):
          l = None
          env = MountainCar()
          if(ql):
            l = QLearning(env,order=o, alpha=a, gamma=g, epsilon=e)
          else:
            l = SARSA(env,order=o, alpha=a, gamma=g, epsilon=e)            
          l.run(env)
          if(p in testing):
            testing[p] += l.episodes
          else:
            testing[p] = l.episodes
          r[i] = l.episodes
      testing[p] = testing[p]/trials
      plot_and_save(prefix +"hyptest_order_"+str(o-1)+"al_"+str(a)+"eps_"+str(e)+".png", np.mean(r,axis=0),np.std(r,axis=0),np.arange(len(l.episodes)))
      print np.mean(testing[p])
  
  joblib.dump(testing, prefix +"hyptest_"+str(o)+".jobpkl")

# hyperparamtuning(False)
# hyperparamtuning(True)
