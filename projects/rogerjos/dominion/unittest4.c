/*******************************************************************************
 * Program: unittest4.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: Unit test for disardCard() function in dominion.c
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

	int i,		// A general counter/variable
		player,		// Current player
		discarded,	// Last card discarded
		discardCount,	// Counter for number of cards discarded
		trashFlag,	// Tracks whether card is to be trashed
		testHandCount = 10,

		// error counters 
		errLowOnCount = 0,
		errLowOnPile = 0,
		errLowOffCount = 0,
		errLowOffPile = 0,
		errLowBound = 0,
		errMidOnCount = 0,
		errMidOnPile = 0,
		errMidOffCount = 0,
		errMidOffPile = 0,
		errMidBound = 0,
		errTopOnCount = 0,
		errTopOnPile = 0,
		errTopOffCount = 0,
		errTopOffPile = 0,
		errTopBound = 0,
		errFinal = 0;	// Counter for final test result

	int testHand[10] = {estate, duchy, province, copper, silver, gold, 
						adventurer, council_room, feast, gardens};

	memset(&control, 23, sizeof(struct gameState));	// Clear junk data
	control.playedCardCount = 0;	// Required to avoid segfault due to discardCard() bug misdirecting cards.
	for (player = 0; player < MAX_PLAYERS; player++) {	// Update all players' hand & discard
		memcpy(control.hand[player], testHand, sizeof(int) * testHandCount);	// Give player a test hand
		control.handCount[player] = testHandCount;	// Update player hand size
		control.discardCount[player] = 0;	// Reset player discard count
	}
	
	for (trashFlag = 0; trashFlag < 2; trashFlag++) {	// Test both trashed and untrashed discards

		for (player = 0; player < MAX_PLAYERS; player++) {	// Test all players

			/* Reset game state */
			memcpy(&state, &control, sizeof(struct gameState));
			discardCount = 0;

			/* Test discarding top card in hand */
			for (i = 1; i <= testHandCount; i++) {	// While hand is not empty
				discarded = state.hand[player][testHandCount - i];
				discardCard(state.handCount[player] - 1, player, &state, trashFlag);	// Discard the last card in hand

				if (!trashFlag) {	// Check discard count if trashFlag is off
					discardCount++;
					if (state.discardCount[player] != discardCount) {
						if (VERBOSE) {
							printf("discardCard(): FAIL player %d discardCount is %d -- expected %d (trashFlag %d)\n", player + 1, state.discardCount[player], discardCount, trashFlag);
						}
						state.discardCount[player] = discardCount;	// Correct game state variable
						errTopOffCount++;
					}
					
					if (state.discard[player][state.discardCount[player] - 1] != discarded) {	// Check discard pile if trashFlag is off
						if (VERBOSE) {
							printf("discardCard(): FAIL discarded card not copied to player discard array with trashFlag on\n");
						}
						errTopOffPile++;
					}
				}
				else {	// Check discard count if trashFlag is on					
					if (state.discardCount[player] > discardCount) {
						if (VERBOSE) {
							printf("discardCard(): FAIL player %d discardCount is %d -- expected %d (trashFlag %d)\n", player + 1, state.discardCount[player], discardCount, trashFlag);
						}
						errTopOnCount++;
						state.discardCount[player] = discardCount;	// Correct game state variable
					}
					
					if (state.discard[player][state.discardCount[player] - 1] == discarded) {	// Check discard pile if trashFlag on
						if (VERBOSE) {
							printf("discardCard(): FAIL discarded card copied to player discard array with trashFlag on\n");
						}
						errTopOnPile++;
					}
				}	
			}
			
			/* All cards discarded. Check for out of bound changes to game state */
			memcpy(state.hand[player], testHand, sizeof(int) * testHandCount);	// Reset player hand to test hand
			state.handCount[player] = testHandCount;	// Reset player hand size
			memset(state.discard[player], 23, sizeof(int) * testHandCount);	// Clear player discard pile
			state.discardCount[player] = 0;	// Reset player discard count
			if (memcmp(&control, &state, sizeof(struct gameState))) {
				if (VERBOSE) {
					printf("discardCard(): FAIL Out of bound changes made to game state struct.\n");
				}
				errTopBound++;
			}
		}
	}


	/* Test discarding first card in hand */
	for (trashFlag = 0; trashFlag < 2; trashFlag++) {	// Test both trashed and untrashed discards

		for (player = 0; player < MAX_PLAYERS; player++) {	// Test all players


			/* Reset game state  */
			memcpy(&state, &control, sizeof(struct gameState));
			discardCount = 0;

			/* While hand not empty */
			for (i = 0; i < testHandCount; i++) {
				discarded = state.hand[player][0];
				discardCard(0, player, &state, trashFlag);	// Discard the first card in hand

				if (!trashFlag) {	// Check discard count if trashFlag is off
					discardCount++;
					if (state.discardCount[player] != discardCount) {
						if (VERBOSE) {
							printf("discardCard(): FAIL player %d discardCount is %d -- expected %d (trashFlag %d)\n", player + 1, state.discardCount[player], discardCount, trashFlag);
						}
						state.discardCount[player] = discardCount;	// Correct game state variable
						errLowOffCount++;
					}
					
					if (state.discard[player][state.discardCount[player] - 1] != discarded) {	// Check discard pile if trashFlag is off
						if (VERBOSE) {
							printf("discardCard(): FAIL discarded card not copied to player discard array with trashFlag on\n");
						}
						errLowOffPile++;
					}
				}
				else {	// Check discard count if trashFlag is on					
						if (state.discardCount[player] > discardCount) {
						if (VERBOSE) {
							printf("discardCard(): FAIL player %d discardCount is %d -- expected %d (trashFlag %d)\n", player + 1, state.discardCount[player], discardCount, trashFlag);
						}
						errLowOnCount++;
						state.discardCount[player] = discardCount;	// Correct game state variable
					}
					
					if (state.discard[player][state.discardCount[player] - 1] == discarded) {	// Check discard pile if trashFlag on
						if (VERBOSE) {
							printf("discardCard(): FAIL discarded card copied to player discard array with trashFlag on\n");
						}
						errLowOnPile++;
					}
				}
			}
			
			/* All cards discarded. Check for out of bound changes to game state */
			memcpy(state.hand[player], testHand, sizeof(int) * testHandCount);	// Reset player hand to test hand
			state.handCount[player] = testHandCount;	// Reset player hand size
			memset(state.discard[player], 23, sizeof(int) * testHandCount);	// Clear player discard pile
			state.discardCount[player] = 0;	// Reset player discard count
			if (memcmp(&control, &state, sizeof(struct gameState))) {
				if (VERBOSE) {
					printf("discardCard(): FAIL Out of bound changes made to game state struct.\n");
				}
				errLowBound++;
			}
		}
	}


	/* Test discarding mid index card in */
	for (trashFlag = 0; trashFlag < 2; trashFlag++) {	// Test both trashed and untrashed discards

		for (player = 0; player < MAX_PLAYERS; player++) {	// Test all players

			/* Reset game state */
			memcpy(&state, &control, sizeof(struct gameState));
			discardCount = 0;

			/* While hand not empty */
			for (i = 1; i <= testHandCount; i++) {	// While hand is not empty
				discarded = state.hand[player][state.handCount[player]/2];
				discardCard((state.handCount[player]/2), player, &state, trashFlag);	// Discard a mid card in hand (int div)

				if (!trashFlag) {	// Check discard count if trashFlag is off
					discardCount++;
					if (state.discardCount[player] != discardCount) {
						if (VERBOSE) {
							printf("discardCard(): FAIL player %d discardCount is %d -- expected %d (trashFlag %d)\n", player + 1, state.discardCount[player], discardCount, trashFlag);
						}
						state.discardCount[player] = discardCount;	// Correct game state variable
						errMidOffCount++;
					}
					
					if (state.discard[player][state.discardCount[player] - 1] != discarded) {	// Check discard pile if trashFlag is off
						if (VERBOSE) {
							printf("discardCard(): FAIL discarded card not copied to player discard array with trashFlag on\n");
						}
						errMidOffPile++;
					}
				}
				else {	// Check discard count if trashFlag is on					
						if (state.discardCount[player] > discardCount) {
						if (VERBOSE) {
							printf("discardCard(): FAIL game state discardCount incremented with trashFlag on\n");
						}
						errMidOnCount++;
						state.discardCount[player] = discardCount;	// Correct game state variable
					}
					
					if (state.discard[player][state.discardCount[player] - 1] == discarded) {	// Check discard pile if trashFlag on
						if (VERBOSE) {
							printf("discardCard(): FAIL discarded card copied to player discard array with trashFlag on\n");
						}
						errMidOnPile++;
					}
				}	
			}
			
			/* All cards discarded. Check for out of bound changes to game state */
			memcpy(state.hand[player], testHand, sizeof(int) * testHandCount);	// Reset player hand to test hand
			state.handCount[player] = testHandCount;	// Reset player hand size
			memset(state.discard[player], 23, sizeof(int) * testHandCount);	// Clear player discard pile
			state.discardCount[player] = 0;	// Reset player discard count
			if (memcmp(&control, &state, sizeof(struct gameState))) {
				if (VERBOSE) {
					printf("discardCard(): FAIL Out of bound changes made to game state struct.\n");
				}
				errMidBound++;
			}
		}
	}	


	/* Display Results */
	// LOW
	printf("discardCard(): ");
	if (errLowOffCount) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard count incremented when discarding low index cards in hand\n");


	printf("discardCard(): ");
	if (errLowOffPile) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard pile updated when discarding low index cards in hand\n");


	printf("discardCard(): ");
	if (errLowOnCount) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard count not incremented when trashing low index cards in hand\n");


	printf("discardCard(): ");
	if (errLowOnPile) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard pile not updated when trashing low index cards in hand\n");

	printf("discardCard(): ");
	if (errLowBound) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" unrelated game state members unchanged when selecting low index cards in hand\n");

	// MID
	printf("discardCard(): ");
	if (errMidOffCount) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard count incremented when discarding mid index cards in hand\n");


	printf("discardCard(): ");
	if (errMidOffPile) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard pile updated when discarding mid index cards in hand\n");


	printf("discardCard(): ");
	if (errMidOnCount) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard count not incremented when trashing mid index cards in hand\n");


	printf("discardCard(): ");
	if (errMidOnPile) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard pile not updated when trashing mid index cards in hand\n");

	printf("discardCard(): ");
	if (errMidBound) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" unrelated game state members unchanged when selecting mid index cards in hand\n");

	// TOP
	printf("discardCard(): ");
	if (errTopOffCount) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard count incremented when discarding top index cards in hand\n");


	printf("discardCard(): ");
	if (errTopOffPile) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard pile updated when discarding top index cards in hand\n");


	printf("discardCard(): ");
	if (errTopOnCount) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard count not incremented when trashing top index cards in hand\n");


	printf("discardCard(): ");
	if (errTopOnPile) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" discard pile not updated when trashing top index cards in hand\n");

	printf("discardCard(): ");
	if (errTopBound) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" unrelated game state members unchanged when selecting top index cards in hand\n");

	errFinal = errLowOnCount + errLowOnPile + errLowOffCount + errLowOffPile + errLowBound + errMidOnCount + errMidOnPile + errMidOffCount + errMidOffPile + errMidBound + errTopOnCount + errTopOnPile + errTopOffCount + errTopOffPile + errTopBound;

	/* Display final results */
	if (!errFinal) {
		printf("discardCard(): ALL TESTS PASSED.\n");
	}

	return 0;
}
