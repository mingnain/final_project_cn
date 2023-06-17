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
#include <string>
#define BUFF_SIZE 200

#define PORT 1234
using namespace std;

char send_buf[BUFF_SIZE], recv_buf[BUFF_SIZE];

typedef struct{
    char id[100];
    char email[100];
}student;

int main()
{
    struct sockaddr_in myaddr;
    struct sockaddr_in client_addr;
    struct hostent *hent;
	int sockfd, streamfd, port, status;
	int addr_size;
	bzero(&myaddr, sizeof(myaddr));
	myaddr.sin_family = PF_INET;
	myaddr.sin_port = htons(PORT);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){
        printf("error in socketfd \n");
        exit(1);
    }else{
        printf("successfully in socketfd\n");
    }

    if(bind (sockfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) < 0){
        printf("binding error\n");
        exit(1);
    }else{
        printf("binding success\n");
    }
	listen(sockfd, 10);
	addr_size = sizeof (client_addr);

    printf("heh");


	vector<student> student_list;
    FILE *fp;
    if ((fp = fopen("query.txt", "r")) == NULL) {
        printf("File 'query.txt' doesn't found\n");
        exit(-1);
    }
    else {
        student st;
        while (fscanf(fp, "%s %s", st.id, st.email) != EOF) {
            student_list.push_back(st);
        }
    }

	while(1){
        printf("start connection\n");
	    streamfd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &addr_size);

        memset(send_buf, '\0', sizeof(send_buf));
        while (1) {
            memset(send_buf, '\0', sizeof(send_buf));
            strcpy(send_buf, "What's your requirement? 1.DNS 2.QUERY 3.QUIT : ");
            write(streamfd, send_buf, strlen(send_buf));

            memset(recv_buf, '\0', sizeof(recv_buf));
            read(streamfd, recv_buf, BUFF_SIZE);

            if (!strcmp(recv_buf, "1")) {
                memset(send_buf, '\0', sizeof(send_buf));
                strcpy(send_buf, "Input URL address : ");
                write(streamfd, send_buf, strlen(send_buf));
                memset(recv_buf, '\0', sizeof(recv_buf));
                read(streamfd, recv_buf, BUFF_SIZE);

                hent = gethostbyname(recv_buf);

                if (hent == NULL) {
                    printf("No such DNS\n");
                    memset(send_buf, '\0', sizeof(send_buf));
                    strcpy(send_buf, "No such DNS\n\n");
                    write(streamfd, send_buf, strlen(send_buf));
                }
                else {
                    printf("address get from the domain name: %s\n", inet_ntoa(*(struct in_addr*) hent->h_addr));
                    memset(send_buf, '\0', sizeof(send_buf));
                    strcpy(send_buf, "address get from the domain name: ");
                    strcat(send_buf, inet_ntoa(*(struct in_addr*) hent->h_addr));
                    strcat(send_buf, "\n\n");
                    write(streamfd, send_buf, strlen(send_buf));
                }

            }else if (!strcmp(recv_buf, "2")) {
                    memset(send_buf, '\0', sizeof(send_buf));
                    strcpy(send_buf, "Input student ID : ");
                    write(streamfd, send_buf, strlen(send_buf));
                    memset(recv_buf, '\0', sizeof(recv_buf));
                    read(streamfd, recv_buf, BUFF_SIZE);
                    int i;
                    for (i = 0; i < student_list.size(); i++) {
                        if (!strcmp(student_list[i].id, recv_buf)) {
                            memset(send_buf, '\0', sizeof(send_buf));
                            strcpy(send_buf, "Email get from the server : ");
                            strcat(send_buf, student_list[i].email);
                            strcat(send_buf, "\n\n");
                            write(streamfd, send_buf, strlen(send_buf));
                            printf("%s\n", student_list[i].email);
                            break;
                        }
                    }
                    if(i == student_list.size()){
                        printf("No such student ID\n");
                        memset(send_buf, '\0', sizeof(send_buf));
                        strcpy(send_buf, "Email get from the server : No such student ID\n\n");
                        write(streamfd, send_buf, strlen(send_buf));
                    }

            }else if (!strcmp(recv_buf, "3")) {
                break;
            }else {
                memset(send_buf, '\0', sizeof(send_buf));
                strcpy(send_buf, "Invalid requirement!\n The requirement should between 1-3\n\n");
                write(streamfd, send_buf, strlen(send_buf));
            }

        }
	    close(streamfd);

    }
    return 0;
}
