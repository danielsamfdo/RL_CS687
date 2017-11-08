import random
import numpy as np
import copy
from collections import defaultdict
from datetime import datetime
from Fourier import *
import matplotlib
import numpy as np
import matplotlib.pyplot as plt
import random

def decision(probability):
    return random.random() < probability
class QLearning():
    def __init__(self, env, order=3, alpha=0.05, gamma=1, epsilon=0.1 ):
        self.total = order*order
        self.W = np.array([0.0] * (self.total * len(env.possible_actions)) )
        self.alpha = alpha
        self.gamma = gamma
        self.epsilon = epsilon
        self.env=env
        self.f = FirstOrderFourier(order)
        self.episode_count = 0
        self.episode_stop_count = 60
        self.time_steps = np.array([0.0] * self.episode_stop_count)
        self.episodes = np.array([0.0] * self.episode_stop_count)
        return
    
    def selectEGreedy(self, state, Egreedy=True):
        feature = self.phi(state[0],state[1])
        if(Egreedy and decision(self.epsilon)):
            # Return some value, exploring
            return random.choice(self.env.possible_actions)
        maxVal = None
        maxAct = None
        y = self.W*np.tile(feature,3)
        assert len(self.W) == len(np.tile(feature,3))
        result = [0*3]
        # result = np.add.reduceat(y, [0,self.total,2*self.total])
        result = np.array([np.sum(y[0:self.total]),np.sum(y[self.total:2*self.total]),np.sum(y[2*self.total:])])
        # assert result[0]==np.sum(y[0:self.total])
        # assert result[1]==np.sum(y[self.total:2*self.total])
        # assert result[2]==np.sum(y[2*self.total:])
        if(not Egreedy): # For Delat Update
            return np.max(result)
        # assert (np.argmax(result) - 1) in [-1,0,1]
        return np.argmax(result) - 1
        
    def phi(self, s,v):
        state = (s,v)
        return self.f.getFeature(state)
    
    def run(self, env):
        while(self.episode_count < self.episode_stop_count):
            env.reset_state()
            s = copy.deepcopy(env.state)
            while(not env.terminated):
                a = self.selectEGreedy(s)
                next_state,r = env.next_step(a)
                phi_v = self.phi(s[0],s[1])
                start_a = a+1
                delta_t_multiplier = self.selectEGreedy(next_state, Egreedy=False) 
                if(next_state[0]==self.env.goal_pos):
                    delta_t_multiplier = 0
                delta_t = (r + ( self.gamma * delta_t_multiplier) - (np.dot(self.W[start_a*self.total:start_a*self.total+self.total],phi_v)))
                self.W[start_a*self.total:start_a*self.total+self.total] += (self.alpha* delta_t * phi_v)
#                 print next_state,a_prime
                s = copy.deepcopy(next_state)
            self.episodes[self.episode_count] = -(float(env.time))
            self.time_steps[self.episode_count] = env.time
            self.episode_count+=1
