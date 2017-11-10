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



def gatherdata(directory):
    files = os.listdir(directory)
    print len(files)
    sar = [None]*len(files)
    i = 0
    for filename in files:
        sar[i] = joblib.load(directory+filename)
        i+=1
    data = getdata(sar)
    std = np.std(data,axis=0)
    mean = np.mean(data,axis=0)
    print len(data)
    return data, mean, std


def getdata(sar):
    data = np.zeros((len(sar),len(sar[0].episodes)))
    i = 0
    for s in sar:
        data[i] = s.episodes
        i+=1
    return data[:10000]

def plot_and_save(fname, mean, std, ind):
    pl.figure(figsize=(7, 7))  # Don't create a humongous figure
    pl.errorbar(ind, mean, std, linestyle='None', color='r', marker='^')
    pl.ylim([-1000,0])
    pl.yticks(list(pl.yticks()[0]) + [-100,-200,-300,-400])
    pl.xlabel("episode")
    pl.ylabel("undiscounted mean rewards")
    pl.savefig(fname, format='png') 

