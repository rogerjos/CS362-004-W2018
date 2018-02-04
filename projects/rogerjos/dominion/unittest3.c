/*******************************************************************************
 * Program: unittest1.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: Unit test for shuffle() function in dominion.c
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

// Set to 1 to display tests passed
#define VERBOSE 1

int main() {
	struct gameState state,	// Current game state 
					 control;	// For detecting unexpected state changes

	int i,	// A general counter/variable
		player,	// Current player
		opponent,
		fullDeckCount = 27,
		shortDeckCount = 10,
		emptyDeckCount = 0,
		errors = 0;	// Counter for test failures
		
	int fullDeck[27] = {curse, estate, duchy, province, copper, silver, gold, 
						adventurer, council_room, feast, gardens, mine, remodel,
						smithy, village, baron, great_hall, minion, steward, 
						tribute, ambassador, cutpurse, embargo, outpost, 
						salvager, sea_hag, treasure_map};

	int shortDeck[10] = {curse, estate, duchy, province, copper, silver, 
						 gold, adventurer, councin_room, feast};

	/* Test shuffle() with full decks */

	/* Clear junk data from gameState struct */
	memset(&state, 23, sizeof(struct gameState));

	/* Set up full decks for all players and save current state to control struct */
	for (player = 0; player < MAX_PLAYERS; player++) {
		memcpy(state.deck[player], fullDeck, sizeof(int) * fullDeckCount);
		state.deckCount[player] = fullDeckCount;
	}
	memcpy(&control, &state, sizeof(struct gameState));		
 
	/* Test shuffling full decks for all players */
	for (player = 0; player < MAX_PLAYERS; player++) {
		shuffle(player, &state);
		if (!memcmp(state.deck[player], control.deck[player], sizeof(int) * fullDeckSize)) {	// Err if player deck not shuffled
			printf("shuffle() test FAIL: no change to state of target %d card deck.\n", fullDeckSize);
			errors++;
		}
		else {
			if (VERBOSE) printf("shuffle() test PASS: changed state of target %d card deck.\n", fullDeckSize);
		}
		for (opponent = 0; opponent < MAX_PLAYERS; opponent++) {	// Check all opponents' decks
			if (opponent != player) {
				if (memcmp(state.deck[player], control.deck[player], sizeof(int) * fullDeckSize)) {	// Err if opponent deck shuffled
					printf("shuffle() test FAIL: changed state of opponent %d card deck.\n", fullDeckSize);
					errors++;
				}
				else {
					if (VERBOSE) printf("shuffle() test PASS: no change to state of opponent %d card deck.\n", fullDeckSize);
				}
			}
		}
		memcpy(control.deck[player], state.deck[player], sizeof(int) * fullDeckCount);	// Update control with shuffled deck
	}

	/* Test for changes to game state other than intended deck shuffling */
	if(memcmp(control, state, sizeof(struct gameState))) {
		printf("shuffle() test FAIL: change to non-deck game state when run with %d card decks.\n", fullDeckSize);
		errors++;
	}
	else {
		if (VERBOSE) printf("shuffle() test PASS: no change to non-deck game state when run with %d card decks.\n", fullDeckSize);
	}

	/* Test shuffle() with short decks */

	/* Clear junk data from gameState struct */
	memset(&state, 23, sizeof(struct gameState));

	/* Set up short decks for all players and save current state to control struct */
	for (player = 0; player < MAX_PLAYERS; player++) {
		memcpy(state.deck[player], shortDeck, sizeof(int) * shortDeckCount);
		state.deckCount[player] = shortDeckCount;
	}
	memcpy(&control, &state, sizeof(struct gameState));		
 
	/* Test shuffling short decks for all players */
	for (player = 0; player < MAX_PLAYERS; player++) {
		shuffle(player, &state);
		if (!memcmp(state.deck[player], control.deck[player], sizeof(int) * shortDeckSize)) {	// Err if player deck not shuffled
			printf("shuffle() test FAIL: no change to state of target %d card deck.\n", shortDeckSize);
			errors++;
		}
		else {
			if (VERBOSE) printf("shuffle() test PASS: changed state of target %d card deck.\n", shortDeckSize);
		}
		for (opponent = 0; opponent < MAX_PLAYERS; opponent++) {	// Check all opponents' decks
			if (opponent != player) {
				if (memcmp(state.deck[player], control.deck[player], sizeof(int) * shortDeckSize)) {	// Err if opponent deck shuffled
					printf("shuffle() test FAIL: changed state of opponent %d card deck.\n", shortDeckSize);
					errors++;
				}
				else {
					if (VERBOSE) printf("shuffle() test PASS: no change to state of opponent %d card deck.\n", shortDeckSize);
				}
			}
		}
		memcpy(control.deck[player], state.deck[player], sizeof(int) * shortDeckCount);	// Update control with shuffled deck
	}

	/* Test for changes to game state other than intended deck shuffling */
	if(memcmp(control, state, sizeof(struct gameState))) {
		printf("shuffle() test FAIL: change to non-deck game state when run with %d card decks.\n", shortDeckSize);
		errors++;
	}
	else {
		if (VERBOSE) printf("shuffle() test PASS: no change to non-deck game state when run with %d card decks.\n", shortDeckSize);
	}

	/* Test shuffle() with empty decks */

	/* Clear junk data from gameState struct */
	memset(&state, 23, sizeof(struct gameState));

	/* Set up empty decks for all players and save current state to control struct */
	for (player = 0; player < MAX_PLAYERS; player++) {
		state.deckCount[player] = emptyDeckCount;
	}
	memcpy(&control, &state, sizeof(struct gameState));		
 
	/* Test shuffling empty decks for all players */
	for (player = 0; player < MAX_PLAYERS; player++) {
		if (!shuffle(player, state)) {
			printf("shuffle() test FAIL: did not exit with error on empty deck.\n");
			errors++;
		}
		else {
			if (VERBOSE) printf("shuffle() test PASS: exited with error on empty deck.\n");
		}
	}

	/* Test for changes to game state other than intended deck shuffling */
	if(memcmp(control, state, sizeof(struct gameState))) {
		printf("shuffle() test FAIL: change to non-deck game state when run with %d card decks.\n", emptyDeckSize);
		errors++;
	}
	else {
		if (VERBOSE) printf("shuffle() test PASS: no change to non-deck game state when run with %d card decks.\n", emptyDeckSize);
	}

	/* Display results */
	printf("shuffle() test RESULTS: ");
	if (errors) {
		printf("%d TESTS FAILED.\n", errors);
	} 
	else {
		printf("ALL TESTS PASSED.\n");
	}

	return 0;
}
