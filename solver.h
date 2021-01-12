#ifndef solver_h
#define solver_h

void PrintVials(const struct Queue q[], size_t numVials);
int EncodeMove(int moveFrom, int moveTo);
int GetFrom(int encodedMove);
int GetTo(int encodedMove);
int NeedsOne(const struct Queue vial);
int NumSameColorsTogether(const struct Queue vial);
int VialContainsSameColor(const struct Queue vial);
int IsSmartMove(struct Queue vials[], int numVials, const struct Queue movelist, int moveFrom, int moveTo);
void DoMove(struct Queue vials[], int encodedMove);
void UndoMove(struct Queue vials[], int ecodedMove);
void PrintMoveList(const struct Queue movelist);
int IsSolved(struct Queue vials[], size_t numVials);
int Solver(struct Queue vials[], size_t numVials, struct Queue *movelist);

#endif
