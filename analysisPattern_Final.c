#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void CutData(char* path,int temp2);
void Stringize(int temp);
void print();
char comp_Str[101];

char** bwStr;
char* LCS(char* a, char* b);
int day_price[101];
int day_volume[101];
char*** Pattern;

#define NEITHER       0
#define UP            1
#define LEFT          2
#define UP_AND_LEFT   3

typedef struct{
	int company;
	int date;
	int time;
	int price;
	int volume;
	int x;
}rdata;



void main(){
	int i;
	int temp=0;
	int temp2;
	FILE *fp2;
	char codeline[10];
	char** codelist;
	char*** fileName;
	char** codelist_temp;
	char temp2_char[6];
	char path[40];
	char** bestPattern;
	char** worstPattern;
	char* lcs_temp;
	


	fp2=fopen("목록_Sample.txt","r");

	//회사 목록들어있는 파일 읽어서
	//한 줄씩 codelist에 넣음.

	if(fp2==NULL){
		printf("There are no list of File\n");
		exit(0);
	}

	codelist=(char**)malloc(sizeof(char*)*15);
	while(!feof(fp2)){

		fgets(codeline,10,fp2);
		codeline[strlen(codeline)-1]='\0'; //마지막 공백 없애주기
		codelist[temp]=(char*)malloc(sizeof(char)*strlen(codeline)+1);
		strcpy(codelist[temp],codeline);
		temp++;
	}
	fclose(fp2);

	// code_0000숫자.txt
	//////////////////////////////////////////////////////////////////////
	codelist_temp=(char**)malloc(sizeof(char*)*15+1);
	fileName=(char***)malloc(sizeof(char**)*15+1);
	bwStr=(char**)malloc(sizeof(char*)*2);
	/////////////////////////////////////////////////////////////////////

	for(temp=0;temp<15;temp++){ //code

		codelist_temp[temp]=(char*)malloc(strlen(codelist[temp])+8);
		fileName[temp]=(char**)malloc(sizeof(char*)*101);

		for(temp2=0;temp2<=100;temp2++){ //한 폴더 안에 파일 101개씩 들어있음
			strcpy(codelist_temp[temp],codelist[temp]); //임시로 저장

			sprintf(temp2_char,"%05d",temp2); //00001~00100

			strcat(codelist_temp[temp],"_");//006910_
			strcat(codelist_temp[temp],temp2_char);//006910_00001

			fileName[temp][temp2]=(char*)malloc(strlen(codelist_temp[temp])+1);

			strcpy(fileName[temp][temp2],codelist_temp[temp]);

			strcpy(path,"RawData_Sample\\");
			strcat(path,codelist[temp]);
			strcat(path,"\\");
			strcat(path,fileName[temp][temp2]);
			strcat(path,".txt");
			//파일 읽고 자르고 패턴 분석까지.

			if(strcmp(path,"RawData_Sample\\046940\\046940_00100.txt")!=0){  //046940파일만 _99까지 있음
				CutData(path,temp2); //day_price[],day_volume[] 도출 ->한 회사의 하루 평균 거래량, 가격 ->101번 다 돌면 배열에 그 회사의 100일 간의 평균 값 저장
				
			//	printf("%d / ",day_price[temp]);
			}

		}
		//CutData를  통해 day_price랑 day_volume 배열 다 채웠고, 그걸 이용해서 한 회사를 stringize..패턴찾음..
		printf("%d번째 회사: \n",temp+1);
		Stringize(temp); //회사 당 string화..급증 급감 구역 string 잘라서 lcs시켜줘야..어디서할까.->함수 안에서!
/*		lcs_temp=(char*)malloc(sizeof(char)*21+1); 
		strcpy(lcs_temp,bestPattern[0]);  //초기화
		for(i=0;i<101;i++){
			lcs_temp=LCS(lcs_temp,bestPattern[i]);
			printf("%s\n",lcs_temp);
		}
		//여기에서 lcs실행해야..
		
*/
	}
	free(fileName);
	free(codelist_temp);

	printf("진짜 되는거임???\n");
}

