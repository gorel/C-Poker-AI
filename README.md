C-Poker-AI
==========

First I made a poker AI in Ruby... not fast enough!
Then I made another in Python... still too slow!
So this is my third poker AI written entirely in C using the 2+2 hand evaluator algorithm.

The HANDRANKS.DAT file needs to be generated from the given XPokerEval.TwoPlusTwo project.  You can find the source code for this at https://github.com/christophschmalhofer/poker/blob/master/XPokerEval/XPokerEval.TwoPlusTwo/

I'm using libcurl to handle HTTP GET and HTTP POST in order to interact with any poker server.  urlconnection.[ch] also provides the ability to convert this data into a JSON format using cJSON.

The AI uses Monte Carlo simulations to simulate as many games as it can before the timeout threshold is reached.  It spawns pthreads to do this work concurrently, which allows quite a few more games to be simulated in the time limit.

After doing some testing, the AI is able to simulate between 150k and 600k games per second.  I have greatly improved the logging of the AI's choices to make it easy for someone to fine-tune their AI logic and see how it performs.  Here is an example of the output:
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
Simulated 321k games.
Win probability: 1.33%
Rate of return:  0.21
ACTION: FOLDING
```

This is the output that will be generated if the AI's logging level is set to LOGLEVEL_INFO.  When set to LOGLEVEL_NONE, you will see no output.  When set to LOGLEVEL_DEBUG, additional information about thread spawning will be available, including an identifier for each thread indicating when they start and stop, as well as how many games each thread was able to simulate.
