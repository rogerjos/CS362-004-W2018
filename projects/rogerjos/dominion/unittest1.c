/*******************************************************************************
 * Program: unittest1.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: Unit test for updateCoins() function in dominion.c
 * Note: Based on testUpdateCoins.c on Canvas. Test was improved to check
 *		 for other interesting coin combinations and unexpected state changes.
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

// Toggle verbose output 0 = off, else on
#define VERBOSE 1

#define ESTATE_VALUE 0
#define COPPER_VALUE 1
#define SILVER_VALUE 2
#define GOLD_VALUE 3
#define SEED 23
#define MAX_TEST_HAND_SIZE 10
#define MAX_TEST_BONUS 15


int main() {
	struct gameState state,	// Current game state 
					 control;	// For detecting unexpected state changes

	int bonus,	// Current bonus
		handCount,	// Current cards in hand
		i,	// A general counter/variable
		player,	// Current player	
		valErrors = 0,
		boundErrors = 0,	
		totalErrors = 0;	

	int k[10] = {adventurer, council_room, feast, gardens, mine,	// Arbitrary cards
					remodel, smithy, village, baron, great_hall};

	int es[MAX_TEST_HAND_SIZE]; // An array of estates
	int cu[MAX_TEST_HAND_SIZE];	// An array of copper coins
	int ag[MAX_TEST_HAND_SIZE];	// An array of silver coins
	int au[MAX_TEST_HAND_SIZE];	// An array of gold coins

	/* Fill card arrays */
	for (i = 0; i < MAX_TEST_HAND_SIZE; i++) {	
		es[i] = estate;
		cu[i] = copper;
		ag[i] = silver;
		au[i] = gold;
	}

	for (player = 0; player < MAX_PLAYERS; player++) { // Test all players

		for (handCount = 0; handCount <= MAX_TEST_HAND_SIZE; handCount++) { // Test hand sizes from 0 to max

			for (bonus = 0; bonus <= MAX_TEST_BONUS; bonus++) {	// Check all available bonuses

				/* Clear junk data from gameState structs */
				memset(&state, 23, sizeof(struct gameState));
				memset(&control, 23, sizeof(struct gameState));

				/* Fill state with possible data & copy it to control */
				if (initializeGame(MAX_PLAYERS, k, SEED, &state)) {
					if (VERBOSE) printf("ABORT: initializeGame() exited with error.\n");
					return 1;
				}
				state.handCount[player] = handCount;	// Update handCount in state to match test loop
				memcpy(&control, &state, sizeof(struct gameState));		
			
				/* Test Hand of Estates */
				memcpy(state.hand[player], es, sizeof(int) * handCount);
				updateCoins(player, &state, bonus);
				i = (ESTATE_VALUE * handCount) + bonus;
				if (state.coins != i) {
					if (VERBOSE) printf("updateCoins() FAIL: found %d coins when %d expected (%d Estates, %d bonus)\n", state.coins, i, handCount, bonus);
				valErrors++;
				}

				/* Check for unwanted modifications to game state */
				memcpy(control.hand[player], state.hand[player], sizeof(int) * handCount);
				memcpy(&control.coins, &state.coins, sizeof(int));
				if (memcmp(&state, &control, sizeof(struct gameState))) {
					if (VERBOSE) printf("updateCoins() FAIL: out of bound game state values modified. (%d Estates, %d bonus)\n", handCount, bonus);
					boundErrors++;
				}

				/* Test Hand of Copper */
				memcpy(state.hand[player], cu, sizeof(int) * handCount);
				updateCoins(player, &state, bonus);
				i = (COPPER_VALUE * handCount) + bonus;
				if (state.coins != i) {
					if (VERBOSE) printf("updateCoins() FAIL: found %d coins when %d expected (%d Copper, %d bonus)\n", state.coins, i, handCount, bonus);
					valErrors++;
				}
	
				/* Check for unwanted modifications to game state */
				memcpy(control.hand[player], state.hand[player], sizeof(int) * handCount);
				memcpy(&control.coins, &state.coins, sizeof(int));
				if (memcmp(&state, &control, sizeof(struct gameState))) {
					if (VERBOSE) printf("updateCoins() FAIL: out of bound game state values modified. (%d Copper,, %d bonus)\n", handCount, bonus);
					boundErrors++;
				}

				/* Test Hand of Silver */
				memcpy(state.hand[player], ag, sizeof(int) * handCount);
				updateCoins(player, &state, bonus);
				i = (SILVER_VALUE * handCount) + bonus;
				if (state.coins != i) {
					if (VERBOSE) printf("updateCoins() FAIL: found %d coins when %d expected (%d Silver, %d bonus)\n", state.coins, i, handCount, bonus);
					valErrors++;
				}

				/* Check for unwanted modifications to game state */
				memcpy(control.hand[player], state.hand[player], sizeof(int) * handCount);
				memcpy(&control.coins, &state.coins, sizeof(int));
				if (memcmp(&state, &control, sizeof(struct gameState))) {
					if (VERBOSE) printf("updateCoins() FAIL: out of bound game state values modified. (%d Silver, %d bonus)\n", handCount, bonus);
					boundErrors++;
				}
	
				/* Test Hand of Gold */
				memcpy(state.hand[player], au, sizeof(int) * handCount);
				updateCoins(player, &state, bonus);
				i = (GOLD_VALUE * handCount) + bonus;
				if (state.coins != i) {
					if (VERBOSE) printf("updateCoins() FAIL: found %d coins when %d expected (%d Gold, %d bonus)\n", state.coins, i, handCount, bonus);
					valErrors++;
				}

				/* Check for unwanted modifications to game state */
				memcpy(control.hand[player], state.hand[player], sizeof(int) * handCount);
				memcpy(&control.coins, &state.coins, sizeof(int));
				if (memcmp(&state, &control, sizeof(struct gameState))) {
					if (VERBOSE) printf("updateCoins() FAIL: out of bound game state values modified. (%d Gold, %d bonus)\n", handCount, bonus);
					boundErrors++;
				}			

				/* Check combinations of coin types */
				if (handCount == 2) {

					/* Test Hand of Copper/Silver */
					state.hand[player][0] = cu[0];
					state.hand[player][1] = ag[1];
					updateCoins(player, &state, bonus);
					i = (COPPER_VALUE + SILVER_VALUE) + bonus;
					if (state.coins != i) {
						if (VERBOSE) printf("updateCoins() FAIL: found %d coins when %d expected (1 Copper, 1 Silver, %d bonus)\n", state.coins, i, bonus);
						valErrors++;
					}

					/* Check for unwanted modifications to game state */	
					memcpy(control.hand[player], state.hand[player], sizeof(int) * handCount);
					memcpy(&control.coins, &state.coins, sizeof(int));
					if (memcmp(&state, &control, sizeof(struct gameState))) {
						if (VERBOSE) printf("updateCoins() FAIL: modified out of bound game state values. (1 Copper, 1 Silver, %d bonus)\n", bonus);
						boundErrors++;
					}

					/* Test Hand of Copper/Gold */
					state.hand[player][0] = cu[0];
					state.hand[player][1] = au[1];
					updateCoins(player, &state, bonus);
						i = (COPPER_VALUE + GOLD_VALUE) + bonus;
					if (state.coins != i) {
						if (VERBOSE) printf("updateCoins() FAIL: found %d coins when %d expected (1 Copper, 1 Gold, %d bonus)\n", state.coins, i, bonus);
						valErrors++;
					}

					/* Check for unwanted modifications to game state */
					memcpy(control.hand[player], state.hand[player], sizeof(int) * handCount);
					memcpy(&control.coins, &state.coins, sizeof(int));
					if (memcmp(&state, &control, sizeof(struct gameState))) {
						if (VERBOSE) printf("updateCoins() FAIL: out of bound game state values modified. (1 Copper, 1 Gold, %d bonus)\n", bonus);
						boundErrors++;
					}
			
					/* Test Hand of Silver/Gold */
					state.hand[player][0] = ag[0];
					state.hand[player][1] = au[1];
					updateCoins(player, &state, bonus);
							i = (SILVER_VALUE + GOLD_VALUE) + bonus;
					if (state.coins != i) {
						if (VERBOSE) printf("updateCoins() FAIL: found %d coins when %d expected (1 Silver, 1 Gold, %d bonus)\n", state.coins, i, bonus);
						valErrors++;
					}
	
					/* Check for unwanted modifications to game state */
					memcpy(control.hand[player], state.hand[player], sizeof(int) * handCount);
					memcpy(&control.coins, &state.coins, sizeof(int));
					if (memcmp(&state, &control, sizeof(struct gameState))) {
						if (VERBOSE) printf("updateCoins() FAIL: out of bound game state values modified. (1 Silver, 1 Gold, %d bonus)\n", bonus);
						boundErrors++;
					}
				}

				else if (handCount == 3) {

					/* Test Hand of Copper/Silver/Gold */
					state.hand[player][0] = cu[0];
					state.hand[player][1] = ag[1];
					state.hand[player][2] = au[2];
					updateCoins(player, &state, bonus);
					i = COPPER_VALUE + SILVER_VALUE + GOLD_VALUE + bonus;
					if (state.coins != i) {
						if (VERBOSE) printf("updateCoins() FAIL: found %d coins when %d expected (1 Copper, 1 Silver, 1 Gold, %d bonus)\n", state.coins, i, bonus);
						valErrors++;
					}					
	
					/* Check for unwanted modifications to game state */
					memcpy(control.hand[player], state.hand[player], sizeof(int) * handCount);
					memcpy(&control.coins, &state.coins, sizeof(int));
					if (memcmp(&state, &control, sizeof(struct gameState))) {
						if (VERBOSE) printf("updateCoins() FAIL: out of bound game state values modified. (1 Copper, 1 Silver, 1 Gold, %d bonus)\n", bonus);
						boundErrors++;
					}			
				}
			}
		}
	}

	printf("updateCoins(): ");
	if (valErrors) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" coin combination values as expected\n");

	printf("updateCoins(): ");
	if (boundErrors) {
		printf("FAIL");
	}
	else {
		printf("PASS");
	}
	printf(" no out of bound modifications to game state\n");

	totalErrors = valErrors + boundErrors;
	
	/* Display final results */
	if (!totalErrors) {
		printf("updateCoins(): ALL TESTS PASSED.\n");
	}

	return 0;
}
