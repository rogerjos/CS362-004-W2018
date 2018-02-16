/*******************************************************************************
 * Program: randomcardtest1.c
 * Author: Joshua L. Rogers
 * Date: 18 Feb 2018
 * Course: CS362
 * Description: random test generator for smithyEffect() in dominion.c
 * Card text: "+3 Cards"
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

/* 	randomValidCard() 
	Returns random valid card (e.g., supply != -1) 
	from the set of cards in a gameState struct */
int randomValidCard(struct gameState state);

/* 	randomizeGameState()
	Clears all data from a gameState struct
	and replaces it with relatively sane data */
int randomizeGameState(struct gameState *state);

/*	playSmithy()
	Puts a Smithy in player hand, 
	saves game state,
	then calls smithyEffect() */
int playSmithy(struct gameState *pre, struct gameState *post);

/*	testSmithy()
	Tests smithyEffect() by comparing game states */
int testSmithy(struct gameState const *pre, struct gameState const *post, int *err); 

/*	printResults()
	Displays results in aggregate across all tests.
	fut: name of function under test
	caseCount: number of test cases
	testCount: number of tests evaulated per case
	err: failure count array
	testMsg: array of test explanation messages
*/
int printResults(char *fut, unsigned long int caseCount, int testCount, int *err, char **testMsg);


int main(int argc, char *argv[]) {

	time_t	stop_time,
			start_time = time(NULL);

	unsigned long int iterations;

	int	i = 0, 
		err[6] = {0};

	char *testMsg[6];

	testMsg[0] = "Played card count incremented by one";
	testMsg[1] = "Smithy top card of played pile";	
	testMsg[2] = "Cards in hand incremented by two";
	testMsg[3] = "Cards in deck decremented by three";
	testMsg[4] = "Cards in deck + discard decremented by three";
	testMsg[5] = "No change to out of scope game elements";

	struct gameState pre, post;

	// If incorrect argument count print usage and exit with error
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <test_iteration_count>\n", argv[0]);
		exit(1);
	}

	// Correct argument count. Step through argument and check for integers
	else {
		while (argv[1][i] != '\0') {

			// On non-digit print usage and exit with error
			if (!(argv[1][i] >= '0' && argv[1][i] <= '9')) {
				fprintf(stderr, "Usage: %s <test_iteration_count>\n", argv[0]);				
				exit(1);
			}

			// Increment i
			i++;
		}

		iterations = (unsigned long int)atoi(argv[1]);
	}

	printf("Smithy: Evaluating %lu random test cases.\n", iterations);
	fflush(stdout);

	for (i = 1; i <= iterations; i++) {
		// Set up RNG
		SelectStream(1);
		PutSeed((long)i);

		randomizeGameState(&pre); // Generate random gamestate

		playSmithy(&pre, &post); // call Smithy code

		testSmithy(&pre, &post, err); // evaluate test case
	}

	// Print aggregate results
	printResults("Smithy", iterations, 6, err, testMsg);

	// Append runtime.
	stop_time = time(NULL);
	printf("%s runtime: %fs\n", argv[0], difftime(stop_time, start_time));

	return 0;
}

/* 	randomValidCard() 
	Returns random valid card (e.g., supply != -1) 
	from the set of cards in a gameState struct */
int randomValidCard(struct gameState state) {
	int card;

	do {
		card = ((int)(Random() * 1000) % 27);
	} while (state.supplyCount[card] < 0); // If not in game, supply is -1: repeat!

	return card;
}

/* 	randomizeGameState()
	Clears all data from a gameState struct
	and replaces it with relatively sane data */
