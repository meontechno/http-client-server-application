/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/stat.h>

#define MAXLINE 5000

void *cliSvr(void *arg)
{
    int   n, sockfd;
    char  buffer[MAXLINE], buffer2[MAXLINE];
    char rmsg[100], *ROOT, *FPATH; 
    char *method, seperator;    
    char* c_time_string;  
    char http_not_found[] = "HTTP/1.0 404 Not Found\n";
    char http_ok[] = "HTTP/1.0 200 OK";
    char send_head[500];  
    char hostname[1024];
    gethostname(hostname, 1024);
    //printf("server name is %s\n",hostname );

    time_t current_time;
    current_time = time(NULL);   
    
    FILE * fileptr; 
    
    //clearing the buffer before getting the client's message
    bzero(buffer,256);
    sockfd = *(int *)arg;

    //read the message form the socketfd to the buffer
    n = read(sockfd,buffer,sizeof(buffer));        
    if (n < 0) {
        fprintf(stderr, "Error reading from socket, errno = %d (%s)\n",
                errno, strerror(errno));
        close(sockfd);
        return NULL;
    }
    //root address is stored in ROOT
    ROOT = getenv("PWD");
    //printf("root is :%s\n", ROOT);
    printf("Incoming request:\n%s\n",buffer);

    //below block executes if the incoming request method is GET
    if((strncmp(buffer, "GET /", 5)) == 0) {
        
        char *ret = strstr(buffer, "/");
        char *ret2 = strchr(ret, ' ');
        int lindex = (int)(ret2 - ret);

        memset(rmsg, '\0', sizeof(rmsg));
        strncpy(rmsg, ret, lindex);
        FPATH = malloc(strlen(ROOT)+strlen(rmsg)+1);
        strcpy(FPATH, ROOT);
        strcat(FPATH, rmsg);        
        sprintf(send_head,"%s\n\r\nDate: %s\r\nServer: %s\n\r\nLast-Modified: Wed, 27 Feb 2017 13:15:56 GMT\n\r\nContent-Type: text/html\n\r\nConnection: Closed\n\r\n",http_ok, ctime(&current_time),hostname);

    }
    //below block executes if the incoming request method is POST
    else if((strncmp(buffer, "POST", 4)) == 0) {
        char *ret = strrchr(buffer, '/');        
        char *ret2 = strrchr(ret, ' ');
        int lindex = (int)(ret2 - ret);        
        memset(rmsg, '\0', sizeof(rmsg));
        strncpy(rmsg, ret, lindex);        
        FPATH = malloc(strlen(ROOT)+strlen(rmsg)+1);
        strcpy(FPATH, ROOT);
        strcat(FPATH, rmsg);
        sprintf(send_head,"%s\n\r\nDate: %s\r\nServer: %s\n\r\nLast-Modified: Wed, 27 Feb 2017 13:15:56 GMT\n\r\nContent-Type: text/html\n\r\nConnection: Closed\r\n",http_ok, ctime(&current_time),hostname);
    }
     //below block executes if the incoming request method is HEAD
    else if((strncmp(buffer, "HEAD /", 6)) == 0) {
        char *ret = strchr(buffer, '/');        
        char *ret2 = strchr(ret, ' ');
        int lindex = (int)(ret2 - ret);        
        memset(rmsg, '\0', sizeof(rmsg));
        strncpy(rmsg, ret, lindex);        
        FPATH = malloc(strlen(ROOT)+strlen(rmsg)+1);
        strcpy(FPATH, ROOT);
        strcat(FPATH, rmsg);
        sprintf(send_head,"%s\n\r\nDate: %s\r\nServer: %s\n\r\nLast-Modified: Wed, 27 Feb 2017 13:15:56 GMT\n\r\nContent-Type: text/html\n\r\nConnection: Closed\n\r\n",http_ok, ctime(&current_time),hostname);
                
    }

    else{
        fprintf(stderr, "Invalid method type, errno = %d (%s)\n",
                errno, strerror(errno));
        close(sockfd);
        return NULL;
    }


    printf("Server:: Requested file : %s\n",FPATH);

     if ((fileptr = fopen(FPATH, "r")) == NULL ) {
        printf("File not found!\n");
        send(sockfd, http_not_found, strlen(http_not_found), 0); //sends HTTP 404
    }
    else {  
            /* Send response back to the client */
            if((strncmp(buffer, "HEAD /", 6)) == 0) {

                strcpy(buffer2, send_head);
                send(sockfd, buffer2, sizeof(buffer2), 0);
                close(sockfd);
                return NULL;
            }
            else {
                printf("Sending the file...\n");
        
                memset(&buffer, 0, sizeof(buffer));
        
                while (!feof(fileptr)) { //sends the file
                    fread(&buffer, sizeof(buffer), 1, fileptr);
                    strcpy(buffer2, send_head);
                    strcat(buffer2, buffer);
                    send(sockfd, buffer2, sizeof(buffer2), 0);
                    printf("File successfully sent\n");
                    memset(&buffer2, 0, sizeof(buffer2));
                    memset(&buffer, 0, sizeof(buffer));
                    close(sockfd);
                    return NULL;
                }
            }
        
    }     

    close(sockfd);
 
    return NULL;

}


int main(int argc, char *argv[])
{
     int sockfd, clisockfd, port, clilen;
      

     //structure to store server address and client address
     struct sockaddr_in serv_addr, cli_addr; 

     pthread_t  tid;

     //program exits if the arguments are less than than 2
     if (argc < 2) {
         fprintf(stderr,"Usage: %s <port>\n", argv[0]);
         exit(1);
     }

     /* Open a TCP socket connection 
      AF_INET is to implement IP concept
      SOCK_STREAM is to implement TCP concept
      Third argument is 0 to let the system decide the socket 
      based on the first two arguments */
     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     //sockfd is less than 0 if there is any error creating socket
     if (sockfd < 0) { 
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n", 
                errno, strerror(errno));
        return -1;
     }

     //bzero is to clear the serv_addr (server addresses)
     bzero((char *) &serv_addr, sizeof(serv_addr));

     //assigning the port number to port variable
     port = atoi(argv[1]);

     //tells the server to use IP format address
     serv_addr.sin_family = AF_INET;
     //tells the system to get the address by its own(address of the PC)
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     //assigning the port number to the server by using htons() which converts integer port number to network format
     serv_addr.sin_port = htons(port);

     //to bind the server address structure with the socket
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
         fprintf(stderr, "Error bind to socket, erron = %d (%s) \n",
                 errno, strerror(errno));
         return -1;
     }

     //listen to the socket and support max 5 clients at a time
     listen(sockfd, 5);

     /* Wait for incoming socket connection requests */
     while (1) {
         //get the size of the client address
         clilen = sizeof(cli_addr);
         //accept the connection using the sockfd, client address and length
         clisockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

         // clisockfd is less than 0 if there is an error in accepting the connection
         if (clisockfd < 0) {
             fprintf(stderr, "Error accepting socket connection request, errno = %d (%s) \n",
                     errno, strerror(errno));
             break;
         }
      
         /* Create thread for client requests/responses */ 
         pthread_create(&tid, NULL, (void *)&cliSvr, (void *)&clisockfd);
     }

     close(sockfd);

     return 0; 
}
