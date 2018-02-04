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
		crashErrors = 0,
		handCountErrors = 0,	
		deckCountErrors = 0,
		playedCountErrors = 0,
		playedErrors = 0,	
		boundErrors = 0,	
		totalErrors = 0;

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
		drawCard(player, &state);	// Draw a card
		state.hand[player][state.handCount[player] - 1] = smithy; // Set most recently drawn card to smithy
		crashErrors += playCard(state.handCount[player] - 1, 0, 0, 0, &state);	// Play smithy and check for error

		/* Test outcome */
		if (state.handCount[player] != control.handCount[player] + 2) { // Smithy draws 3 when played, so +2 net to hand
			handCountErrors++;
		}

		if ((state.deckCount[player] + state.discardCount[player]) != (control.deckCount[player] + control.discardCount[player] - 3)) {	// Smithy draws 3, so three cards should be missing between deck and discard (discard shuffled if < 3 in deck)
			deckCountErrors++;
		}

		if (state.playedCardCount != control.playedCardCount + 1) {	// Smithy was played, so playedCardCount should increment
			playedCountErrors++;
		}

		if (!((state.playedCardCount > 0) && (state.playedCards[state.playedCardCount - 1] == smithy))) {	// playedCardCount incremented and top card of playedCards should be smithy
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


		if (memcmp(&control, &state, sizeof(struct gameState))) {	// Check for out of bound changes to game state 	
			boundErrors++;
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

	/* Display results for boundary errors */
	printf("smithy: ");
	if (boundErrors) {
		printf("FAIL no unexpected modifications to game state\n");
	}
	else {
		printf("PASS no unexpected modifications to game state\n");
	}

	totalErrors = crashErrors + handCountErrors + deckCountErrors + playedCountErrors + playedErrors + boundErrors;

	/* Display final results */
	if (!totalErrors){
		printf("smithy: ALL TESTS PASSED");
	}

	return 0;
}