int randomizeGameState(struct gameState *state)
{
	int i,
		j,
		tokens,
		card;

	int k[10]; // Array of kingdom cards used in game

	memset(state, -1, sizeof(struct gameState)); // Wipe all gameState data

	// Set player count [0, MAX_PLAYERS]
	state->numPlayers = (((int)(Random() * 1000) % MAX_PLAYERS) + 1);

	// Select random set of unique kingdom cards
	for (i = 0; i < 10; i++) { // Step through all supply piles
		do {
			k[i] = 0;
			card = (((int)(Random() * 1000) % 20) + adventurer); // Choose a random supply card
			for (j = 0; j < i; j++) { // Step through all previously-selected kingdom cards
				if (k[j] == card) { // If random card is found
					k[i] = -1;	// Set error
					break; // Stop evaluating previous kingdom cards
				}
			}
			if (k[i] != -1) { // Check for duplicate cards
				k[i] = card; // No duplicate found, so assign card
			}
		} while (k[i] != card); // loop until non-duplicate found
	}


	// Set sane supply counts

	// curse: [0,30]
	state->supplyCount[curse] = ((int)(Random() * 1000) % 31);

	// victory: [0,12]
	for (i = estate; i <= province; i++) {
		state->supplyCount[i] = ((int)(Random() * 1000) % 13);	
	}
	
	// treasure: c[0,50] s[0,40], g[0,30]
	for (i = copper; i <= gold; i++) {
		state->supplyCount[i] = ((int)(Random() * 1000) % (51 - (10*(i-copper))));
	}

	// kingdom cards: [0,10]
	for (i = adventurer; i <= treasure_map; i++) {
		for (j = 0; j < 10; j++) {
			if (k[j] == i) {
				state->supplyCount[i] = ((int)(Random() * 1000) % 11);
				break;	
			}
		}
	}

	if (state->supplyCount[embargo] > -1) { // If embargo is in game
		tokens = 10 - state->supplyCount[embargo]; // Assume they've all been played
		for (i = curse; i <= treasure_map; i++) { // distribute tokens
			if (state->supplyCount[i] > -1) { // if card is in game
				state->embargoTokens[i] = ((int)(Random() * 1000) % (tokens + 1)); // Give it [0, tokens remaining]
				tokens -= state->embargoTokens[i]; // Track tokens distributed
			}	
		}
	} // Note: This doesn't account for embargos in hand, but the important thing is that the number is sane and random.

	state->outpostPlayed = 0;
	state->outpostTurn = 0;
	if (state->supplyCount[outpost] > -1) { // If outpost in game
		state->outpostPlayed = ((int)(Random() * 1000) % 3); // Set outpostPlayed in [0, 2]
		state->outpostTurn = ((int)(Random() * 1000) % 3); // Set outpostTurn in [0, 2]
	}

	// Set current player
	state->whoseTurn = ((int)(Random() * 1000) % state->numPlayers);

	// set phase - not used. :-/
	state->phase = 0;	

	// Set actions: [1, 10]
	state->numActions = (((int)(Random() * 1000) % 10) + 1);

	// Set buys: [1, 10]
	state->numBuys = (((int)(Random() * 1000) % 10) + 1);

	// Set up player cards	
	for (i = 0; i < state->numPlayers; i++) {

		// Set deckCount in [0, MAX_DECK]
		state->deckCount[i] = ((int)(Random() * 1000) % (MAX_DECK + 1));

		// Set discardCount in [0, MAX_DECK - deckCount]
		state->discardCount[i] = ((int)(Random() * 1000) % ((MAX_DECK - state->deckCount[i]) + 1));

		// Set handCount in [0, 20]
		state->handCount[i] = ((int)(Random() * 1000) % 21);

		// If hand bigger than deck, set hand in [0, deckCount]
		if (state->handCount[i] > state->deckCount[i]) {
			state->handCount[i] = ((int)(Random() * 1000) % (state->deckCount[i] + 1));
		}

		// Subtract hand from deck (i.e., "draw")
		state->deckCount[i] -= state->handCount[i];

		// Fill deck with cards
		for (j = 0; j < state->deckCount[i]; j++) {
			state->deck[i][j] = randomValidCard(*state);
		}

		// Fill discard with cards
		for (j = 0; j < state->discardCount[i]; j++) {
			state->discard[i][j] = randomValidCard(*state);
		}

		// Fill hand with cards
		for (j = 0; j < state->handCount[i]; j++) {
			state->hand[i][j] = randomValidCard(*state);
		}		
	}

	// Set played cards in [0, 10]
	state->playedCardCount = ((int)(Random() * 1000) % 11);

	// If more played cards than are in deck, set played cards in [0, deckCount]
	if (state->playedCardCount > state->deckCount[state->whoseTurn]) {
		state->playedCardCount = ((int)(Random() * 1000) % (state->deckCount[state->whoseTurn] + 1));
	}

	// Update played card array from deck array
	for (i = 0; i < state->playedCardCount; i++) {

		// Copy the top card of current player's deck to the played cards stack
		state->playedCards[i] = state->deck[state->whoseTurn][state->deckCount[state->whoseTurn] - 1];

		// Erase the top card of current player's deck
		state->deck[state->whoseTurn][state->deckCount[state->whoseTurn] - 1] = -1;

		 // Decrement size of current player's deck
		state->deckCount[state->whoseTurn]--;
	}


	// Update coins
	state->coins = 0;
	for (i = 0; i < state->handCount[state->whoseTurn]; i++) {
		if (state->hand[state->whoseTurn][i] == copper) {
			state->coins += 1;
		}
		else if (state->hand[state->whoseTurn][i] == silver) {
			state->coins += 2;
		}
		else if (state->hand[state->whoseTurn][i] == gold) {
			state->coins += 3;
		}
	}

	return 0;
}

