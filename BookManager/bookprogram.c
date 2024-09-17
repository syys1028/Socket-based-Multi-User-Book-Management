#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "book.h"

struct flock lock;			// 전역변수 선언
BOOK record, temp, copy[1000];

void printlist(){			// record list print
	printf("현재 정보는 ISBN:%d 제목:%s 작가:%s 출판일:%s 리뷰:%s 가격:%d 소유자id:%s\n", record.bookNumber, record.name, record.author, record.date, record.review, record.price, record.ownerID);
}

void printsort(int n, char memberID[]){	// copy list print, ID 일치
	for (int i=0; i<n; i++){
                if(copy[i].bookNumber != 0 && strcmp(copy[i].ownerID, memberID) == 0){
                        printf("ISBN:%d 제목:%s 작가:%s 출판일:%s 리뷰:%s 가격:%d 소유자id:%s\n", copy[i].bookNumber, copy[i].name, copy[i].author, copy[i].date, copy[i].review, copy[i].price, copy[i].ownerID);
                }
        }
}

void printsortAll(int n){		// all list print
	for (int i=0; i<n; i++){
                if(copy[i].bookNumber != 0){
                        printf("ISBN:%d 제목:%s 작가:%s 출판일:%s 리뷰:%s 가격:%d 소유자id:%s\n", copy[i].bookNumber, copy[i].name, copy[i].author, copy[i].date, copy[i].review, copy[i].price, copy[i].ownerID);
                }
        }
}
BOOK inputData(){			// 데이터 삽입
	printf("제목:");
        scanf("%s", record.name);
        printf("작가:");
        scanf("%s", record.author);
        printf("출판일(출판년월일):");
        scanf("%s", record.date);
        printf("리뷰(공백포함 최소 100자):");
        scanf(" %[^\n]", record.review);
        printf("가격:");
        scanf("%d", &record.price);
	return record;
}

int titleSort(char memberID[], int result){		// 제목 순 정렬
        int fd, n=0;
        if ((fd = open(DBX, O_RDWR)) == -1){
                perror(DBX);
                exit(2);
        }
	lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = sizeof(record);
        lock.l_len = sizeof(record);
        fcntl(fd, F_SETLKW, &lock);
        for (int i = 1; ; i++){
                if (read(fd, (char*) &record, sizeof(record)) != sizeof(record)) break;
                copy[i-1] = record;
                n += 1;
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                lseek(fd, i*sizeof(record), SEEK_SET);
        }
        for(int i = 0; i<n-1; i++){
                for (int j = 0; j<n-1-i; j++){
                        if(strcmp(copy[j].name, copy[j+1].name) >0){
                                temp = copy[j];
                                copy[j] = copy[j+1];
                                copy[j+1] = temp;
        }       }       }
	if (result == 0){
	        printsort(n, memberID);
	}
	if (result == 1){
		printsortAll(n);
	}
        close(fd);
}

int priceSort(char memberID[]){             // 가격순 정렬
        int fd, n=0;
        if ((fd = open(DBX, O_RDWR)) == -1){
                perror(DBX);
                exit(2);
        }
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = sizeof(record);
        lock.l_len = sizeof(record);
        fcntl(fd, F_SETLKW, &lock);
        for (int i = 1; ; i++){
                if (read(fd, (char*) &record, sizeof(record)) != sizeof(record)) break;
                copy[i-1] = record;
                n += 1;
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                lseek(fd, i*sizeof(record), SEEK_SET);
        }
        for(int i = 0; i<n-1; i++){
                for (int j = 0; j<n-1-i; j++){
                        if(copy[j].price > copy[j+1].price){
                                temp = copy[j];
                                copy[j] = copy[j+1];
                                copy[j+1] = temp;
        }       }       }
        printsort(n, memberID);
        close(fd);
}

int ownerSort(){             // 도서입력자 정렬
        int fd, n=0;
        if ((fd = open(DBX, O_RDWR)) == -1){
                perror(DBX);
                exit(2);
        }
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = sizeof(record);
        lock.l_len = sizeof(record);
        fcntl(fd, F_SETLKW, &lock);
        for (int i = 1; ; i++){
                if (read(fd, (char*) &record, sizeof(record)) != sizeof(record)) break;
                copy[i-1] = record;
                n += 1;
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                lseek(fd, i*sizeof(record), SEEK_SET);
        }
        for(int i = 0; i<n-1; i++){
                for (int j = 0; j<n-1-i; j++){
                        if(strcmp(copy[j].ownerID, copy[j+1].ownerID) >0){
                                temp = copy[j];
                                copy[j] = copy[j+1];
                                copy[j+1] = temp;
        }       }       }
        printsortAll(n);
        close(fd);
}

int writeData(char memberID[], int result){		// 데이터 추가
        int fd, id, flag = 0;
	char c;
        if ((fd = open(DBX, O_RDWR|O_APPEND|O_CREAT, 0640)) == -1) {
                perror(DBX);
                exit(2);
        }
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = sizeof(record);
        lock.l_len = sizeof(record);
        fcntl(fd, F_SETLKW, &lock);
	do{
		printf("ISBN:");
		scanf("%d", &id);
		for ( int i = 1; ; i++){
		        int ret = read(fd, (char*) &record, sizeof(record));
        		if (ret != sizeof(record)) break;
			if (record.bookNumber == id){
				printf("해당 아이디가 존재합니다.\n");
				flag = 1;
				break;
			}
			lseek(fd, i*sizeof(record), SEEK_SET);
		}
		if (flag == 0){
			if (result == 0){
			        record.bookNumber = id;
		        	inputData();
				strcpy(record.ownerID, memberID);
		        	write(fd, (char*) &record, sizeof(record));
		        	lock.l_type = F_UNLCK;
        			fcntl(fd, F_SETLKW, &lock);
	        	}
			if (result == 1){
				record.bookNumber = id;
                                inputData();
                                printf("(관리자 권한) 도서 소유자:");
				scanf("%s", record.ownerID);
                                write(fd, (char*) &record, sizeof(record));
                                lock.l_type = F_UNLCK;
                                fcntl(fd, F_SETLKW, &lock);
			}
		}
		printf("더 추가하시겠습니까?(y/n) ");
		scanf(" %c", &c);
	}while(c == 'y');
	close(fd);
}


