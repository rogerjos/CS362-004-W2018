/*******************************************************************************
 * Program: cardtest2.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: test for village card in dominion.c
 * Card text: "+1 Card, +2 Actions"
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
		crashErrors = 0,	// Counter for village returning errors
		handCountErrors = 0,	// Counter for test failures
		drawnCardErrors = 0;
		deckCountErrors = 0,	// Counter for test failures
		playedCountErrors = 0,	// Counter for test failures
		playedErrors = 0,	// Counter for test failures
		numActionsErrors = 0,

	int k[10] = {adventurer, council_room, feast, gardens, mine,	// Arbitrary cards
					remodel, cutpurse, minion, baron, great_hall};	// No village to ensure village not drawn

	/* Set up game */
	memset(&state, 23, sizeof(struct gameState)); // Clear data
	if (initializeGame(MAX_PLAYERS, k, SEED, &state)) {	// Print error and exit if new game fails
			printf("village ABORT: initializeGame() exited with error.\n");
			return 1;
	}
	memcpy(&control, &state, sizeof(struct gameState));	// Save beginning game state	
	
	for (player = 0; player < MAX_PLAYERS; player++) {	// Run test for each player

		/* Get and play village */
		drawCard(player, state);	// Draw a card
		state.hand[player][state.handCount[player] - 1] = village // Set most recently drawn card to village
		crashErrors += playCard(state.handCount[player] - 1, 0, 0, 0, &state);	// Play village and check for error

		/* Test outcome */
		if (state.handCount[player] != control.handCount[player]) { // Village draws 1 when played, so no net draw
			handCountErrors++;
		}

		if (state.hand[player][state.handCount[player] - 1] == village) {	// Drawn card should not be village
			drawnCardErrors++;
		}

		if ((state.deckCount[player] + state.discardCount[player]) != (control.deckCount[player] + control.discardCount[player] -1)) {	// Village draws 1, so 1 card should be missing between deck and discard (discard shuffled if 0 in deck)
			deckCountErrors++;
		}

		if (state.playedCardCount != control.playedCardCount + 1) {	// Card was played, so playedCardCount should increment
			playedCountErrors++;
		}

		if (!((state.playedCardCount > 0) && (state.playedCards[state.playedCardCount - 1] == village))) {	// playedCardCount incremented and top card of playedCards should be village
			playedErrors++;
		}

		if (state.numActions != control.numActions + 2) {	// Village +2 actions, so action cound should be control+2
			numActionsError++;
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
		memcpy(&state.numActions, &control.numActions, sizeof(int));	// numActions
		

		if (memcmp(control, state, sizeof(struct gameState))) {	// Check for out of bound changes to game state 	
			errors++;
		}	
		memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	
	}
		
	/* Display results crash */
	printf("village: ");
	if (crashErrors) {
		printf("FAIL does not return error\n");
	} 
	else {
		printf("PASS does not return error\n");
	}

	/* Display results draw card */
	printf("village: ");
	if (handCountErrors) {
		printf("FAIL draws card into correct hand\n");
	} 
	else {
		printf("PASS draws card into correct hand\n");
	}

	/* Display results drawn card not village */

	printf("village: ");
	if (drawnCardErrors) {
		printf("FAIL drawn card not village\n");
	} 
	else {
		printf("PASS drawn card not village\n");
	}

	/* Display results deck+discard size reduced by three */
	printf("village: ");
	if (deckCountErrors) {
		printf("FAIL draws card from correct deck\n");
	} 
	else {
		printf("PASS draws card from correct deck\n");
	}
	
	/* Display results played count incremented */
	printf("village: ");
	if (playedCountErrors) {
		printf("FAIL played card count incremented\n");
	} 
	else {
		printf("PASS played card count incremented\n");
	}
	
	/* Display results village placed on top of played card pile */
	printf("village: ");
	if (playedErrors) {
		printf("FAIL placed on top of played card pile\n");
	} 
	else {
		printf("PASS placed on top of played card pile\n");
	}
	
	/* Display results numActions +2 */
	printf("village: ");
	if (numActionsErrors) {
		printf("FAIL action count increased by two\n");
	} 
	else {
		printf("PASS action count increased by two\n");
	}

	/* Display final results */
	if (!(crashErrors || handCountErrors || drawnCardErrors || deckCountErrors || playedCountErrors || playedErrors || numActionsErrors)){
		printf("village: ALL TESTS PASSED");
	}

	return 0;
}
