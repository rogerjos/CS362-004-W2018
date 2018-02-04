/*******************************************************************************
 * Program: cardtest4.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: test for Adventurer card in dominion.c
 * Card text: "Reveal cards from your deck until you reveal 2 Treasure cards. 
 *				Put those Treasure cards into your hand and discard the other 
 *				revealed cards."
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
		treasureCount,	
		crashErrors = 0,	
		handCountErrors = 0,	
		treasureErrors = 0;
		deckCountErrors = 0,	
		playedCountErrors = 0,	
		playedErrors = 0,
		totalErrors = 0;

	int k[10] = {cutpurse, council_room, feast, gardens, mine,	// Arbitrary cards
					remodel, smithy, village, baron, great_hall};
	
	int treasure[2] = {-1, -1};	 // Holds last two cards drawn for treasure check

	/* Set up game */
	memset(&state, 23, sizeof(struct gameState)); // Clear data
	if (initializeGame(MAX_PLAYERS, k, SEED, &state)) {	// Print error and exit if new game fails
			printf("adventurer ABORT: initializeGame() exited with error.\n");
			return 1;
	}
	memcpy(&control, &state, sizeof(struct gameState));	// Save beginning game state	
	
	for (player = 0; player < MAX_PLAYERS; player++) {	// Run test for each player

		/* Get and play adventurer */
		drawCard(player, state);	// Draw a card
		state.hand[player][state.handCount[player] - 1] = adventurer // Set most recently drawn card to adventurer
		crashErrors += playCard(state.handCount[player] - 1, 0, 0, 0, &state);	// Play adventurer and check for error

		/* Test outcome */
		if (state.handCount[player] != control.handCount[player] + 1) { // Adventurer draws 2 when played, so +1 net to hand
			handCountErrors++;
		}

		if (state.handCount[player] < 2) { // Adventurer draws two treasures, so hand must contain at least two cards
			treasureErrors++;
		}
		else {	// Check whether most recently-drawn two cards are treasures
			treasure[0] = state.hand[player][state.handCount[player] - 1];	// treasure[0] = most recently drawn card
			treasure[1] = state.hand[player][state.handCount[player] - 2];	// trasure [0] = next most recently drawn card
			if ((treasure[0] != copper && (treasure[0] != silver && treasure[0] != gold))
				||
				(treasure[1] != copper && (treasure[1] != silver && treasure[1] != gold))) {
				treasureErrors++;
			}
		}

		if ((state.deckCount[player] + state.discardCount[player]) != (control.deckCount[player] + control.discardCount[player] - 2)) {	// Adventurer draws 2, and puts non-treasures into discard, so two cards should be missing between deck and discard
			deckCountErrors++;
		}

		if (state.playedCardCount != control.playedCardCount + 1) {	// Adventurer was played, so playedCardCount should increment
			playedCountErrors++;
		}

		if (!((state.playedCardCount > 0) && (state.playedCards[state.playedCardCount - 1] == adventurer))) {	// playedCardCount incremented and top card of playedCards should be adventurer
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
			errors++;
		}	
		memcpy(&state, &control, sizeof(struct gameState));	//Revert to beginning game state	
	}
		
	/* Display results crash */
	printf("adventurer: ");
	if (crashErrors) {
		printf("FAIL does not return error\n");
	} 
	else {
		printf("PASS does not return error\n");
	}

	/* Display results draw 2 cards */
	printf("adventurer: ");
	if (handCountErrors) {
		printf("FAIL draws two cards into correct hand\n");
	} 
	else {
		printf("PASS draws two cards into correct hand\n");
	}

	/* Display results cards drawn both treasures */
	printf("adventurer: ");
	if (treasureErrors) {
		printf("FAIL cards drawn both treasures\n");
	} 
	else {
		printf("PASS cards drawn both treasures\n");
	}

	/* Display results deck+discard size reduced by two */
	printf("adventurer: ");
	if (deckCountErrors) {
		printf("FAIL two cards removed from correct deck + discard\n");
	} 
	else {
		printf("PASS two cards removed from correct deck + discard\n");
	}
	
	/* Display results played count incremented */
	printf("adventurer: ");
	if (playedCountErrors) {
		printf("FAIL played card count incremented\n");
	} 
	else {
		printf("PASS played card count incremented\n");
	}
	
	/* Display results adventurer placed on top of played card pile */
	printf("adventurer: ");
	if (playedErrors) {
		printf("FAIL placed on top of played card pile\n");
	} 
	else {
		printf("PASS placed on top of played card pile\n");
	}

	totalErrors = crashErrors + handCountErrors + treasureErrors + deckCountErrors + playedCountErrors + playedErrors;

	/* Display final results */
	if (!totalErrors) {
		printf("adventurer: ALL TESTS PASSED");
	}

	return 0;
}
