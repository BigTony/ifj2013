typedef enum{
	// hodnoty ktere jsou mozne ulozit na zasobnik
	VALUE, // promena + konstanta
	PLUS, // +
	MINUS, // -
	MULTI, // *
	DIV, // /
	EQ, // ===
	NEQ, // !==
	GTHE, // >=
	LTHE, // <=
	GTH, // >
	LTH, // <
	COM, // ,	
	FUNC, // Funkce
	BRACE_L, // (
	BRACE_R, // )
	KONK, // konkatenace .

	// ridici symboly, decision
	D_G, // >
	D_L, // <
	D_E, // =
	D_B, // blank, prazdna cast tabulky
	ERROR,
} TExpType;

tabulka{		/* VALUE|PLUS|MINUS|MULTI|DIV|EQ|NEQ|GTHE|LTHE|GTH|LTH|COM|FUNC|BRACER_L|BRACE_R|KONK|END*/
	/* VALUE*/		{D_B,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_B,D_B,D_G,D_G,D_G},
	/* PLUS*/		  {D_L,D_G,D_G,D_L,D_L,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_L,D_L,D_G,D_G,D_G},
	/* MINUS*/		{D_L,D_G,D_G,D_L,D_L,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_L,D_L,D_G,D_G,D_G},
	/* MULTI*/		{D_L,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_L,D_L,D_G,D_G,D_G},
	/* DIV */	  	{D_L,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_L,D_L,D_G,D_G,D_G},
	/* EQ */		  {D_L,D_L,D_L,D_L,D_L,D_G,D_G,D_L,D_L,D_L,D_L,D_G,D_L,D_L,D_G,D_L,D_G},
	/* NEQ */		  {D_L,D_L,D_L,D_L,D_L,D_G,D_G,D_L,D_L,D_L,D_L,D_G,D_L,D_L,D_G,D_L,D_G},
	/* GTHE */		{},
	/* LTHE */		{},
	/* GTH */		  {},
	/* LTH */		  {},
	/* COM */		  {D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_E,D_L,D_L,D_E,D_L,D_B},
	/* FUNC */		{},
	/* BRACE_L */	{},
	/* BRACE_R */	{},
	/* KONK */		{D_L,D_G,D_G,D_L,D_L,D_G,D_G,D_G,D_G,D_G,D_G,D_G,D_L,D_L,D_G,D_G,D_G},
	/* END */   	{D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_L,D_B,D_L,D_L,D_B,D_L,D_B}
}
