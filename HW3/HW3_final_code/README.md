Install Dependencies : numpy, matplotlib, joblib, pylab

pip install --user joblib


To Run Q Learning : 

Go to main.py

In main function set trials to required number of trials and set the hyper parameters like alpha, epsilon, gamma and Fourier Order

So if you want fourier of order "x", please set order = "x+1"

Also if you want to plot graph for the question make sure the directory q3_final_QL is empty and then run `python main.py`  will run for required trials but will not plot any graphs. It saves all the trials to the directory q3_final_QL. 

Graph - uses all files saved to q3_final_QL

To plot a graph, go to main.py and comment main() and uncomment plotforQLearning()

If you want to update the number of episodes QLearning runs for, in each trial. Go to Qlearning.py  and update episode_count from 60 to the desired number

If you want to set the number of process that is spawned inside training , go to main.py and set n_jobs=required_number_of_jobs


To Run SARSA : 

Go to mainsarsa.py

In main function set trials to required number of trials and set the hyper parameters like alpha, epsilon, gamma and Fourier Order

So if you want fourier of order "x", please set order = "x+1"

Also if you want to plot graph for the question make sure the directory q3_final_SARSA is empty and then run `python mainsarsa.py`  will run for required trials but will not plot any graphs. It saves all the trials to the directory q3_final_SARSA. 

Graph - uses all files saved to q3_final_SARSA

To plot a graph, go to main.py and comment main() and uncomment plotforSARSA()


If you want to set the number of process that is spawned inside training , go to main.py and set n_jobs=required_number_of_jobs
