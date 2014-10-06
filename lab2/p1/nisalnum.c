#include <ctype.h>
#include "reminder.h"

int nisalnum(int size, char* string){
	int i = 0;
	while(i < size){
		int q = isalnum(string[i]);
		if(!q)
			return 0;
		i++;
	}
	return 1;
}