/*	playSmithy()
	Puts a Smithy in player hand, 
	saves game state,
	then calls smithyEffect() */
int playSmithy(struct gameState *pre, struct gameState *post) {

	pre->hand[pre->whoseTurn][pre->handCount[pre->whoseTurn]] = smithy; // Put an adventurer into current player's hand
	pre->handCount[pre->whoseTurn]++; // Increment current player handcount

	memcpy(post, pre, sizeof(struct gameState)); // Copy pre to post

	smithyEffect(post, post->whoseTurn, post->handCount[post->whoseTurn] - 1); // Play Smithy

	return 0;
}


/*	testSmithy()
	Tests smithyEffect() by comparing game states */
int testSmithy(struct gameState const *pre, struct gameState const *post, int *err) {

	struct gameState test;

	int player = pre->whoseTurn;

	// post playedCardCount should be > 0 and one greater than pre pCC.
	if (!( (post->playedCardCount > 0) && (post->playedCardCount == pre->playedCardCount + 1) )) {
		err[0]++;
	}

	// If no played cards then error
	if (!(post->playedCardCount > 0)) {
		err[1]++;
	}
	// If top of pile is not Smithy then error		
	else if (!(post->playedCards[post->playedCardCount - 1] == smithy)) {
			err[1]++;
	}

	// Smithy should net +2 cards to hand since Smithy itself is removed from hand 
	if (post->handCount[player] != pre->handCount[player] + 2) {
		err[2]++;
	}

	// If deck has at least 3 cards before Smithy then no shuffle: deck size decrements 3
	if (pre->deckCount[player] >= 3) { 
		if (post->deckCount[player] != pre->deckCount[player] - 3) {
			err[3]++;
		}
	}
	// If deck has fewer than 3 cards before Smithy then shuffle required: deck size + discard size decrements 3
	else if ((post->deckCount[player] + post->discardCount[player]) != (pre->deckCount[player] + pre->discardCount[player] - 3)) { 
		err[4]++;
	}

	// Make a copy of pre in test
	memcpy(&test, pre, sizeof(struct gameState));

	// Wipe evaluated changes from test struct
	memcpy(&(test.handCount[player]), &(post->handCount[player]), sizeof(int));	// handCount
	memcpy(&(test.hand[player]), &(post->hand[player]), sizeof(int) * MAX_HAND);	// hand
	memcpy(&(test.deckCount[player]), &(post->deckCount[player]), sizeof(int));	// deckCount
	memcpy(&(test.deck[player]), &(post->deck[player]), sizeof(int) * MAX_DECK);	// deck
	memcpy(&(test.discardCount[player]), &(post->discardCount[player]), sizeof(int));	// discardCount
	memcpy(&(test.discard[player]), &(post->discard[player]), sizeof(int) * MAX_DECK);		// discard
	memcpy(&(test.playedCardCount), &(post->playedCardCount), sizeof(int));	// playedCardCount
	memcpy(&(test.playedCards), &(post->playedCards), sizeof(int) * MAX_DECK);	// playedCards

	// compare post to test by byte. Should be equal
	if (memcmp(&test, post, sizeof(struct gameState))) {
		err[5]++;
	}

	return 0;
}

int printResults(char *fut, unsigned long int caseCount, int testCount, int *err, char **testMsg) {

	int i,
		failures = 0;

	for (i = 0; i < testCount; i++) {
		printf("%s: ", fut);
		if (!err[i]) {
			printf("PASS ");
		}
		else {
			printf("FAIL (x%d) ", err[i]);
			failures += err[i];	
		}
		printf("%s\n", testMsg[i]);
	}

	if (!failures) {
		printf("%s: ALL TESTS PASSED\n", fut);
	}
	else {
		printf("%s: %d FAILURES ACROSS %lu TEST CASES\n", fut, failures, caseCount);
	}

	return 0;
}



