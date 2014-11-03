#include "mytalk.h"

void user_interact(){
	char a[100], c;
	int i;
	struct termios term1, term2;

	// remember original terminal setting
	tcgetattr(0, &term1);
	term2 = term1;
	// set to noncanonical mode and disable automatic echoing, i.e.,
	// keyboard characters are immediately returned and not echo'ed 
	// to stdout
	term2.c_lflag &= ~(ICANON | ECHO); 
	tcsetattr(0, TCSANOW, &term2);

	// this code is for illustration purposes and must be adapted to
	// to fit one's app code
	i = 0;
	while(1) {				// user types chat message
		c = getchar();
		a[i] = c;			// store typed characters in a[];
		if ((c == 127) || (c == 8)) {	// if typed character is DEL or BS
			i = i - 1;			// then emulate backspace and print
			a[i] = '\0';			// corrected input on a new line
			fprintf(stdout,"\n%s",a);
			continue;
		}
		else
			fputc(c, stdout);		// manual echo to stdout
		i++;
		// your code must add logic to transmit a message if user enters RET
		if(c=='\n'){
			if(peer_connected && i!=1 && a[0]!='d'){
				send_buffer(remote_ip, global_port, a, i, 0);
				alert_user(a);
			}else{
				//TODO PARSE USER INPUT
			}
		}
	}

	tcsetattr(0, TCSANOW, &term1);	// return terminal to default mode
}
