/*******************************************************************************
 * Program: randomcardtest1.c
 * Author: Joshua L. Rogers
 * Date: 18 Feb 2018
 * Course: CS362
 * Description: random test generator for adventurerEffect() in dominion.c
 * Card text:  "Reveal cards from your deck
 *				until you reveal 2 Treasure cards.
 *				Put those Treasure cards into your hand
 *				and discard the other revealed cards."
 *
 * Card rulings: 1) If you have to shuffle in the middle, shuffle. 
 					Don't shuffle in the revealed cards as these 
 					cards do not go to the Discard pile until you 
 					have finished revealing cards.

				 2) If you run out of cards after shuffling and still
				 	only have one Treasure, you get just that one Treasure.
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

/* 	randomValidNonTreasureCard() 
	Returns random valid non-treasure card (e.g., supply != -1) 
	from the set of cards in a gameState struct */
int randomValidCard(struct gameState state);

/* 	randomizeGameState()
	Clears all data from a gameState struct
	and replaces it with relatively sane data */
int randomizeGameState(struct gameState *state);

/*	playAdventurer()
	Puts a Adventurer in player hand, 
	saves game state,
	then calls adventurerEffect() */
int playAdventurer(struct gameState *pre, struct gameState *post);

/*	testAdventurer()
	Tests adventurerEffect() by comparing game states */
int testAdventurer(struct gameState const *pre, struct gameState const *post, int *err); 

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
		err[7] = {0};

	char *testMsg[7];

	testMsg[0] = "Played card count incremented by one";
	testMsg[1] = "Adventurer top card of played pile";
	testMsg[2] = "Cards in hand correct net change";
	testMsg[3] = "Cards drawn are all Treasures";
	testMsg[4] = "Correct number of cards left in deck";
	testMsg[5] = "Correct number of cards discarded";
	testMsg[6] = "No change to out of scope game elements";

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

	printf("Adventurer: Evaluating %lu random test cases.\n", iterations);
	fflush(stdout);

	for (i = 1; i <= iterations; i++) {
		// Set up RNG
		SelectStream(1);
		PutSeed((long)i);

		randomizeGameState(&pre); // Generate random gamestate

		playAdventurer(&pre, &post); // call Adventurer code

		testAdventurer(&pre, &post, err); // evaluate test case
		}

	// Print aggregate results
	printResults("Adventurer", iterations, 7, err, testMsg);

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


