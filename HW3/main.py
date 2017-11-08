from Qlearning import *
from sarsa import *
from env import *
from joblib import Parallel, delayed
import joblib
from math import sqrt
import datetime
def plot(a):
    plt.plot(np.arange(len(a)), a)


# mc = MountainCar()
# s = SARSA(mc,order=2, alpha=0.05, gamma=1, epsilon=0.1)
# s.run(mc)
# plot(s.episodes)

def plot(a,fname):
    plt.plot(np.arange(len(a)), a)
    plt.xlabel('Episodes', fontsize=18)
    plt.ylabel('mean undiscounted return', fontsize=16)
    # axes = plt.gca()
    # axes.set_xlim([0,60])
    # axes.set_ylim([-1000,0])
    plt.savefig(fname)
    # plt.show()



trials = 10000


def gather_info(QLearningList):
  QLearningEpisodes = np.zeros(60)
  for QLearning_instance in QLearningList:
    QLearningEpisodes += np.array(QLearning_instance.episodes)
  QLearningEpisodes/=len(QLearningList)
  plot(QLearningEpisodes,'Qlearning_fin.png')

test = np.ones(trials)


def parallelfun(obj,env,qlist,ind):
  print ind
  obj.run(env)
  joblib.dump(obj, "Qlearning_all_instances_fin"+str(ind)+".jobpkl")  


print datetime.datetime.now()
mcql = [MountainCar() for count in xrange(trials)]
QLearningList = [QLearning(mcql[count],order=2, alpha=0.05, gamma=1, epsilon=0.1) for count in xrange(trials)]
Parallel(n_jobs=-1,verbose=5)(delayed(parallelfun)(QLearningList[i],mcql[i],QLearningList,i) for i in xrange(trials))



# def parallelfun(obj,env,qlist,ind):
#   obj.run(env)
#   test[ind] = 0
#   if(np.sum(test)==0):
#     joblib.dump(QLearningList, "Qlearning_all_instances_fin.jobpkl")  
#     gather_info(QLearningList)


# print datetime.datetime.now()
# mcql = [MountainCar() for count in xrange(trials)]
# QLearningList = [QLearning(mcql[count],order=2, alpha=0.05, gamma=1, epsilon=0.1) for count in xrange(trials)]
# # Parallel(n_jobs=10)(delayed(parallelfun)(QLearningList[i],mcql[i],QLearningList,i) )


# for i in range(trials):
#   print i
#   QLearningList[i].run(mcql[i])
# joblib.dump(QLearningList, "Qlearning_all_instances_fin.jobpkl")  
# gather_info(QLearningList)
print datetime.datetime.now()
