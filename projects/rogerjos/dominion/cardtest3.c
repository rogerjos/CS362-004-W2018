/*******************************************************************************
 * Program: cardtest3.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: test for great_hall card in dominion.c
 * Card text: "+1 Card, +1 Action, 1 Victory Point"
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

// VERBOSE triggers extra error messages. =0 off, else on
#define VERBOSE 0

#define SEED 23

int main() {
	struct gameState state,	// Current game state 
					 control;	// For detecting and reverting state changes

	int	player,
		returnVal,
		crashErrors = 0,	// Counter for great_hall returning errors
		handCountErrors = 0,	
		drawnCardErrors = 0,
		deckCountErrors = 0,	
		playedCountErrors = 0,	
		playedErrors = 0,	
		numActionsErrors = 0,
		boundErrors = 0,
		totalErrors = 0;
	int k[10] = {adventurer, council_room, feast, gardens, mine,	// Arbitrary cards
					remodel, village, minion, baron, cutpurse};	// No great_hall to ensure great_hall not drawn

	/* Set up game */
	memset(&state, 23, sizeof(struct gameState)); // Clear data
	if (initializeGame(MAX_PLAYERS, k, SEED, &state)) {	// Print error and exit if new game fails
			printf("great_hall ABORT: initializeGame() exited with error.\n");
			return 1;
	}
	memcpy(&control, &state, sizeof(struct gameState));	// Save beginning game state	
	
	for (player = 0; player < MAX_PLAYERS; player++) {	// Run test for each player

		/* Get and play great_hall */
		state.whoseTurn = player;	
		state.handCount[player] += 1;	// Create a card in hand
		state.hand[player][state.handCount[player] - 1] = great_hall; // Set card in hand to great_hall
		returnVal = playCard(state.handCount[player] - 1, 0, 0, 0, &state);	// Play great_hall and check for error
		if (returnVal) {
			if (VERBOSE) {
				printf("great_hall: FAIL returned error %d\n", returnVal);
			}
			crashErrors++; 
		}

		/* Test outcome */
		if (state.handCount[player] != control.handCount[player] + 1) { // Great_hall draws 1 when played, drew great_hall, net + 1
			if (VERBOSE) {
				printf("great_hall: FAIL player %d hand count is %d -- expected %d\n", player+1, state.handCount[player], control.handCount[player] + 1);
			}
			handCountErrors++;
		}

		if (state.hand[player][state.handCount[player] - 1] == great_hall) {	// Drawn card should not be great_hall
			drawnCardErrors++;
		}

		if ((state.deckCount[player] + state.discardCount[player]) != (control.deckCount[player] + control.discardCount[player] -1)) {	// Great_hall draws 1, so 1 card should be missing between deck and discard (discard shuffled if 0 in deck)
			deckCountErrors++;
		}

		if (state.playedCardCount != control.playedCardCount + 1) {	// Card was played, so playedCardCount should increment
			playedCountErrors++;
		}

		if (!((state.playedCardCount > 0) && (state.playedCards[state.playedCardCount - 1] == great_hall))) {	// playedCardCount incremented and top card of playedCards should be great_hall
			playedErrors++;
		}

		if (state.numActions != control.numActions) {	// Great_hall +1 action, one action used, so action count unchanged
			numActionsErrors++;
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
		memcpy(&state.coins, &control.coins, sizeof(int));	// revert coins
		memcpy(&state.whoseTurn, &control.whoseTurn, sizeof(int));	// revert turn
	

		if (memcmp(&control, &state, sizeof(struct gameState))) {	// Check for out of bound changes to game state 	
			boundErrors++;
		}	

		memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	
	}
		
	/* Display results crash */
	printf("great_hall: ");
	if (crashErrors) {
		printf("FAIL does not return error\n");
	} 
	else {
		printf("PASS does not return error\n");
	}

	/* Display results draw card */
	printf("great_hall: ");
	if (handCountErrors) {
		printf("FAIL draws card into correct hand\n");
	} 
	else {
		printf("PASS draws card into correct hand\n");
	}

	/* Display results drawn card not great_hall */
	printf("great_hall: ");
	if (drawnCardErrors) {
		printf("FAIL drawn card not great_hall\n");
	} 
	else {
		printf("PASS drawn card not great_hall\n");
	}

	/* Display results deck+discard size reduced by three */
	printf("great_hall: ");
	if (deckCountErrors) {
		printf("FAIL draws card from correct deck\n");
	} 
	else {
		printf("PASS draws card from correct deck\n");
	}
	
	/* Display results played count incremented */
	printf("great_hall: ");
	if (playedCountErrors) {
		printf("FAIL played card count incremented\n");
	} 
	else {
		printf("PASS played card count incremented\n");
	}
	
	/* Display results great_hall placed on top of played card pile */
	printf("great_hall: ");
	if (playedErrors) {
		printf("FAIL placed on top of played card pile\n");
	} 
	else {
		printf("PASS placed on top of played card pile\n");
	}
	
	/* Display results numActions +2 */
	printf("great_hall: ");
	if (numActionsErrors) {
		printf("FAIL action count incremented\n");
	} 
	else {
		printf("PASS action count incremented\n");
	}

	/* Display results for boundary errors */
	printf("great_hall: ");
	if (boundErrors) {
		printf("FAIL no unexpected modifications to game state\n");
	}
	else {
		printf("PASS no unexpected modifications to game state\n");
	}

	totalErrors = crashErrors + handCountErrors + drawnCardErrors + deckCountErrors + playedCountErrors + playedErrors + numActionsErrors + boundErrors;

	/* Display final results */
	if (!totalErrors){
		printf("great_hall: ALL TESTS PASSED\n");
	}

	return 0;
}
