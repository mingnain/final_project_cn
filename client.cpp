#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>

#define BUFF_SIZE 200

#define PORT 1234
using namespace std;


int main() {


    struct sockaddr_in server_addr;
	int sockfd, status;
	//setup the server address
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr ("127.0.0.1");
	//connect to the server
	sockfd = socket (PF_INET, SOCK_STREAM, 0);
	connect (sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in));

	char send_buf[BUFF_SIZE], recv_buf[BUFF_SIZE];
	while(1){
        memset(recv_buf, '\0', sizeof(recv_buf));
        read(sockfd, recv_buf, BUFF_SIZE);

        printf("%s", recv_buf);
        memset(send_buf, '\0', sizeof(send_buf));
        scanf("%s", send_buf);
        write(sockfd, send_buf, BUFF_SIZE);

        if (!strcmp(send_buf, "3")) {
            break;
        }
	}


    close(sockfd);
	return 0;
}
