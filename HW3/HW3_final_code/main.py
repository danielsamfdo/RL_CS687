from Qlearning import *
from sarsa import *
from env import *
from joblib import Parallel, delayed
import joblib
from math import sqrt
import datetime

import os
import pylab as pl
import joblib

import matplotlib
import numpy as np
import matplotlib.pyplot as plt

from utils import *

def parallelfun(obj,env,qlist,ind):
  obj.run(env)
  joblib.dump(obj, "q3_final_QL/daniel_Qlearning_all_instances_fin"+str(ind)+".jobpkl")  


def main():
  trials = 5
  done = 0
  # HERE FOURIER ORDER = ORDER - 1, so the below runs for first order fourier
  alp, gam, eps, order = (0.05, 1, 0.5, 2)
  print datetime.datetime.now()
  mcql = [MountainCar() for count in xrange(trials-done)]
  QLearningList = [QLearning(mcql[count],order=order, alpha=alp, gamma=gam, epsilon=eps) for count in xrange(trials-done)]
  Parallel(n_jobs=-1)(delayed(parallelfun)(QLearningList[i],mcql[i],QLearningList,i) for i in xrange(trials-done))
  print datetime.datetime.now()



def plotforQLearning():
  data, mean, std = gatherdata("q3_final_QL/")
  plot_and_save('q3_ql_10k.png',mean,std,np.arange(60))



main()
# plotforQLearning()
