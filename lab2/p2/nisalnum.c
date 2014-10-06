#include <ctype.h>
#include "reminder.h"

int nisalnum(int size, char* string){
	//printf("alnum(%d)? %s\n", size, string);
	int i = 0;
	while(i < size){
		int q = isalnum(string[i]);
	//	printf("%c: %d\n", string[i], q);
		if(!q)
			return 0;
		i++;
	}
	return 1;
}