////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void CutData(char* path,int temp2){	
	rdata** data;
	int j=0;
	int w[6];
	FILE *fp;
	char line_p[70];
	char seps[]="/";
	char *token;

	int total_price=0;
	int total_volume=0;

	

	//	printf("%s",path);


	fp=fopen(path,"r");
	//파일 없으면 그냥 종료

	if(fp==NULL){
		printf("There are no File\n");
		exit(0); 
	}
	data=(rdata**)malloc(sizeof(rdata*)*100000);
	//파일 있을 때

	//파일 끝날때까지
	while(!feof(fp)){

		data[j]=(rdata*)malloc(sizeof(rdata));

		//한 줄 읽기
		fgets(line_p,70,fp);

		line_p[strlen(line_p)-1]='\0'; //줄 마지막 공백 없애줌.
		token=strtok(line_p,seps); //token으로 첫 단어 짜름


		//각 token을 integer형으로 바꿈
		w[0]=atoi(token);
		data[j]->company=w[0];

		//차례로 자르면서 각 데이터 저장..
		token=strtok(NULL,seps); 
		if(token==NULL)
			break;
		w[1]=atoi(token);
		data[j]->date=w[1];

		token=strtok(NULL,seps);
		w[2]=atoi(token);
		data[j]->time=w[2];

		token=strtok(NULL,seps);
		w[3]=atoi(token);
		data[j]->price=w[3];

		token=strtok(NULL,seps);
		w[4]=atoi(token);
		data[j]->volume=w[4];

		token=strtok(NULL,seps);
		w[5]=atoi(token);
		data[j]->x=w[5];
		
		total_price=total_price+data[j]->price;
		total_volume=total_price+data[j]->volume;
//		printf("%d번째 줄: %d/%d/%d/%d/%d/%d/\n",j+1,data[j]->company,data[j]->date,data[j]->time,data[j]->price,data[j]->volume,data[j]->x);
		j++;//줄수
	}
	//하루의 평균
	day_price[temp2]=total_price/j; 
	day_volume[temp2]=total_volume/j;
	

	fclose(fp);
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Stringize(int temp){
	int i;
	int j=0;//급감 구역의 수
	int j_u=0; //급증 구역의 수
	int k;
	int m;
	int Fac_j=1;
	int Fac_j_u=1;
	char* str;
	float rate_price;
	float rate_volume;
	char** Bstr;
	char** Wstr;
	char* lcs_temp;
	
	str=(char*)malloc(sizeof(char)*101);
	Bstr=(char**)malloc(sizeof(char*)*20);
	Wstr=(char**)malloc(sizeof(char*)*20);
	Pattern=(char***)malloc(sizeof(char**)*15);
//	str[0]='Y';
	
	for(i=1;i<101;i++){
		rate_price=(float)(day_price[i]-day_price[i-1])/(float)day_price[i-1]*100;
		rate_volume=(float)(day_volume[i]-day_price[i-1])/(float)day_volume[i-1]*100;
		//string 구간 지정해줘야..
		if(rate_price<=-3){
			if(rate_volume<=-45)
				str[i-1]='A';
			else if(rate_volume>-45 &&rate_volume<=0)
				str[i-1]='B';
			else if(rate_volume>0 && rate_volume<45)
				str[i-1]='C';
			else if(rate_volume>=45)
				str[i-1]='D';
		}
		else if(rate_price>-3 && rate_price<0){
			if(rate_volume<=-45)
				str[i-1]='E';
			else if(rate_volume>-45 &&rate_volume<=0)
				str[i-1]='F';
			else if(rate_volume>0 && rate_volume<45)
				str[i-1]='G';
			else if(rate_volume>=45)
				str[i-1]='H';
		}
		else if(rate_price<3 && rate_price>=0){
			if(rate_volume<=-45)
				str[i-1]='I';
			else if(rate_volume>-45 &&rate_volume<=0)
				str[i-1]='J';
			else if(rate_volume>0 && rate_volume<45)
				str[i-1]='K';
			else if(rate_volume>=45)
				str[i-1]='L';
		}
		else if(rate_price>=3){
			if(rate_volume<=-45)
				str[i-1]='M';
			else if(rate_volume>-45 &&rate_volume<=0)
				str[i-1]='N';
			else if(rate_volume>0 && rate_volume<45)
				str[i-1]='O';
			else if(rate_volume>=45)
				str[i-1]='P';
		}
		
	} //회사 1개의 stringize완성..
	str[i-1]='\0';
	printf("%s\n",str);

	for(i=0;i<101;i++){
		if(str[i]=='A' || str[i]=='B' ||str[i]=='C' || str[i]=='D'){
			if(i>=8){
				Wstr[j]=(char*)malloc(sizeof(char)*7+1);
				m=0;
				for(k=i-8;k<=i-1;k++){
					Wstr[j][m]=str[k];
					m++;//한 글자씩 저장
				}
				Wstr[j][m]='\0';
			}
			else{
				Wstr[j]=(char*)malloc(sizeof(char)*i+2);
				for(k=0;k<i;k++)
					Wstr[j][k]=str[k];
				Wstr[j][k]='\0';
			}
			j++; //급감하는 구역의 갯수
		}
		
		else if(str[i]=='M' || str[i]=='N' ||str[i]=='O' || str[i]=='P'){
			if(i>=8){
				Bstr[j_u]=(char*)malloc(sizeof(char)*7+1);
				m=0;
				for(k=i-8;k<=i-1;k++){
					Bstr[j_u][m]=str[k];
					m++;//한 글자씩 저장
				}
				Bstr[j_u][m]='\0';
			}
			else{
				Bstr[j_u]=(char*)malloc(sizeof(char)*i+2);
				for(k=0;k<i;k++)
					Bstr[j_u][k]=str[k];
				Bstr[j_u][k]='\0';
			}
			j_u++; //급증하는 구역의 갯수
		}
	}
	m=0;
	for(i=1;i<=j;i++)
		Fac_j=Fac_j*i;
	
	for(i=1;i<=j_u;i++)
		Fac_j_u=Fac_j_u*i;

	Pattern[temp]=(char**)malloc(sizeof(char*)*2);
	Pattern[temp][0]=(char*)malloc(sizeof(char)*20); //temp회사의 급감패턴
	Pattern[temp][1]=(char*)malloc(sizeof(char)*20); //temp회사의 급증패턴

	//일단 급감 패턴 찾기
	lcs_temp=(char*)malloc(sizeof(char)*21);
	for(i=0;i<j;i++){
		for(k=i+1;k<j;k++){
			if(i==0 && k==1){
				strcpy(Pattern[temp][0],LCS(Wstr[i],Wstr[k]));
			}
			
			strcpy(lcs_temp,LCS(Wstr[i],Wstr[k]));

			if(strlen(lcs_temp)>strlen(Pattern[temp][0]))
				strcpy(Pattern[temp][0],lcs_temp);
			
		}
	}

	for(i=0;i<j_u;i++){
		for(k=i+1;k<j_u;k++){
			if(i==0 && k==1){
				strcpy(Pattern[temp][1],LCS(Bstr[i],Bstr[k]));
			}
			
			strcpy(lcs_temp,LCS(Bstr[i],Bstr[k]));

			if(strlen(lcs_temp)>strlen(Pattern[temp][1]))
				strcpy(Pattern[temp][1],lcs_temp);
			
		}
	}
	
	printf("%d번째 회사 급감 패턴 : %s\n",temp+1,Pattern[temp][0]);
	printf("%d번째 회사 급증 패턴 : %s\n",temp+1,Pattern[temp][1]);
}



