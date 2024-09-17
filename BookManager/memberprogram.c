#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "book.h"
#include "member.h"

struct flock lock;			// 전역변수 선언
MEMBER data;

void printMemberList(){
	printf("현재 정보는 key:%d id:%s pw:%s 이름:%s 휴대폰번호:%s 이메일:%s 생년월일:%d 관리자 여부:%c\n", data.primary_key, data.id, data.password, data.name, data.phone, data.email, data.birthday, data.isAdmin);
}

MEMBER inputMember(){
	printf("password:");
        scanf("%s", data.password);
        printf("회원 이름:");
        scanf("%s", data.name);
        printf("휴대폰 번호:");
        scanf("%s", data.phone);
        printf("이메일:");
        scanf("%s", data.email);
	printf("생년월일(YYMMDD):");
        scanf("%d", &data.birthday);
        printf("Admin 여부(Y/N):");
        scanf(" %c", &data.isAdmin);
        return data;
}

int addnewMember(char memberID[]){
	int fd, key, n = 0, flag = 0, trap = 0, acount = 0;
        char c, ids[1000], keys[100] = {0,};
        if ((fd = open(DAT, O_RDWR|O_APPEND|O_CREAT, 0640)) == -1) {
                perror(DAT);
                exit(2);
        }
        printf("id:");
        scanf("%s", ids);
        for ( int i = 1; ; i++){
		lock.l_type = F_WRLCK;
        	lock.l_whence = SEEK_SET;
        	lock.l_start = sizeof(data);
	        lock.l_len = sizeof(data);
	        fcntl(fd, F_SETLKW, &lock);
		int ret = read(fd, (char*) &data, sizeof(data));
                if (ret != sizeof(data)) break;
		if ((ret > 0) && (data.primary_key > 0)){
			if (strcmp(memberID, data.id) == 0){
				if (data.isAdmin == 'Y'){
					acount = 1;
				} else acount = 0;
			}
	                if (strcmp(ids, data.id) == 0){
        	                printf("해당 아이디가 존재합니다.\n");
                	        flag = 1;
                	}
			keys[i-1] = data.primary_key;
		}
		lseek(fd, i*sizeof(data), SEEK_SET);
        }
	srand(time(NULL));
	while(trap != 1){
		n = rand() % 90 + 10;
		for (int i = 0; i<100; i++){
			if (keys[i] == n){
				break;
			}
			else trap = 1;
		}
	}
        if (flag == 0){
		if (acount == 0){
			data.primary_key = n;
        	        strcpy(data.id,ids);
                	inputMember();
		}
		if (acount == 1){
                        strcpy(data.id,ids);
                        inputMember();
			printf("(관리자 권한) 회원번호:");
                        scanf("%d", &key);
			for (int i = 0; i<100; i++){
				if (keys[i] == key){
					printf("이미 있는 회원번호입니다. 자동 입력됩니다.\n");
					data.primary_key = n;
					break;
				}
			}
			if (data.primary_key != n){
				data.primary_key = key;
			}
		}
                write(fd, (char*) &data, sizeof(data));
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLKW, &lock);
		close(fd);
	}
}

int memberList(){            // 회원 정보 보기
        int fd, n=0;
        if ((fd = open(DAT, O_RDWR)) == -1){
                perror(DAT);
                exit(2);
        }
        for (int i = 1; ; i++){
        	int ret = read( fd, (char*) &data, sizeof(data) );
                if ( ret != sizeof( data )) break;
                if ((ret > 0) && (data.primary_key != 0)){
                        printMemberList();
                }
                lseek(fd, i*sizeof(data), SEEK_SET);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
        }
        close(fd);
}

int deleteMember() {           // 데이터 삭제
        int fd;
	char ids[1000];
        MEMBER deletedata = {0, 0, 0, 0, 0, 0, 0, 0};
        if ((fd = open(DAT, O_RDWR)) == -1) {
                perror(DAT);
                exit(2);
        }
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = sizeof(data);
        lock.l_len = sizeof(data);
        fcntl(fd, F_SETLKW, &lock);
        printf("삭제할 회원의 아이디 입력: ");
        scanf("%s", ids);
        for (int i = 1; ; i++){
                int ret = read(fd, (char *)&data, sizeof(data));
                if (ret != sizeof(data)) break;
                if ((ret > 0) && (data.primary_key > 0)) {
                       if (strcmp(data.id, ids) == 0){
                                printMemberList();
                                data = deletedata;
                                printf("삭제되었습니다.\n");
                                lseek(fd, -sizeof(data), SEEK_CUR);
                                write(fd, (char *)&data, sizeof(data));
                        }
                }
                lseek(fd, i * sizeof(data), SEEK_SET);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
        }
        close(fd);
}


int updateMember() {           // 회원정보 수정
        int fd;
	char ids[1000], check;
        if ((fd = open(DAT, O_RDWR)) == -1) {
                perror(DAT);
                exit(2);        }
        printf("수정할 멤버의 아이디 입력: ");
        scanf("%s", ids);
        for (int i = 1; ; i++){
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = sizeof(data);
                lock.l_len = sizeof(data);
                fcntl(fd, F_SETLKW, &lock);
                int ret = read(fd, (char *)&data, sizeof(data));
                if (ret != sizeof(data)) break;
                if ((ret > 0) && (data.primary_key > 0)) {
                        if (strcmp(ids, data.id) == 0){
                                printMemberList();
				check = data.isAdmin;
                                inputMember();
				if (check == 'Y'){
                                        printf("(관리자 권한) 회원번호:");
                                        scanf("%d", &data.primary_key);
                                } else data.isAdmin = check;
				printf("수정되었습니다.\n");
                                lseek(fd, -sizeof(data), SEEK_CUR);
                                write(fd, (char *)&data, sizeof(data));
                                break;
                        }
                }
                lseek(fd, i * sizeof(data), SEEK_SET);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
        }close(fd);
}

int changeData(char memberID[]){		// 개인정보 변경
        int fd;
        char ids[1000], check;
        if ((fd = open(DAT, O_RDWR)) == -1) {
                perror(DAT);
                exit(2);        }
        for (int i = 1; ; i++){
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = sizeof(data);
                lock.l_len = sizeof(data);
                fcntl(fd, F_SETLKW, &lock);
                int ret = read(fd, (char *)&data, sizeof(data));
                if (ret != sizeof(data)) break;
                if ((ret > 0) && (data.primary_key > 0)) {
                        if (strcmp(memberID, data.id) == 0){
                                printMemberList();
				check = data.isAdmin;
                                inputMember();
				if (check == 'Y'){
					printf("(관리자 권한) 회원번호:");
					scanf("%d", &data.primary_key);
				} else data.isAdmin = check;
                                printf("수정되었습니다.\n");
                                lseek(fd, -sizeof(data), SEEK_CUR);
                                write(fd, (char *)&data, sizeof(data));
                                break;
                        }
                }
                lseek(fd, i * sizeof(data), SEEK_SET);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
        }close(fd);
}
