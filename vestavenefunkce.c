#include "ial.h"

void boolval(item *item){
	switch(item->type){
		case VARINT:
			
			break;
		case VARDOUBLE:
			break;
		case STRING:
			break;
		case VARBOOL:
			break;
		case NIL:
			break;
		default:
			print_error(E_SYN,"token nelze prevest");
			break;
	}
}

int main(){
	return 0;
}