int randomValidNonTreasureCard(struct gameState state) {
	int card;

	do {
		card = ((int)(Random() * 1000) % 27);
	} while (state.supplyCount[card] < 0 && card != copper && card != silver && card != gold); // No treasures, no unchosen cards

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

		// Set deckCount in [0, MAX_DECK - 1]
		state->deckCount[i] = ((int)(Random() * 1000) % (MAX_DECK));

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

		// Fill deck with cards -- no treasures!
		for (j = 0; j < state->deckCount[i]; j++) {
			state->deck[i][j] = randomValidNonTreasureCard(*state);
		}

		// Fill discard with cards
		for (j = 0; j < state->discardCount[i]; j++) {
			state->discard[i][j] = randomValidNonTreasureCard(*state);
		}

		// Fill hand with cards
		for (j = 0; j < state->handCount[i]; j++) {
			state->hand[i][j] = randomValidNonTreasureCard(*state);
		}		

		// Add random treasures
		switch ((int)(Random() * 1000) % 7) { // On 6 deck/discard/hand have zero Treasures

			case 0: // One Treasure in deck
				if (state->deckCount[i] > 0) {
					state->deck[i][(int)(Random() * 1000) % state->deckCount[i]] = ((int)(Random() * 1000) % 3) + 3; // Add a Treasure 4||5||6
				}
				break;

			case 1: // One Treasure in discard
				if (state->discardCount[i] > 0) {
					state->discard[i][(int)(Random() * 1000) % state->discardCount[i]] = ((int)(Random() * 1000) % 3) + 3; // Add a Treasure 4||5||6
				}
				break;

			case 2: // ~Two Treasures in deck
				if (state->deckCount[i] > 0) {
					state->deck[i][(int)(Random() * 1000) % state->deckCount[i]] = ((int)(Random() * 1000) % 3) + 3; // Add a Treasure 4||5||6
				}
				if (state->deckCount[i] > 0) {
					state->deck[i][(int)(Random() * 1000) % state->deckCount[i]] = ((int)(Random() * 1000) % 3) + 3; // Add a Treasure 4||5||6
				}
				break;

			case 3: // ~Two Treasures in descard
				if (state->discardCount[i] > 0) {
					state->discard[i][(int)(Random() * 1000) % state->discardCount[i]] = ((int)(Random() * 1000) % 3) + 3; // Add a Treasure 4||5||6
				}
				if (state->discardCount[i] > 0) {
					state->discard[i][(int)(Random() * 1000) % state->discardCount[i]] = ((int)(Random() * 1000) % 3) + 3; // Add a Treasure 4||5||6
				}
				break;

			case 4: // One Treasure in deck + one Treasure in discard
				if (state->deckCount[i] > 0) {
					state->deck[i][(int)(Random() * 1000) % state->deckCount[i]] = ((int)(Random() * 1000) % 3) + 3; // Add a Treasure 4||5||6
				}
				if (state->discardCount[i] > 0) {
					state->discard[i][(int)(Random() * 1000) % state->discardCount[i]] = ((int)(Random() * 1000) % 3) + 3; // Add a Treasure 4||5||6
				}
				break;

			case 5: // Random Treasures
				for (j = 0; j < state->deckCount[i]; j++) {
					state->deck[i][j] = randomValidCard(*state);
				}
				for (j = 0; j < state->discardCount[i]; j++) {
					state->discard[i][j] = randomValidCard(*state);
				}
				break;
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


/*	playAdventurer()
	Puts a Adventurer in player hand, 
	saves game state,
	then calls adventurerEffect() */
int playAdventurer(struct gameState *pre, struct gameState *post) {

	int temphand[MAX_HAND] = {-1};
	int	z = 0;

	pre->hand[pre->whoseTurn][pre->handCount[pre->whoseTurn]] = adventurer; // Put an adventurer into current player's hand
	pre->handCount[pre->whoseTurn]++; // Increment current player handcount

	memcpy(post, pre, sizeof(struct gameState)); // Copy pre to post

	adventurerEffect(post, pre->whoseTurn, temphand, &z); // Play Adventurer

	return 0;
}


/*	testAdventurer()
	Tests adventurerEffect() by comparing game states */
int testAdventurer(struct gameState const *pre, struct gameState const *post, int *err) {

	int i,
		player = pre->whoseTurn,
		cTreasure = 0, // treasure counter
		indexTreasure2 = -1;

	struct gameState test;

	// Get pre state info to identify evaluation cases:

	// Look for treasures in deck, search from top!
	for (i = pre->deckCount[player]-1; i >= 0;  i--) {
	
		// Is card treasure?
		if ((pre->deck[player][i] >= copper && pre->deck[player][i] <= gold)) {
			cTreasure++; // Found a treasure!
		}

		// Found two treasures in deck?
		if (cTreasure == 2) {
			indexTreasure2 = pre->deckCount[player]-i-1; // Record location of second treasure in deck
			break; // Adventurer only digs to the second treasure, so stop
		}
	}

	// If fewer than two treasures in deck, expand search to discard pile.
	if (cTreasure < 2) {

		// Look for treasures in discard
		for (i = pre->discardCount[player]-1; i >= 0;  i--) {
			
			// Is card treasure?
			if ((pre->discard[player][i] >= copper && pre->discard[player][i] <= gold)) {
				cTreasure++; // Found a treasure!
			}

			// Found two treasures between deck + discard?
			if (cTreasure == 2) {
				break; // Adventurer only needs two treasures, so stop
			}
		}
	}


	// Tests: 

	// post playedCardCount should be > 0 and one greater than pre pCC.
	if (!( (post->playedCardCount > 0) && (post->playedCardCount == pre->playedCardCount + 1) )) {
		err[0]++;
	}

	// If no played cards then error
	if (!(post->playedCardCount > 0)) {
		err[1]++;
	}

	// If top of pile is not Adventurer then error		
	else if (!(post->playedCards[post->playedCardCount - 1] == adventurer)) {
		err[1]++;
	}

	/* 	Evaluate case such that at least two treasures
		are in deck before Adventurer is played */
	if (indexTreasure2 >= 0) {

		// Adventurer nets +1 cards when two treasures drawn
		if (post->handCount[player] != (pre->handCount[player] + 1)) {
			err[2]++;
		}

		// Adventurer adds two treasures to hand if possible, and it's possible!
		if (!(post->hand[player][post->handCount[player] - 1] >= copper && post->hand[player][post->handCount[player] - 1] <= gold) ||
			!(post->hand[player][post->handCount[player] - 2] >= copper && post->hand[player][post->handCount[player] - 2] <= gold)) {
			err[3]++;
		}
				
		// Two treasures in deck means no shuffling required. Ensure correct number of cards left in deck
		if (post->deckCount[player] != pre->deckCount[player] - (indexTreasure2 + 1)) {
			err[4]++;
		}

		// Deck predictable, so discard predictable. Should contain all cards removed from deck less two Treasures
		if (post->discardCount[player] != pre->discardCount[player] + indexTreasure2 -1) {
			err[5]++;
		}

	}

	/* 	Evaluate case such that fewer than two treasures are in deck but
		at least two are in (deck + discard) before Adventurer is played */
	else if (cTreasure == 2) {

		// Adventurer nets +1 cards when two treasures drawn
		if (post->handCount[player] != (pre->handCount[player] + 1)) {
			err[2]++;
		}

		// Adventurer adds two treasures to hand if possible, and it's possible!
		if (!(post->hand[player][post->handCount[player] - 1] >= copper && post->hand[player][post->handCount[player] - 1] <= gold) ||
			!(post->hand[player][post->handCount[player] - 2] >= copper && post->hand[player][post->handCount[player] - 2] <= gold)) {
			err[3]++;
		}

	}

	/*	Evaluate case such that fewer than two treasures
		are in deck + discard before Adventurer is played*/
	else {

		// Adventurer nets count(Treasures)-1 cards when two treasures are not available to be drawn
		if (post->handCount[player] != (pre->handCount[player] + cTreasure - 1)) {
			err[2]++;
		}

		// < 2 Treasures to be drawn, so ensure that if a Treasure was in deck + discard that it was drawn.
		if (cTreasure > 0 && 
			!(post->hand[player][post->handCount[player] - 1] >= copper && 
				post->hand[player][post->handCount[player] - 1] <= gold)) {
			err[3]++;
		}

		// Fewer than two treasures in deck after a shuffle entire discard pile will be drawn and discarded
		if (post->deckCount[player]) {
			err[4]++;
		}

		// Deck predictable, so discard predictable. Should contain all cards removed from deck less any Treasures
		if (post->discardCount[player] != pre->discardCount[player] + pre->deckCount[player] - cTreasure) {
			err[5]++;
		}
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
		err[6]++;
	}

	return 0;
}


/*	printResults()
	Displays results in aggregate across all tests.
	fut: name of function under test
	caseCount: number of test cases
	testCount: number of tests evaulated per case
	err: failure count array
	testMsg: array of test explanation messages */
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