int updateData(char memberID[], int result) {		// 데이터 수정
        int fd, id;
        if ((fd = open(DBX, O_RDWR)) == -1) {
                perror(DBX);
                exit(2);        }
	printf("수정할 책의 ISBN 넘버 입력: ");
        if (scanf("%d", &id) == 1) {
                for (int i = 1; ; i++){
	        	lock.l_type = F_WRLCK;
        		lock.l_whence = SEEK_SET;
        		lock.l_start = sizeof(record);
        		lock.l_len = sizeof(record);
        		fcntl(fd, F_SETLKW, &lock);
                        int ret = read(fd, (char *)&record, sizeof(record));
                        if (ret != sizeof(record)) break;
                        if ((ret > 0) && (record.bookNumber > 0)) {
                                if (id == record.bookNumber){
					printlist();
					if (result == 0){
						if (strcmp(record.ownerID, memberID)==0){
	                                        	inputData();
						}
					}
					if (result == 1){
						inputData();
						printf("(관리자 권한) 도서 소유자:");
                                		scanf("%s", record.ownerID);
					}
                                        lseek(fd, -sizeof(record), SEEK_CUR);
                                        write(fd, (char *)&record, sizeof(record));
					break;
                                }
                        }
	                lseek(fd, i * sizeof(record), SEEK_SET);
        	        lock.l_type = F_UNLCK;
                	fcntl(fd, F_SETLK, &lock);
                }
        }close(fd);
}

int deleteData(char memberID[], int result) {		// 데이터 삭제
        int fd, id;
        BOOK deletedata = {0, 0, 0, 0, 0, 0};
        if ((fd = open(DBX, O_RDWR)) == -1) {
                perror(DBX);
                exit(2);
        }
	lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = sizeof(record);
        lock.l_len = sizeof(record);
        fcntl(fd, F_SETLKW, &lock);
        printf("삭제할 책의 ISBN 넘버 입력: ");
        if (scanf("%d", &id) == 1) {
                for (int i = 1; ; i++){
                        int ret = read(fd, (char *)&record, sizeof(record));
                        if (ret != sizeof(record)) break;
                        if ((ret > 0) && (record.bookNumber > 0)) {
                                if (id == record.bookNumber){
					printlist();
					if (result == 0){
						if (strcmp(record.ownerID, memberID)==0){
							record = deletedata;
                                        		printf("삭제되었습니다.\n");
						}
						else printf("본인 소유가 아닙니다.\n");
					}
					if (result == 1){
						record = deletedata;
						printf("삭제되었습니다.\n");
					}
                                        lseek(fd, -sizeof(record), SEEK_CUR);
                                        write(fd, (char *)&record, sizeof(record));
                                }
                        }
                        lseek(fd, i * sizeof(record), SEEK_SET);
                        lock.l_type = F_UNLCK;
                        fcntl(fd, F_SETLK, &lock);
                }
        } else printf("번호 입력오류");
        close(fd);
}

int searchTitle(char memberID[], int result) {		// 제목 검색
        int fd, flag=0;
        char title[1000];
        if ((fd = open(DBX, O_RDONLY)) == -1) {
                perror(DBX);
                exit(2);
	}
	while (flag == 0){
	        printf("검색할 책의 제목 입력(최소 2자 이상): ");
        	scanf("%s", title);
		if (strlen(title) > 1){
			flag == 1;
			break;
		}
		else	printf("2글자 이상 입력해주세요.\n");
	}
	lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = sizeof(record);
        lock.l_len = sizeof(record);
        fcntl(fd, F_SETLKW, &lock);
        for (int i = 1; ; i++){
                int ret = read( fd, (char*) &record, sizeof(record) );
                if ( ret != sizeof( record )) break;
                if ((ret > 0) && (record.bookNumber != 0)){
                        if (strstr(record.name, title) != NULL){
				printf("검색된 책 정보\n");
                                if (result == 0){
                                        if (strcmp(record.ownerID, memberID)==0){
                                                printlist();
                                        }
                                        else printf("본인 소유가 아닙니다.\n");
                                }
                                if (result == 1){
                                        printlist();
                                }
                        }
                }
		lseek(fd, i*sizeof(record), SEEK_SET);
		lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
        }
        close(fd);
}

int searchAuthor(char memberID[]) {		// 저자명 검색
        int fd, flag=0;
	char name[1000];
        if ((fd = open(DBX, O_RDONLY)) == -1) {
                perror(DBX);
                exit(2);
        }
        printf("검색할 책의 제목 입력(최소 2자 이상): ");
        scanf("%s", name);
	lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = sizeof(record);
        lock.l_len = sizeof(record);
        fcntl(fd, F_SETLKW, &lock);
        for (int i = 1; ; i++){
                int ret = read( fd, (char*) &record, sizeof(record) );
                if ( ret != sizeof( record )) break;
                if ((ret > 0) && (record.bookNumber != 0)){
                        if (strstr(record.author, name) != NULL){
                                printf("검색된 책 정보\n");
                                if (strcmp(record.ownerID, memberID)==0){
                                        printlist();
                                }
                                else printf("본인 소유가 아닙니다.\n");
                        }
                }
                lseek(fd, i*sizeof(record), SEEK_SET);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
        }
        close(fd);
}
