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
class SARSA():
    def __init__(self, env, order=3, alpha=0.05, gamma=1, epsilon=0.1 ):
        self.total = order*order
        self.W = np.array([0.0] * (self.total * len(env.possible_actions)) )
        self.alpha = alpha
        self.gamma = gamma
        self.epsilon = epsilon
        self.env=env
        self.f = FirstOrderFourier(order)
        self.episode_count = 0
        self.episode_stop_count = 200
        self.time_steps = np.array([0.0] * self.episode_stop_count)
        self.episodes = np.array([0.0] * self.episode_stop_count)
        self.debug = False
        return
    
    def selectEGreedy(self, state):
        feature = self.phi(state[0],state[1])
        if(decision(self.epsilon)):
            # Return some value, exploring
            if(self.debug):
                print "Taking Random"
            return random.choice(self.env.possible_actions)
        if(self.debug):
                print "Taking Greedy"
        maxVal = None
        maxAct = None
        y = self.W*np.tile(feature,3)
        # assert len(self.W) == len(np.tile(feature,3))
        result = [0*3]
        # result = np.add.reduceat(y, [0,self.total,2*self.total])
        result = np.array([np.sum(y[0:self.total]),np.sum(y[self.total:2*self.total]),np.sum(y[2*self.total:])])
        # assert result[0]==np.sum(y[0:self.total])
        # assert result[1]==np.sum(y[self.total:2*self.total])
#         print y[2*self.total:], result[2], np.sum(y[2*self.total:])
        # assert result[2]==np.sum(y[2*self.total:])
        val = random.choice(np.where(result == result.max())[0])
        return val - 1
        # assert (val - 1) in [-1,0,1]
        # return np.argmax(result) - 1
        
    def phi(self, s,v):
        state = (s,v)
        return self.f.getFeature(state)
    
    def run(self, env):
        while(self.episode_count < self.episode_stop_count):
            env.reset_state()
            s = copy.deepcopy(env.state)
            a = self.selectEGreedy(s)
            while(not env.terminated):
                next_state,r = env.next_step(a)
                a_prime = self.selectEGreedy(next_state)
                phi_v = self.phi(s[0],s[1])
                start = a_prime + 1
                start_a = a+1

                delta_t_multiplier = np.dot(self.W[start*self.total : start*self.total+self.total],self.phi(next_state[0],next_state[1]))
                if(next_state[0]==self.env.goal_pos):
                    delta_t_multiplier = 0
                delta_t = (r + (self.gamma * (delta_t_multiplier)) - (np.dot(self.W[start_a*self.total:start_a*self.total+self.total],phi_v)))
                self.W[start_a*self.total:start_a*self.total+self.total] += (self.alpha* delta_t * phi_v)
                if(self.debug==True):
                    print next_state,a_prime
                a = copy.deepcopy(a_prime)
                s = copy.deepcopy(next_state)
            self.episodes[self.episode_count] = -(float(env.time))
            self.time_steps[self.episode_count] = env.time
            self.episode_count+=1
