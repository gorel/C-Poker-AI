C-Poker-AI
==========

Update 2/4/2015: I finally got a chance to work on this again.  I've drastically increased the number of games I'm able to simulate when I found out that rand() isn't thread-safe.  I'm kind of surprised it has never been mentioned in any of my CS classes, and it makes me wonder how many other people are unaware of this.  Anyways, I've seen at least a 10-fold increase in how many games can be simulated.  Next time I get the chance to play around with this, I hope to improve the AI some more by adding in some machine learning.  Currently, the AI uses some hard-coded values for its logic.  I would like to find some good heuristics to use and change the AI to work like an SVM (or at least a Perceptron).

First I made a poker AI in Ruby... not fast enough!
Then I made another in Python... still too slow!
So this is my third poker AI written entirely in C using the 2+2 hand evaluator algorithm.

The HANDRANKS.DAT file needs to be generated from the given XPokerEval.TwoPlusTwo project.  You can find the source code for this at https://github.com/christophschmalhofer/poker/blob/master/XPokerEval/XPokerEval.TwoPlusTwo/

I'm using libcurl to handle HTTP GET and HTTP POST in order to interact with any poker server.  urlconnection.[ch] also provides the ability to convert this data into a JSON format using cJSON.

The AI uses Monte Carlo simulations to simulate as many games as it can before the timeout threshold is reached.  It spawns pthreads to do this work concurrently, which allows quite a few more games to be simulated in the time limit.

After doing some testing, the AI is able to simulate between 0.75M and 10M games per second on a mid-level laptop.  I have greatly improved the logging of the AI's choices to make it easy for someone to fine-tune their AI logic and see how it performs.  Here is an example of the output:
```
Hand    Community
3S TH   4D JH AS 8S
Current pot: 14100
Me:
        initial_stack= 4176, call_amount=  947
                stack= 1392, current_bet= 2784
Opponents:
      Will: initial_stack= 2096, stack= 1398, current_bet=  698
   Charlie: initial_stack= 4298, stack= 3691, current_bet= 1138
    Thomas: initial_stack= 4013, stack= 3578, current_bet=  435
    Oliver: initial_stack= 9380, stack=  335, current_bet= 9045
Simulated 4.184M games.
Win probability: 1.33%
Rate of return:  0.21
ACTION: FOLDING
```

This is the output that will be generated if the AI's logging level is set to LOGLEVEL_INFO.  When set to LOGLEVEL_NONE, you will see no output.  When set to LOGLEVEL_DEBUG, additional information about thread spawning will be available, including an identifier for each thread indicating when they start and stop, as well as how many games each thread was able to simulate.

Monte Carlo Simulation
======================
Using the AI Logic Test, it is easy to determine how efficient your machine is at simulating games using cat, grep, and the R programming language.

First, extract the number of simulations from ailogictest.log using tools like cat, grep, cut, sed, etc.  Send this to a new file.
Then, provided R is installed on your machine, you can use this command to get statistics about the results:
```R -q -e "x <- read.csv('OUTPUT.TXT', header = F); summary(x); sd(x[ , 1])"```
where OUTPUT.TXT is the name of the new file you created.
This will give you the five number summary and standard deviation of your results.  Here is what I have received when running AI Logic Test with numtrials=100 (Note: numbers represent millions of games simulated):
```
Min.   :0.703
1st Qu.:3.993
Median :4.928
Mean   :4.864
3rd Qu.:6.363
Max.   :8.575
[1] 2.220754
```

AI Logic Test
=============
The AI Logic Test is useful for refining the logic used by the AI when making the fold/call/raise decision.  The test will create a random game state and ask the AI for its decision.  It will then simulate the rest of the game to see if the AI made the right choice or not.  With this information, it is easy to refine the bounds for when the AI should fold, call, or raise.

At the end of the test, a table like the following will be output:
```
Results:
Folding:     24982 (33 wins,  3 losses,  0 unlucky)
Calling:    114857 (13 wins, 22 losses, 11 unlucky)
Raising:    144814 (13 wins,  4 losses,  1 unlucky)

Average logic score:  2846
```

Note that it is expected to have many "Call" losses if the AI could potentially win a lot of money and it doesn't cost much to play.
Example:
```
AI must call 100 for a potential pot of 20k
```

Poker Server
============
I've been playing around with Python a little bit, and I realized it is ridiculously easy to write a server that supports CGI with it (import CGIHTTPServer, obviously).  In src/pokerserver, you'll find server.py, which starts a server on the local machine.  It takes an optional parameter representing the port the server should run on (or it will default to 9313).  The server will then take HTTP requests at that hostname and port and allow users to calculate their win probabilities in various situations.

Yes, I know I'm awful at web development.  I know there are ways to do some of the things easier, but I was looking for an easy way to get something running without having to install any other dependencies -- all you need is Python!

Note: Be sure to set the location of the winprob binary in pokerserver/cgi/poker.py.  I made mine an absolute link in case I decide to move where the server code is.
