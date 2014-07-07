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
9S 6C   8S TS 2S JS
Simulated 592k games.
Win probability: 86.97%
Rate of return:  1.27
ACTION: BETTING 1808
```

This is the output that will be generated if the AI's logging level is set to LOGLEVEL_INFO.  When set to LOGLEVEL_NONE, you will see no output.  When set to LOGLEVEL_DEBUG, additional information about thread spawning will be available, including an identifier for each thread indicating when they start and stop, as well as how many games each thread was able to simulate.
