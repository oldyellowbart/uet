# One more Simulation 
# Goal: estimate the probability of getting a sum of 7 when rolling two dice 
 trials = 30
 dices = 3
 x = matrix(sample(1:6,dices*trials, replace=TRUE), nrow=dices, ncol=trials)  
 y = colSums(x)  
 print(mean(y == 7))
 hist(x, col='cyan') 