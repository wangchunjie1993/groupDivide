#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LINE_LENGTH 1024

struct Student{
	char ID[32];
	char name[32];
	char major[128];
	char institute[128];
	struct Student* next;
};

struct Institute{
	char name[128];
	struct Student* sHeader;
	struct Student* printIndex;
	struct Institute* next;
};

struct Group{
	int num;
	struct Student* ps;
};



int insertStudent(struct Institute* header,struct Student* newStudent);
int divideIntoGroups(struct Institute* header,struct Group* groupHeader,int groupNum,int studentNum);
void printGroups(struct Group* groupHeader,int groupNum);
void freeGroups(struct Group* groupHeader,int groupNum);
void freeInstitute(struct Institute* header);

int main(int argc,char **argv){
	FILE *fp;
	char line[LINE_LENGTH];
	struct Institute* header;
	struct Student* newStudent;
	struct Group* groupHeader;
	int studentNum=0;
	int groupNum = atoi(argv[2]);
	int i;

	if(argc!=3){
		fprintf(stderr,"Must be 3 args!\n");
		exit(0);
	}
	
	if((fp = fopen(argv[1],"r"))==NULL){
		fprintf(stderr,"Open file %s failed!\n",argv[1]);
		exit(0);
	}

	header = (struct Institute*)malloc(sizeof(struct Institute));
	if(header==NULL){
		fprintf(stderr,"Malloc new institute failed!\n");
		exit(0);
	}
	header->next = NULL;
	fgets(line,LINE_LENGTH,fp);
	while(!feof(fp)){
		memset(line,0,LINE_LENGTH);
		fgets(line,LINE_LENGTH,fp);
		if(line[0]==0){
			continue;
		}
		studentNum++;
//		printf("line : %s",line);
		newStudent = (struct Student*)malloc(sizeof(struct Student));
		if(newStudent==NULL){
			fprintf(stderr,"Malloc new student failed!\n");
			exit(0);
		}
		sscanf(line,"%*[^,],%[^,],%[^,],%[^,],%[^,]",newStudent->ID,newStudent->name,newStudent->major,newStudent->institute);
		newStudent->next = NULL;

		insertStudent(header,newStudent);
	}

	groupHeader = (struct Group*)malloc(groupNum*sizeof(struct Group));
	if(groupHeader==NULL){
		fprintf(stderr,"Malloc group failed!\n");
		exit(0);
	}
	for(i=0;i<groupNum;i++){
		groupHeader[i].num = 0;
		groupHeader[i].ps = NULL;
	}
	divideIntoGroups(header,groupHeader,groupNum,studentNum);
	printGroups(groupHeader,groupNum);
	freeGroups(groupHeader,groupNum);
	freeInstitute(header);
	return 0;
}


int insertStudent(struct Institute* header,struct Student* newStudent){
	struct Institute* ip = header,*newInstitute;
	struct Student* sp;

	while(ip->next!=NULL){
		if(strcmp(ip->next->name,newStudent->institute)==0){
			newStudent->next = ip->next->sHeader;
			ip->next->sHeader = newStudent;
			ip->next->printIndex = newStudent;
			return 1;
	/*		if(ip->next->sHeader==NULL){
				ip->next->sHeader = newStudent;
				ip->next->printIndex = newStudent;
				return 1;
			}else{
				newStudent->next = ip->next->sHeader;
				ip->next->sHeader = newStudent;
				ip->next->printIndex = newStudent;
				return 1;
			}*/
		}
		ip = ip->next;
	}
	
	newInstitute = (struct Institute*)malloc(sizeof(struct Institute));
	if(!newInstitute){
		fprintf(stderr,"Malloc new institute failed!\n");
		exit(0);
	}
	strcpy(newInstitute->name,newStudent->institute);
	newInstitute->next = NULL;
	newInstitute->printIndex = newStudent;
	newInstitute->sHeader = newStudent;
	ip->next = newInstitute;

	return 1;
}


int divideIntoGroups(struct Institute* header,struct Group* groupHeader,int groupNum,int studentNum){
	int eachGroupNum = (studentNum+groupNum-1)/groupNum;
	struct Institute* ip = header->next;
	struct Student* ps;
	int groupIndex = 0;

	while(ip!=NULL){
		ps = ip->sHeader;
		if(ps!=NULL){
			ip->sHeader = ps->next;
			ps->next = NULL;

			while(groupHeader[groupIndex].num>=eachGroupNum){
				groupIndex = (groupIndex+1)%groupNum;
			}

			if(groupHeader[groupIndex].ps==NULL){
				groupHeader[groupIndex].ps = ps;
				groupHeader[groupIndex].num++;
			}else if(groupHeader[groupIndex].num<eachGroupNum){
				ps->next = groupHeader[groupIndex].ps;
				groupHeader[groupIndex].ps = ps;
				groupHeader[groupIndex].num++;
			}
			groupIndex = (groupIndex+1)%groupNum;
		}else{
			ip = ip->next;
		}
	}
}


void printGroups(struct Group* groupHeader,int groupNum){
	struct Student* ps;
	int i;
	for(i=0;i<groupNum;i++){
		printf("\n*******************************************\nGroup %d:\n",i+1);
		ps = groupHeader[i].ps;
		while(ps!=NULL){
			printf("%-16s, %-10s, %-24s, %-24s\n",ps->ID,ps->name,ps->major,ps->institute);
			ps = ps->next;
		}
		printf("\n");
	}
}

void freeGroups(struct Group* groupHeader,int groupNum){
	int i;
	struct Student* ps,*q;
	for(i=0;i<groupNum;i++){
		ps = groupHeader[i].ps;
		while(ps!=NULL){
			q = ps;
			ps = ps->next;
			free(q);
		}
	}
	free(groupHeader);
}

void freeInstitute(struct Institute* header){
	struct Institute* ip = header->next;
	while(ip!=NULL){
		header->next = ip->next;
		free(ip);
		ip = header->next;
	}
	free(header);
}

//int windInstitute(struct Institute* header){
//	struct Institute* ip = header;
//	while(ip->next!=NULL){
//		ip = ip->next;
//	}
//	ip->next = header->next;
//	return 1;
//}

//void printGroups(struct Institute* header,int studentNum,int groupNum){
//	int eachGroupNum = (studentNum+groupNum-1)/groupNum;
//	int lastGroupNum = studentNum - eachGroupNum*(groupNum-1);
//	struct Institute* ip = header->next;
//	int i,j;
//	for(i=0;i<groupNum-1;i++){
//		printf("\n*******************************************\nGroup %d:\n",i+1);
//		for(j=0;j<eachGroupNum;){
//			if(ip->printIndex!=NULL){
//				printf("%s,%s,%s,%s\n",ip->printIndex->ID,ip->printIndex->name,ip->printIndex->major,ip->printIndex->institute);
//				ip->printIndex = ip->printIndex->next;
//				j++;
//			}
//			ip = ip->next;
//		}
//		printf("\n");
//	}
//	printf("\n*******************************************\nGroup %d:\n",i+1);
//	for(j=0;j<lastGroupNum;){
//		if(ip->printIndex!=NULL){
//			printf("%s,%s,%s,%s\n",ip->printIndex->ID,ip->printIndex->name,ip->printIndex->major,ip->printIndex->institute);
//			ip->printIndex = ip->printIndex->next;
//			j++;
//		}
//		ip = ip->next;
//	}
//	printf("\n");
//}

