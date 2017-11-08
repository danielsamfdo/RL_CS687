import joblib
import numpy as np
from Qlearning import *
from sarsa import *
from env import *
def hyperparamtuning(ql):
  alpha = np.logspace(-2, -2, num=1)
  gamma = [1]#[0.1, 0.5, 0.8, 0.9, 1]
  epsilon = [0.1]#[0,0.1,0.3,0.4]
  order = [2]
  params = [(v1,v2,v3,v4) for v1 in alpha for v2 in gamma for v3 in epsilon for v4 in order]
  trials = 2
  t = np.zeros(200)
  testing = {}
  if(ql):
    t = np.zeros(60)
  for p in params:
      a,g,e,o = p
      print p
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
      testing[p] = testing[p]/trials
  prefix = "sarsa"
  if(ql):
    prefix = "ql"
  joblib.dump(testing, prefix +"hyptest_"+str(o)+".jobpkl")

hyperparamtuning(False)
hyperparamtuning(True)
