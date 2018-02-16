CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)

cardtest1: cardtest1.c dominion.o rngs.o
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)

cardtest2: cardtest2.c dominion.o rngs.o
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)

cardtest3: cardtest3.c dominion.o rngs.o
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)

cardtest4: cardtest4.c dominion.o rngs.o
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)

randomtestcard1: randomtestcard1.c dominion.o rngs.o
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o rngs.o
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)

randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
	gcc -o randomtestadventurer -g randomtestadventurer.c dominion.o rngs.o $(CFLAGS)

rununittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
	echo "unittest1.c:" > unittest1results.out
	./unittest1 &>> unittest1results.out
	gcov -abcfu dominion.c | sed -n '/updateCoins/{n;p; n;p; n;p; n;p}' >> unittest1results.out
	
rununittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
	echo "unittest2.c:" > unittest2results.out
	./unittest2 &>> unittest2results.out
	gcov -abcfu dominion.c | sed -n '/isGameOver/{n;p; n;p; n;p; n;p}' >> unittest2results.out
	
rununittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
	echo "unittest3.c:" > unittest3results.out
	./unittest3 &>> unittest3results.out
	gcov -abcfu dominion.c | sed -n '/shuffle/{n;p; n;p; n;p; n;p}' >> unittest3results.out
	
rununittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
	echo "unittest4.c:" > unittest4results.out
	./unittest4 &>> unittest4results.out
	gcov -abcfu dominion.c | sed -n '/discardCard/{n;p; n;p; n;p; n;p}' >> unittest4results.out

randomtestcard1.out: randomtestcard1.c dominion.o rngs.o
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)
	echo "randomtestcard1.c:" > randomtestcard1.out
	./randomtestcard1 10000 &>> randomtestcard1.out
	gcov -abcfu dominion.c | sed -n '/smithyEffect/{n;p; n;p; n;p; n;p}' >> randomtestcard1.out

randomtestcard2.out: randomtestcard2.c dominion.o rngs.o
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)
	echo "randomtestcard2.c:" > randomtestcard2.out
	./randomtestcard2 10000 &>> randomtestcard2.out
	gcov -abcfu dominion.c | sed -n '/villageEffect/{n;p; n;p; n;p; n;p}' >> randomtestcard2.out

randomtestadventurer.out: randomtestadventurer.c dominion.o rngs.o
	gcc -o randomtestadventurer -g randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
	echo "randomtestadventurer.c:" > randomtestadventurer.out
	./randomtestadventurer 10000 &>> randomtestadventurer.out
	gcov -abcfu dominion.c | sed -n '/adventurerEffect/{n;p; n;p; n;p; n;p}' >> randomtestadventurer.out

runcardtests: cardtest1 cardtest2 cardtest3 cardtest4
	echo "cardtest1.c:" > unittestresults.out
	./cardtest1 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/cardEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	echo "cardtest2.c:" >> unittestresults.out
	./cardtest2 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/cardEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	echo "cardtest3.c:" >> unittestresults.out
	./cardtest3 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/cardEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	echo "cardtest4.c:" >> unittestresults.out
	./cardtest4 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/cardEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out

unittestresults.out: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4
	# gcov piped to sed from Piazza @87. My thanks to cas donoghue. sed is great!
	echo -e "Result for running Dominion unit tests:\n" > unittestresults.out
	echo "unittest1.c:" >> unittestresults.out
	./unittest1 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/updateCoins/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	echo "unittest2.c:" >> unittestresults.out
	./unittest2 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/isGameOver/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	echo "unittest3.c:" >> unittestresults.out
	./unittest3 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/shuffle/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	echo "unittest4.c:" >> unittestresults.out
	./unittest4 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/discardCard/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	# It was unclear whether card tests were supposed to be included herein, 
	# so I erred on the side of caution. The coverage information isn't especially 
	# useful for card tests, but I included it anyway. 
	echo "cardtest1.c:" >> unittestresults.out
	./cardtest1 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/cardEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	echo "cardtest2.c:" >> unittestresults.out
	./cardtest2 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/cardEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	echo "cardtest3.c:" >> unittestresults.out
	./cardtest3 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/cardEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	echo "cardtest4.c:" >> unittestresults.out
	./cardtest4 &>> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/cardEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out

all: playdom player testDrawCard testBuyCard badTestDrawCard unittestresults.out

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe badTestDrawCard badTestDrawCard.exe unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 randomtestadventurer randomtestcard1 randomtestcard2
