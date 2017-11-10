import numpy as np
class FirstOrderFourier():
    def __init__(self, order):
        self.order = order
        c = np.arange(order)
        self.c = np.array([ np.array([v1,v2]) for v1 in c for v2 in c ])
        self.length = len(self.c)
        return
    
    def normalize(self, pos, vel):
        pos = (pos + 1.2)/(1.7)
        vel = (vel + 0.07)/(0.14)
        return pos,vel
    
    
    def getFeature(self,s):
        v = np.ones(self.order*self.order) # 3 for len of actions
        pos_v, vel_v = s
        pos_v, vel_v = self.normalize(pos_v,vel_v)
        v = np.cos(np.sum(self.c * np.array([pos_v,vel_v]),axis=1) * np.pi)
        return v

        