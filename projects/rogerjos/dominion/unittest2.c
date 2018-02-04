/*******************************************************************************
 * Program: unittest1.c
 * Author: Joshua L. Rogers
 * Date: 4 Feb 2018
 * Course: CS362
 * Description: Unit test for drawCard() function in dominion.c
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

#define SEED 23

int main() {
	struct gameState state,	// Current game state 
					 control;	// For detecting unexpected state changes

	int i,	// A general counter/variable
		player,	// Current player
		deckSize;	
		errors = 0;	// Counter for test failures

	int k[10] = {adventurer, council_room, feast, gardens, mine,	// Arbitrary cards
					remodel, smithy, village, baron, great_hall};

	for (player = 0; player < MAX_PLAYERS; player++) { // Test all players

				/* Clear junk data from gameState structs */
				memset(&state, 23, sizeof(struct gameState));
				memset(&control, 23, sizeof(struct gameState));

				/* Fill state with possible data & copy it to control */
				if (initializeGame(MAX_PLAYERS, k, SEED, &state)) {
					printf("drawCard() ABORT: initializeGame() exited with error.\n");
					return 1;
				}
				memcpy(&control, &state, sizeof(struct gameState));		

				/* Test drawing entire deck */
				deckSize = state.deckCount[player]; 
				for (i = 0; i < deckSize; i++) { 
					if(drawCard(player, &state)) {
						printf("drawCard() FAIL: Exited with error. (Player %d with %d cards in deck, %d cards in hand, %d discarded)\n")
					}
				}

				state.handCount[player] = handCount;	// Update handCount in state to match test loop
			

				/* Test Hand of Estates */
				memcpy(state.hand[player], es, sizeof(int) * handCount);
				updateCoins(player, &state, bonus);
				i = (ESTATE_VALUE * handCount) + bonus;
				if (state.coins != i) {
					printf("FAILURE %d: updateCoins() found %d coins when %d expected (%d Estates, %d bonus)\n", ++errors, state.coins, i, handCount, bonus);
				}

				/* Check for unwanted modifications to game state */
				memcpy(control.hand[player], state.hand[player], sizeof(int) * handCount);
				memcpy(&control.coins, &state.coins, sizeof(int));
				if (memcmp(&state, &control, sizeof(struct gameState))) {
					printf("FAILURE %d: updateCoins() modified out of bound game state values. (%d Estates, %d bonus)\n", ++errors, handCount, bonus);
				}
	if (!errors) printf("ALL TESTS PASSED.\n");
	return 0;
}

/* Strcutcure notes:
	Init a game
	all players up to max players take turns drawing cards.
		for each card check that handcount went up, deck count went down, drawn card = deck card, and no other gamestate changed unless a shuffle was necessary.
	Do this until a shuffle is necessary, at which point test the normal stuff, but take shuffle into account.
	Also try to draw fro an empty deck with no discard. Expect error -1 and no change


int drawCard(int player, struct gameState *state)
{	int counet;
  int deckCounter;
  if (state->deckCount[player] <= 0){//Deck is empty
    
    //Step 1 Shuffle the discard pile back into a deck
    int i;
    //Move discard to deck
    for (i = 0; i < state->discardCount[player];i++){
      state->deck[player][i] = state->discard[player][i];
      state->discard[player][i] = -1;
    }

    state->deckCount[player] = state->discardCount[player];
    state->discardCount[player] = 0;//Reset discard

    //Shufffle the deck
    shuffle(player, state);//Shuffle the deck up and make it so that we can draw
   
    if (DEBUG){//Debug statements
      printf("Deck count now: %d\n", state->deckCount[player]);
    }
    
    state->discardCount[player] = 0;

    //Step 2 Draw Card
    count = state->handCount[player];//Get current player's hand count
    
    if (DEBUG){//Debug statements
      printf("Current hand count: %d\n", count);
    }
    
    deckCounter = state->deckCount[player];//Create a holder for the deck count

    if (deckCounter == 0)
      return -1;

    state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to hand
    state->deckCount[player]--;
    state->handCount[player]++;//Increment hand count
  }

  else{
    int count = state->handCount[player];//Get current hand count for player
    int deckCounter;
    if (DEBUG){//Debug statements
      printf("Current hand count: %d\n", count);
    }

    deckCounter = state->deckCount[player];//Create holder for the deck count
    state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to the hand
    state->deckCount[player]--;
    state->handCount[player]++;//Increment hand count
  }

  return 0;
}


