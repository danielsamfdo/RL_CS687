import numpy as np
import copy
from collections import defaultdict
from datetime import datetime

import matplotlib
import numpy as np
import matplotlib.pyplot as plt

class MountainCar():
    def __init__(self):
        self.min_x = -1.2
        self.max_x = 0.6
        self.goal_pos = 0.5
        self.possible_actions = [-1,0,1]
        self.reset_state()
        return
    
    def next_step(self,action):
        self.time+=1
        self.reward = -1
        self.velocity += 0.001*action - 0.0025*np.cos(3*self.pos)
        if(self.velocity<-0.07):
            self.velocity = -0.07
        if(self.velocity>0.07):
            self.velocity = 0.07
        self.pos = self.pos + self.velocity
        
        
        if(self.pos<-1.2):
            self.pos = -1.2
        if(self.pos>0.5):
            self.pos = 0.5
            self.reward = 0
            self.terminated = True
        
        if(self.time == 20000):
            self.terminated = True
        
        self.state = (self.pos,self.velocity)
        return (self.state), self.reward
    
    def reset_state(self):
        self.pos = -0.5
        self.velocity = 0
        self.state = (self.pos,self.velocity)
        self.time = 0
        self.terminated = False
        