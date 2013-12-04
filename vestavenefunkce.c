// pretypovani
void tovarint(item *item){
	switch(item->type){
		case BOOL:
			item->type = BOOL;
			break;
		case VARDOUBLE:
			item->type = VARDOUBLE;
			break;
		case STRING:

			item->type = STRING;
			break;
		default:
			print_error(E_INT,"chyba pri pretypovani");
			break;
	}
}

void tobool(item *item){
	switch(item->type){
		case VARINT:
			if(item->data != 0){
				item->data = 1;
			}
			item->type = VARINT;
			break;
		case VARDOUBLE:
			if(item->data == 0){
				item->data = 0.0;
			}else{
				item->data = 1.0;
			}
			item->type = VARDOUBLE;
			break;
		case STRING:
			if(item->data == 0){
				item->data = "";
			}else{
				item->data = "1";
			}
			item->type = STRING;
			break;
		default:
			print_error(E_INT,"chyba pri pretypovani");
			break;
	}
}

void todouble(item *item){
	switch(item->type){
		case VARINT:
			item->data = (int)item->data;
			item->type = VARINT;
			break;
		case STRING:

			break;
		case BOOL:
			if(item->data == 0.0){
				item->data = 0;
			}else{
				item->data = 1;
			}
			item->type = BOOL;
			break;
		default:
			print_error(E_INT,"chyba pri pretypovani");
			break;
	}
}

void tostring(item *item){
	switch(item->type){
		case VARINT:

			item->type = VARINT;
			break;
		case VARDOUBLE:

			item->type = VARDOUBLE;
			break;
		case BOOL:
			if(strcmp (item->data,"") == 0){
				free(item->data);
				item->data = 0;
			}else{
				free(item->data);
				item->data = 1;
			}
			item->type = BOOL;
			break;
		default:
			print_error(E_INT,"chyba pri pretypovani");
			break;
	}
}

void pretypovani(item *item,int to){
	switch(to){
		case VARINT:
			tovarint(item);
			break;
		case BOOL:
			tobool(item);
			break;
		case VARDOUBLE:
			todouble(item);
			break;
		case STRING:
			tostring(item);
			break;
		default:
			break;
	}
}




// vestavene funkce 
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