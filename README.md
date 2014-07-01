C-Poker-AI
==========

First I made a poker AI in Ruby... not fast enough!
Then I made another in Python... still too slow!
So this is my third poker AI written entirely in C using the 2+2 hand evaluator algorithm.

The HANDRANKS.DAT file needs to be generated from the given XPokerEval.TwoPlusTwo project.  You can find the source code for this at https://github.com/christophschmalhofer/poker/blob/master/XPokerEval/XPokerEval.TwoPlusTwo/

I'm using libcurl to handle HTTP GET and HTTP POST in order to interact with any poker server.  urlconnection.[ch] also provides the ability to convert this data into a JSON format using cJSON.

The AI uses Monte Carlo simulations to simulate as many games as it can before the timeout threshold is reached.  It spawns pthreads to do this work concurrently, which allows quite a few more games to be simulated in the time limit.
