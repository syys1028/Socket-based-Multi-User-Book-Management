#define MAX 1000
#define DAT "member.dbx"
typedef struct tagMEMBER{
	int primary_key;
	char id[MAX];
	char password[MAX];
	char name[MAX];
	char phone[MAX];
	char email[MAX];
	int birthday;
	char isAdmin;
} MEMBER;

int memberList();
int deleteMember();
int updateMember();
int addnewMember(char memberID[]);
int changeData(char memberID[]);
int login();
