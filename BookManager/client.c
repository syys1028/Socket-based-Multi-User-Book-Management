#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include "book.h"
#include "member.h"

struct flock lock;
MEMBER data;
char memberID[256]= " ";
void do_chat(int);
void error(char *msg){
	perror(msg);
	exit(0);
}

void menuPrintB() {
	printf("--------------------------\n");
	printf("Book Management\n");
	printf("<일반 회원 도서관리>\n");
	printf("1) 식별자 순 정렬\n");
	printf("2) 가격 순 정렬\n");
	printf("3) 새 책 정보 추가\n");
	printf("4) 책 정보 수정\n");
	printf("5) 책 정보 삭제\n");
	printf("6) 제목으로 검색\n");
        printf("7) 저자명으로 검색\n");
	printf("8) 개인정보 변경\n");
	printf("0) 종료\n");
	printf("--------------------------\n");
}

void menuPrintM() {
        printf("--------------------------\n");
	printf("Book Management\n");
	printf("<Admin 회원 도서 관리>\n");
        printf("1) 식별자 순 정렬\n");
        printf("2) 도서등록자 순 정렬\n");
        printf("3) 새 책 정보 추가\n");
        printf("4) 책 정보 수정\n");
        printf("5) 책 정보 삭제\n");
        printf("6) 제목으로 검색\n");
        printf("7) 회원 리스트 보기\n");
	printf("8) 신규 회원정보 삽입\n");
        printf("9) 회원 정보 삭제\n");
        printf("10) 회원정보 갱신\n");
	printf("11) 개인정보 변경\n");
        printf("0) 종료\n");
        printf("--------------------------\n");
}

void menuPrintS(){
	printf("--------------------------\n");
        printf("Book Management Service\n");
        printf("1) 로그인\n");
        printf("2) 신규 회원 가입\n");
        printf("0) 종료\n");
        printf("--------------------------\n");

}
void selNumberB(int number, int result){
	switch (number) {
	case 1:
		titleSort(memberID, result);
		break;
	case 2:
      		priceSort(memberID);
      		break;
    	case 3:
    		writeData(memberID, result);
      		break;
    	case 4:
      		updateData(memberID, result);
      		break;
    	case 5:
      		deleteData(memberID, result);
      		break;
    	case 6:
      		searchTitle(memberID, result);
      		break;
        case 7:
                searchAuthor(memberID);
                break;
	case 8:
		changeData(memberID);
		break;
	}
}

void selNumberM(int number, int result){
        switch (number) {
        case 1:
                titleSort(memberID, result);
                break;
        case 2:
                ownerSort();
                break;
        case 3:
                writeData(memberID, result);
                break;
        case 4:
                updateData(memberID, result);
                break;
        case 5:
                deleteData(memberID, result);
                break;
        case 6:
                searchTitle(memberID, result);
                break;
        case 7:
       		memberList();
                break;
	case 8:
		addnewMember(memberID);
		break;
        case 9:
                deleteMember();
                break;
        case 10:
                updateMember();
                break;
	case 11:
		changeData(memberID);
		break;
        }
}

int login(){
        int fd, n=0, result = -2;
        char pw[1000];
        printf("id : ");
        scanf("%s", memberID);
        printf("pass : ");
        scanf("%s", pw);
        if ((fd = open(DAT, O_RDWR)) == -1){
                perror(DAT);
                exit(2);
        }
        for (int i = 1; ; i++){
                int ret = read( fd, (char*) &data, sizeof(data) );
                if ( ret != sizeof( data )) break;
                if ((ret > 0) && (data.primary_key != 0)){
                        if (strcmp(data.id, memberID) == 0){
                                if (strcmp(data.password, pw) == 0){
                                        printf("로그인 성공\n");
                                        if (data.isAdmin == 'Y'){
                                                result = 1;
                                        }
                                        else result = 0;
                                }else result = -1;
                        }
                }
                lseek(fd, i*sizeof(data), SEEK_SET);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
        }
        return result;
}

void bookprogram(int result){
        int menuNum = -1;
        while (menuNum != 0) {
                menuPrintB();
                printf("숫자를 입력하세요 : ");
                scanf("%d", &menuNum);
                selNumberB(menuNum, result);
                if (menuNum == 0){
                        printf("종료합니다. \n");
                }
        }
}

void memberprogram(int result){
        int menuNum = -1;
        while (menuNum != 0) {
                menuPrintM();
                printf("숫자를 입력하세요 : ");
                scanf("%d", &menuNum);
                selNumberM(menuNum, result);
                if (menuNum == 0){
                        printf("종료합니다. \n");
                }
        }
}

void selNumberS(int number){
        int result = -3, count = 0, status1, status2;
	switch (number) {
        case 1:
		while(count < 3){
			result = login();
			if (result == 0){
				if (fork() == 0)
					bookprogram(result);
				break;
			}
			if (result == 1){
				if (fork() == 0)
		                	memberprogram(result);
	               		break;
			}
			if (result == -1){
				printf("비밀번호가 일치하지 않습니다.\n");
				count += 1;
			}
			if (result == -2){
				printf("아이디가 일치하지 않습니다.\n");
				count += 1;
			}
			printf("다시 입력해주세요.\n");
		} break;
        case 2:
                addnewMember(memberID);
                break;
        }
}

void chat(int sockfd){
	int n, menuNum = -1, selects = 0;
	char out_buffer[256];
	while (menuNum != 0) {
	        menuPrintS();
	        printf("숫자를 입력하세요 : ");
		scanf("%d", &menuNum);
	        selNumberS(menuNum);

		bzero(out_buffer,256);
                strcpy(out_buffer, memberID);
                n = write(sockfd,out_buffer,strlen(out_buffer));

		if (menuNum == 1){
	                n = write(sockfd, "login", 5);
		}
	        if (menuNum == 0){
			n = write(sockfd, "0", 1);
        	        printf("종료합니다. \n");
        	}
	}
}

int main(int argc, char *argv[]){
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	portno = atoi(argv[2]);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sockfd < 0)
        	error("ERROR opening socket");
    	server = gethostbyname(argv[1]);
    	if (server == NULL) {
        	fprintf(stderr,"ERROR, no such host\n");
        	exit(0);
    	}
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    	serv_addr.sin_port = htons(portno);
    	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    		error("ERROR connecting");

    	chat( sockfd );
    	return 0;
}