////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
char* LCS(char* a, char* b) {
	int n = strlen(a);
	int m = strlen(b);

	int** S;
	int** R;

	int ii;
	int jj;

	int pos;
	char* lcs;

	/* 
	 * Memory allocation.  Sigh.  What a quaint little idea. Nobody does this any more...
	 * A much better way to do this is to allocate one big array for S (and one for R) 
	 * and then do your own pointer indexing.  However, that makes the card just slightly 
	 * harder to read, so I avoid it.
	 */
	S = (int **)malloc( (n+1) * sizeof(int) );
	R = (int **)malloc( (n+1) * sizeof(int) );
	for(ii = 0; ii <= n; ++ii) {
		S[ii] = (int*) malloc( (m+1) * sizeof(int) );
		R[ii] = (int*) malloc( (m+1) * sizeof(int) );
	}

	/* It is important to use <=, not <.  The next two for-loops are initialization */
	for(ii = 0; ii <= n; ++ii) {
		S[ii][0] = 0;
		R[ii][0] = UP;
	}
	for(jj = 0; jj <= m; ++jj) {
		S[0][jj] = 0;
		R[0][jj] = LEFT;
	}

	/* This is the main dynamic programming loop that computes the score and */
	/* backtracking arrays. */
	for(ii = 1; ii <= n; ++ii) {
		for(jj = 1; jj <= m; ++jj) { 

			if( a[ii-1] == b[jj-1] ) {
				S[ii][jj] = S[ii-1][jj-1] + 1;
				R[ii][jj] = UP_AND_LEFT;
			}

			else {
				S[ii][jj] = S[ii-1][jj-1] + 0;
				R[ii][jj] = NEITHER;
			}

			if( S[ii-1][jj] >= S[ii][jj] ) {	
				S[ii][jj] = S[ii-1][jj];
				R[ii][jj] = UP;
			}

			if( S[ii][jj-1] >= S[ii][jj] ) {
				S[ii][jj] = S[ii][jj-1];
				R[ii][jj] = LEFT;
			}
		}
	}

	/* The length of the longest substring is S[n][m] */
	ii = n; 
	jj = m;
	pos = S[ii][jj];
	lcs = (char *) malloc( (pos+1) * sizeof(char) );

	lcs[pos--] = (char)NULL;

	/* Trace the backtracking matrix. */
	while( ii > 0 || jj > 0 ) {
		if( R[ii][jj] == UP_AND_LEFT ) {
			ii--;
			jj--;
			lcs[pos--] = a[ii];
		}
	
		else if( R[ii][jj] == UP ) {
			ii--;
		}

		else if( R[ii][jj] == LEFT ) {
			jj--;
		}
	}

	for(ii = 0; ii <= n; ++ii ) {
		free(S[ii]);
		free(R[ii]);
	}
	free(S);
	free(R);

	return lcs;
}