typedef enum{
	// hodnoty ktere jsou mozne ulozit na zasobnik
	PLUS, // +
	MINUS, // -
	MULTI, // *
	DIV, // /
	KONK, // konkatenace .
	EQ, // ===
	NEQ, // !==
	GTHE, // >=
	LTHE, // <=
	GTH, // >
	LTH, // <
	BRACE_L, // (
	BRACE_R, // )
	VALUE, // promena + konstanta
	FUNC, // Funkce
	COM, // ,	
	END, // $

	// nonterminal
	NONTERM, // pravidlo r
	G,
	L,
	E,
	B,	
	ERROR
} TExpType;

#define MAX_INDEX 17

// #define G 0 // >
// #define L 1 // <
// #define E 2 // =
// #define B 3 // blank, prazdna cast tabulky


// struktury pro zasobnik
typedef struct TSItem{
	TExpType item;
	struct TSItem *ptrNext;
} *TSItemPtr;

typedef struct {
	TSItemPtr top;
} TStack;

// init zasobniku
void SInit(TStack *stack);
// vloz prvek na zasobnik
void SPush(TStack *stack,TExpType item);
// odstran prvek z vrcholu zasobniku
void SPop(TStack *stack);
// nacteni z vrcholu
TExpType STop(TStack *stack);
// bool prazdny zasobnik
int SEmpty(TStack *stack);
// odstran vsechny prvky
void SPopAll(TStack *stack);




