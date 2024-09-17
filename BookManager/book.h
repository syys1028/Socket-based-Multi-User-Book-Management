#define MAX 1000
#define DBX "book2_sy.dbx"

typedef struct tagBOOK {
	int bookNumber;
  	char name[MAX];
  	char author[MAX];
  	char date[MAX];
  	char review[MAX];
  	int price;
	char ownerID[MAX];
} BOOK;

int titleSort(char memberID[], int result);
int priceSort(char memberID[]);
int ownerSort();
int writeData(char memberID[], int result);
int updateData(char memberID[], int result);
int deleteData(char memberID[], int result);
int searchTitle(char memberID[], int result);
int searchAuthor(char memberID[]);
void selNumberB(int number, int result);
void selNumberM(int number, int result);
