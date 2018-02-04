/*******************************************************************************
 * Program: cardtest1.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: test for Smithy card in dominion.c
 * Note: Card text: "+3 cards"
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

#define SEED 23



int main() {
	struct gameState state,	// Current game state 
					 control;	// For detecting and reverting state changes

	int	player,
		crashErrors = 0,	// Counter for smithy returning errors
		handCountErrors = 0,	// Counter for test failures
		deckCountErrors = 0,	// Counter for test failures
		playedCountErrors = 0,	// Counter for test failures
		playedErrors = 0,	// Counter for test failures
		totalErrors = 0;	// Counter for cumulative errors

	int k[10] = {adventurer, council_room, feast, gardens, mine,	// Arbitrary cards
					remodel, smithy, village, baron, great_hall};

	/* Set up game */
	memset(&state, 23, sizeof(struct gameState)); // Clear data
	if (initializeGame(MAX_PLAYERS, k, SEED, &state)) {	// Print error and exit if new game fails
			printf("smithy ABORT: initializeGame() exited with error.\n");
			return 1;
	}
	memcpy(&control, &state, sizeof(struct gameState));	// Save beginning game state	
	
	for (player = 0; player < MAX_PLAYERS; player++) {	// Run test for each player

		/* Get and play smithy */
		drawCard(player, state);	// Draw a card
		state.hand[player][state.handCount[player] - 1] = smithy // Set most recently drawn card to smithy
		crashErrors += playCard(state.handCount[player] - 1, 0, 0, 0, &state);	// Play smithy and check for error

		/* Test outcome */
		if (state.handCount[player] != control.handCount[player] + 2) { // Smithy draws 3 when played, so +2 net to hand
				printf("smithy FAIL: player %d handsize increased by 2.\n", player);
			}
			handCountErrors++;
		}
		else {
			if (VERBOSE) {
				printf("smithy PASS: player %d handsize increased by 2.\n", player);
			}
		}

		if ((state.deckCount[player] + state.discardCount[player]) != (control.deckCount[player] + control.discardCount[player] - 3)) {	// Smithy draws 3, so three cards should be missing between deck and discard (discard shuffled if < 3 in deck)
			if (VERBOSE) {
				printf("smithy FAIL: player %d deck + discard count decreased by 3.\n", player);
			}
			deckCountErrors++;
		}
		else {
			if (VERBOSE) {
				printf("smithy PASS: player %d deck + discard count decreased by 3.\n", player);
			}
		}

		if (state.playedCardCount != control.playedCardCount + 1) {	// Smithy was played, so playedCardCount should increment
			if (VERBOSE) {
				printf("smithy FAIL: playedCardCount incremented.\n");
			}
			playedCountErrors++;
		}
		else {
			if (VERBOSE) {
				printf("smithy PASS: playedCardCount incremented.\n");
			}
		}

		if ((state.playedCardCount > 0) && (state.playedCards[state.playedCardCount - 1] == smithy)) {	// playedCardCount incremented and top card of playedCards should be smithy
			if (VERBOSE) {
				printf("smithy PASS: smithy added to top of playedCards.\n");
			}
		}
		else {
			if (VERBOSE) {
				printf("smithy FAIL: smithy added to top of playedCards.\n");
			}
			playedErrors++;
		} 

		/* Revert expected affected members */
		memcpy(&state.handCount[player], &control.handCount[player], sizeof(int));	// handCount
		memcpy(state.hand[player], control.hand[player], sizeof(int) * MAX_HAND);	// hand
		memcpy(&state.deckCount[player], &control.deckCount[player], sizeof(int));	// deckCount
		memcpy(state.deck[player], control.deck[player], sizeof(int) * MAX_DECK);	// deck
		memcpy(&state.discardCount[player], &control.discardCount[player], sizeof(int));	// discardCount
		memcpy(state.discard[player], control.discard[player], sizeof(int) * MAX_DECK);		// discard
		memcpy(&state.playedCardCount, &control.playedCardCount, sizeof(int));	// playedCardCount
		memcpy(state.playedCards, control.playedCards, sizeof(int) * MAX_DECK);	// playedCards


		if (memcmp(control, state, sizeof(struct gameState))) {	// Check for out of bound changes to game state 	
			if (VERBOSE) {
				printf("smithy FAIL: Out of bound changes made to game state struct.\n");
			}
			errors++;
		}
		else if (VERBOSE) {
			printf("smithy PASS: Out of bound changes made to game state struct.\n");
		}
	
		memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	
	}
		
	/* Display results crash */
	printf("smithy: ");
	if (crashErrors) {
		printf("FAIL does not return error\n");
	} 
	else {
		printf("PASS does not return error\n");
	}

	/* Display results draw 3 cards */
	printf("smithy: ");
	if (handCountErrors) {
		printf("FAIL draws three cards into correct hand\n");
	} 
	else {
		printf("PASS draws three cards into correct hand\n");
	}

	/* Display results deck+discard size reduced by three */
	printf("smithy: ");
	if (deckCountErrors) {
		printf("FAIL draws three cards from correct deck\n");
	} 
	else {
		printf("PASS draws three cards from correct deck\n");
	}
	
	/* Display results played count incremented */
	printf("smithy: ");
	if (playedCountErrors) {
		printf("FAIL played card count incremented\n");
	} 
	else {
		printf("PASS played card cound incremented\n");
	}
	
	/* Display results smithy placed on top of played card pile */
	printf("smithy: ");
	if (playedErrors) {
		printf("FAIL placed on top of played card pile\n");
	} 
	else {
		printf("PASS placed on top of played card pile\n");
	}

	/* Display final results */
	if (!(crashErrors || handCountErrors || deckCountErrors || playedCountErrors || playedErrors)){
		printf("smithy: iALL TESTS PASSED");
	}

	return 0;
}

