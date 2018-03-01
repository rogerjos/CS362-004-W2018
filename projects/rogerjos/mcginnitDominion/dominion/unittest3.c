/*******************************************************************************
 * Program: unittest3.c
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

// Toggle verbose output 0 = off, else on
#define VERBOSE 0

int main() {
	struct gameState state,	// Current game state 
					 control;	// For detecting unexpected state changes

	int	player,	// Current player
		opponent,
		fullDeckCount = 27,
		shortDeckCount = 10,
		emptyDeckCount = 0,
		errors = 0,	// Counter for test failures
		final = 0;	// Counter for final failure count	
	
	int fullDeck[27] = {curse, estate, duchy, province, copper, silver, gold, 
						adventurer, council_room, feast, gardens, mine, remodel,
						smithy, village, baron, great_hall, minion, steward, 
						tribute, ambassador, cutpurse, embargo, outpost, 
						salvager, sea_hag, treasure_map};

	int shortDeck[10] = {curse, estate, duchy, province, copper, silver, 
						 gold, adventurer, council_room, feast};

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
		if (!memcmp(&state.deck[player], &control.deck[player], sizeof(int) * fullDeckCount)) {	// Err if player deck not shuffled
			if (VERBOSE) {
				printf("shuffle(): FAIL no change to state of target %d card deck.\n", fullDeckCount);
			}
			errors++;
		}
		else {
			if (VERBOSE) printf("shuffle(): PASS changed state of target %d card deck.\n", fullDeckCount);
		}
		for (opponent = 0; opponent < MAX_PLAYERS; opponent++) {	// Check all opponents' decks
			if (opponent != player) {
				if (memcmp(&state.deck[opponent], &control.deck[opponent], sizeof(int) * fullDeckCount)) {	// Err if opponent deck shuffled
					if (VERBOSE) {
						printf("shuffle(): FAIL changed state of opponent %d card deck.\n", fullDeckCount);
					}
					errors++;
				}
				else {
					if (VERBOSE) printf("shuffle(): PASS no change to state of opponent %d card deck.\n", fullDeckCount);
				}
			}
		}
		memcpy(control.deck[player], state.deck[player], sizeof(int) * fullDeckCount);	// Update control with shuffled deck
	}

	/* Test for changes to game state other than intended deck shuffling */
	if(memcmp(&control, &state, sizeof(struct gameState))) {
		if (VERBOSE) {
			printf("shuffle(): FAIL change to non-deck game state when run with %d card decks.\n", fullDeckCount);
		}
		errors++;
	}
	else {
		if (VERBOSE) printf("shuffle(): PASS no change to non-deck game state when run with %d card decks.\n", fullDeckCount);
	}

	/* Display results large decks*/
	printf("shuffle(): ");
	if (errors) {
		printf("FAIL when shuffling %d card decks.\n", fullDeckCount);
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when shuffling %d card decks.\n", fullDeckCount);
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
		if (!memcmp(&state.deck[player], &control.deck[player], sizeof(int) * shortDeckCount)) {	// Err if player deck not shuffled
			if (VERBOSE) {
				printf("shuffle(): FAIL no change to state of target %d card deck.\n", shortDeckCount);
			}
			errors++;
		}
		else {
			if (VERBOSE) printf("shuffle(): PASS changed state of target %d card deck.\n", shortDeckCount);
		}
		for (opponent = 0; opponent < MAX_PLAYERS; opponent++) {	// Check all opponents' decks
			if (opponent != player) {
				if (memcmp(&state.deck[opponent], &control.deck[opponent], sizeof(int) * shortDeckCount)) {	// Err if opponent deck shuffled
					if (VERBOSE) {
						printf("shuffle(): FAIL changed state of opponent %d card deck.\n", shortDeckCount);
					}
					errors++;
				}
				else {
					if (VERBOSE) printf("shuffle(): PASS no change to state of opponent %d card deck.\n", shortDeckCount);
				}
			}
		}
		memcpy(control.deck[player], state.deck[player], sizeof(int) * shortDeckCount);	// Update control with shuffled deck
	}

	/* Test for changes to game state other than intended deck shuffling */
	if(memcmp(&control, &state, sizeof(struct gameState))) {
		if (VERBOSE) {
			printf("shuffle(): FAIL change to non-deck game state when run with %d card decks.\n", shortDeckCount);
		}
		errors++;
	}
	else {
		if (VERBOSE) printf("shuffle(): PASS no change to non-deck game state when run with %d card decks.\n", shortDeckCount);
	}

	/* Display results short decks*/
	printf("shuffle(): ");
	if (errors) {
		printf("FAIL when shuffling %d card decks.\n", shortDeckCount);
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when shuffling %d card decks.\n", shortDeckCount);
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
		if (!shuffle(player, &state)) {
			if (VERBOSE) {
				printf("shuffle(): FAIL did not exit with error on empty deck.\n");
			}
			errors++;
		}
		else {
			if (VERBOSE) printf("shuffle(): PASS exited with error on empty deck.\n");
		}
	}

	/* Test for changes to game state other than intended deck shuffling */
	if(memcmp(&control, &state, sizeof(struct gameState))) {
		if (VERBOSE) {
			printf("shuffle(): FAIL change to non-deck game state when run with %d card decks.\n", emptyDeckCount);
		}
		errors++;
	}
	else {
		if (VERBOSE) printf("shuffle(): PASS no change to non-deck game state when run with %d card decks.\n", emptyDeckCount);
	}

	/* Display results shuffling empty decks */
	printf("shuffle(): ");
	if (errors) {
		printf("FAIL when shuffling %d card decks.\n", emptyDeckCount);
		final += errors;
		errors = 0;
	} 
	else {
		printf("PASS when shuffling %d card decks.\n", emptyDeckCount);
	}

	/* Display final results */
	if (!final) {
		printf("shuffle(): ALL TESTS PASSED.\n");
	}

	return 0;
}
