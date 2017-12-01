import joblib
import os

files = os.listdir('sarsa/')
for filename in files:
    d = joblib.load('sarsa/'+filename)
    joblib.dump(d,filename='sarsa/'+filename,protocol=2)