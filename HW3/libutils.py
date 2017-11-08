import joblib
import numpy as np
from Qlearning import *
from env import *
def hyperparamtuning(ql):
  alpha = [0.001, 0.01, 0.1, 0.5]
  gamma = [0.1, 0.5, 0.8, 0.9, 1]
  epsilon = [0,0.1,0.3,0.4]
  order = [1,2]
  params = [(v1,v2,v3,v4) for v1 in alpha for v2 in gamma for v3 in epsilon for v4 in order]
  trials = 2
  t = np.zeros(200)
  testing = {}
  if(ql):
    t = np.zeros(60)
  for p in params:
      a,g,e,o = p
      for i in range(trials):
          l = None
          if(ql):
            env = MountainCar()
            l = Qlearning(env,order=o, alpha=a, gamma=g, epsilon=e)
            l.run(env)
          if(p in testing):
            testing[p] += l.episodes()
          else:
            testing[p] = l.episodes()
      testing[p] = testing[p]/trials
  joblib.dump(testing,"hyptest.jobpkl")