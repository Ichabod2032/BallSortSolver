#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"
#include "solver.h"

//TODO: Implement a priority system for valid moves. I believe this might
//		solve the issue of getting stuck in a loop sometime.

void PrintVials(const struct Queue q[], size_t numVials)
{
	printf("Printing vials...\n");
	for (int i = 0; i < numVials; i++)
	{
		printf("Vial %d: ", i + 1);
		for (int x = 0; x < q[i].count; x++)
		{
			printf("%c ", (char)q[i].Items[x]);
		}
		printf("\n");
	}
}

int EncodeMove(int moveFrom, int moveTo)
{
	return (moveFrom << 16) | moveTo;
}

int GetFrom(int encodedMove)
{
	return encodedMove >> 16;
}

int GetTo(int encodedMove)
{
	return encodedMove & 0xFFFF;
}

#ifdef DBG1
void PrintMoves(const struct Queue q)
{
	for (int i = 0; i < q.count; i++)
		printf("%d:%d ", GetFrom(q.Items[i])+1, GetTo(q.Items[i])+1);

	printf("\n");
}
#endif

int NeedsOne(const struct Queue vial)
{
	return vial.count == 3 &&
		   (vial.Items[0] == vial.Items[1]
		    &&
		    vial.Items[0] == vial.Items[2]);
}

//Returns the amount of balls that are the same color,
// but only continuously from the top of the stack. eg.
//
// \     /
//  | B |
//  | B |
//  | Y |
//  | B |
//  \___/
//
//   With this vial, the two continuous B's at the top would make
//    this function return 2.
int NumSameColorsTogether(const struct Queue vial)
{
	if (vial.count <= 1) return vial.count;

	int top = Peek(vial);
	int same = 1;
	for (int i = vial.count - 2; i >= 0; i--)
	{
		if (vial.Items[i] != top) break;

		same++;
	}

	return same;
}

int VialContainsSameColor(const struct Queue vial)
{
	if (vial.count == 0) return 0;
	if (vial.count == 1) return 1;

	int ball = Peek(vial);
	for (int i = vial.count - 2; i >= 0; i--)
		if (vial.Items[i] != ball) return 0;

	return 1;
}

int IsSmartMove(struct Queue vials[], int numVials, const struct Queue movelist, int moveFrom, int moveTo)
{
	int ballFrom = Peek(vials[moveFrom]);
	int ballTo = Peek(vials[moveTo]);

	if (vials[moveFrom].count == 0) return 0;
	if (vials[moveTo].count == 4) return 0;

	//This rule is confusing. If this move is the inverse
	// of a previous move, and there were no moves in-between
	// that touched these 2 vials, then this move is pointless.

	int inverse = EncodeMove(moveTo, moveFrom);
	for (int i = movelist.count - 1; i >= 0; i--)
	{
		int move = movelist.Items[i];
		if (move == inverse) return 0;

		if (GetFrom(move) == moveFrom ||
			GetTo(move) == moveFrom ||
			GetFrom(move) == moveTo ||
			GetTo(move) == moveTo) break;
	}

	if (VialContainsSameColor(vials[moveFrom]))
	{
		//If a vial is solved, or has only 3 balls
		// and they are all the same color, don't
		// move them
		if (vials[moveFrom].count == 4
			||
			vials[moveFrom].count == 3) return 0;

		//Don't move any balls from a partially solved vial of count=2
		// if the TO vial doesn't have space for all 2 of the balls
		if (vials[moveFrom].count == 2 && vials[moveTo].count > 2)
			return 0;

		//Don't move semi-solved vials into empty
		// vials. That's pointless.
		if (vials[moveTo].count == 0) return 0;
	}

	//If the vial to move to is empty...
	if (vials[moveTo].count == 0)
	{
		//...and the vial to move from has 4 balls total
		//  but the top 3 are the same color...
		if (vials[moveFrom].count == 4 &&
		    NumSameColorsTogether(vials[moveFrom]) == 3)
		{
			//search to make sure there isn't a vial with 1 ball
			// of the color we're looking for. If there is, then
			// we don't want to move the MoveFrom vial into an
			// empty vial

			for (int i = 0; i < numVials; i++)
			{
				if (i == moveFrom) continue;

				if (vials[i].count == 1 &&
					Peek(vials[moveFrom]) == Peek(vials[i]))
					return 0;
			}
		}

		return 1;
	}

	if (ballFrom == ballTo)
	{
		if (NumSameColorsTogether(vials[moveFrom]) == 2
			&&
			vials[moveTo].count > 2)
			return 0;
		else
			return 1;
	}
	else return 0;
}

void DoMove(struct Queue vials[], int encodedMove)
{
	int tmp = Pop(&vials[GetFrom(encodedMove)]);
	Push(&vials[GetTo(encodedMove)], tmp);
}

void UndoMove(struct Queue vials[], int encodedMove)
{
	int tmp = Pop(&vials[GetTo(encodedMove)]);
	Push(&vials[GetFrom(encodedMove)], tmp);
}

