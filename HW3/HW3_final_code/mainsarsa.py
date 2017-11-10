from Qlearning import *
from sarsa import *
from env import *
from joblib import Parallel, delayed
import joblib
from math import sqrt
import datetime

from utils import *


def plotforSARSA():
  data, mean, std = gatherdata("q3_final_SARSA/")
  plot_and_save('q3_sarsa_10k.png',mean,std,np.arange(200))



def parallelfun(obj,env,qlist,ind):
  print ind
  obj.run(env)
  joblib.dump(obj, "q3_final_SARSA/SARSA_all_instances_fin"+str(ind)+".jobpkl")  

def main():
  trials = 5
  alp, gam, eps, order = (0.05, 1, 0.5, 2)
  print datetime.datetime.now()
  mcql = [MountainCar() for count in xrange(trials)]
  QLearningList = [SARSA(mcql[count],order=order, alpha=alp, gamma=gam, epsilon=eps) for count in xrange(trials)]
  Parallel(n_jobs=-1,verbose=5)(delayed(parallelfun)(QLearningList[i],mcql[i],QLearningList,i) for i in xrange(trials))
  print datetime.datetime.now()


main()
# plotforSARSA()