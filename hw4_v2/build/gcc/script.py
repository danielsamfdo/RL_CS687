import matplotlib
import matplotlib.pyplot as pl
import pandas as pd

def plot_and_save(fname, mean, std, ind):
    pl.figure(figsize=(7, 7))  # Don't create a humongous figure
    pl.errorbar(ind, mean, std, linestyle='None', color='b', marker='^')
    pl.xlabel("episode")
    pl.ylabel("undiscounted mean rewards")
    pl.savefig(fname, format='png')


df = pd.read_csv("out_QLambda_Acrobot.csv",sep=",")
plot_and_save("x.png", df["Mean Return"], df["Standard Deviation"], range(len(df)))