void PrintMoveList(const struct Queue movelist)
{
	for (int i = 0; i < movelist.count; i++)
	{
		printf("Move %d: %d to %d\n", i + 1
									, GetFrom(movelist.Items[i]) + 1
									, GetTo(movelist.Items[i]) + 1);
		getchar();
	}
}

int IsSolved(struct Queue vials[], size_t numVials)
{
	for (int i = 0; i < numVials; i++)
	{
		if (vials[i].count == 0) continue;

		if (vials[i].Items[0] != vials[i].Items[1]
			||
			vials[i].Items[0] != vials[i].Items[2]
			||
			vials[i].Items[0] != vials[i].Items[3])
			{
				return 0;
			}
	}

	return 1;
}

int Solver(struct Queue vials[], size_t numVials, struct Queue *movelist)
{

	if (IsSolved(vials, numVials))
		return 1;

	if (movelist->count > 300) return 0; //Let's see if this fixes anything

#ifdef DBG1
printf("\nLevel: %ld\n", movelist->count);
PrintVials(vials, numVials);
printf("\nmovelist: ");
PrintMoves(*movelist);
#endif
	//Build list of valid moves.
	struct Queue validMoves = NewQueue();

	for (int i = 0; i < numVials; i++)
		for (int j = 0; j < numVials; j++)
		{
			if (i == j) continue;

			//IsSmartMove considers several odd rules, such as
			// if there are 4 same-colored balls in 1 vial, and there
			// is an empty vial somewhere, it would not be smart to
			// move any of those balls to the empty vial, even though
			// it is a valid move.
#ifdef DBG2
printf("\nValid Move %d to %d ? %s\n", i, j, IsSmartMove(vials, i, j) == 1 ? "true" : "false");
#endif
			if (IsSmartMove(vials, numVials, *movelist, i, j))
				Push(&validMoves, EncodeMove(i, j));

			//To do:
			// Add a check for a move between 2 vials that undoes itself
			// later on without any other move affecting those vials
			// inbetween. eg:
			// Moves: 1:3 4:9 7:2 5:6 6:4 3:1
			// the last "3:1" move is pointless and should not be included
			// in the valid move list because no other moves inbetween used
			// those vials.
		}

#ifdef DBG1
printf("Valid Moves: ");
PrintMoves(validMoves);
#endif

	//Do moves and test for solved-ness
	for (int i = 0; i < validMoves.count; i++)
	{
		DoMove(vials, validMoves.Items[i]);
		Push(movelist, validMoves.Items[i]);
#ifdef DBG1
printf("Move %d:%d Done\n", GetFrom(Peek(*movelist))+1, GetTo(Peek(*movelist))+1);
PrintVials(vials, numVials);
getchar();
#endif
		if (Solver(vials, numVials, movelist))
			return 1;
		else
		{
			//undo moves
			UndoMove(vials, Pop(movelist));
#ifdef DBG1
printf("Undo move\n");
PrintVials(vials, numVials);
#endif
		}
	}

	FreeQueue(validMoves);
	return 0;
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("Test tube puzzle solver. Usage:\n");
		printf(" ./solver <NumVials> <<Vial1 Contents> <Vial2 Contents>...>\n");
		printf(" example:\n ./solver 3 ABAB BABA\n");
		printf(" This will run the program with 3 vials, the last one of which is empty\n");
		printf(" The first vial contains color A on top, followed by colors B, A, and B underneath.");
		printf(" Colors can be labeled with any character as long as you keep them consistent.\n");
		return 0;
	}

	int numvials = 1;
	numvials = atoi(argv[1]);
	struct Queue vials[numvials];

	for (int i = 0; i < numvials; i++)
		vials[i] = NewQueue();

	unsigned char inputCheck[256] = { 0 };
	for (int i = 2; i < argc && (i - 2) < numvials; i++)
	{
		for (int x = strnlen(argv[i], 4) - 1; x >= 0; x--)
		{
			inputCheck[(unsigned char)argv[i][x]]++;

			if (!Push(&vials[i - 2], (int)argv[i][x]))
			{
				printf("Input invalid. Exiting.\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	for (int i = 0; i < 256; i++)
	{
		if (inputCheck[i] != 4 && inputCheck[i] != 0)
		{
			printf("Not 4 balls per color. Exiting.\n");
			exit(EXIT_FAILURE);
		}
	}
#ifdef DBG1
printf("Start:\n");
PrintVials(vials, numvials);
#endif
	struct Queue movelist = NewQueue();

	if (Solver(vials, numvials, &movelist))
	{
		printf("Solution found in %zu moves!\nPress Enter to iterate though move list.\n", movelist.count);
		PrintMoveList(movelist);
	}
	else printf("No solutions found\n");

#ifdef DBG1
printf("End:\n");
PrintVials(vials, numvials);
#endif

	FreeQueue(movelist);
	for (int i = 0; i < numvials; i++)
		FreeQueue(vials[i]);

	exit(EXIT_SUCCESS);
}
