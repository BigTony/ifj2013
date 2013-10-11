// Error kody
#define E_OK 0
#define E_LEX 1 // Chyba v programu v ramci lexikalni analyzy
#define E_SYN 2 // Chyba v programu v ramci syntaktice analyzy
#define E_SEM_FCE 3 // Nedefinovana funkce, pokus o redefinici funkce
#define E_SEM_PARAM 4 // Chybejici parametr pri volani funkce
#define E_SEM_VAR 5 // Nedeklarovana promena
#define E_SEM_DIV_ZERO 10 // Deleni nulou
#define E_SEM_DOUBLEVAL 11 // Chyba pri pretypovani na cislo
#define E_SEM_TYPE 12 // Chyba typove kompatibility v aritmetickych a relacnich vyrazech
#define E_SEM_OTHER 13 // Ostatni semanticke chyby 
#define E_INTERN 99 // Interni chyba interpretu 
#define E_WRONG_PARAM 20 // Spatny pocet parametru
#define E_FILE 21 // Chyba v souboru, nejde otevrit apod.
#define STR_CR_ERR 22 // Chyba pri alokaci stringu

// Funkce pro obsluhu error kodu
void print_error(int err_code);