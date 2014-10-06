#include "reminder.h"

int secret_matches(int str_size, char* payload){
	printf("  --Secret Comparison--\n    server(%d): %s\n    client(%d): %.*s\n", secret_len, secret, str_size, str_size, payload);
	if(str_size == secret_len)
		return strncmp(secret, payload, str_size) == 0 ? 1 : 0;
	else
		return strcmp(secret, payload) == 0 ? 1 : 0;
}
