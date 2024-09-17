#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void servers(int);
void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
     int sockfd, newsockfd, portno, clilen, pid;
     struct sockaddr_in serv_addr, cli_addr;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
         error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd,
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0)
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             servers(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     }
     return 0;
}

void servers(int sock){
   int n, num;
   char buffer[256];
   time_t t = time(NULL);
   struct tm tm = *localtime(&t);
   printf("소켓기반 Multi-user Book Managerment 서버가 시작되었습니다.\n");

   for(;;){
      printf("[%d시:%d분:%d초] %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, buffer);

      bzero(buffer,256);
      n = read(sock,buffer,255);
      printf("%s님이 ", buffer);

      bzero(buffer,256);
      n = read(sock,buffer,255);
      printf("%s 했습니다.", buffer);

      bzero(buffer,256);
      n = read(sock,buffer,255);
      if ( strncmp( buffer, "0", 1) == 0 ){
         n = write(sock,"<This connection is fininshing>",31);
         break;
      }
   }
}

