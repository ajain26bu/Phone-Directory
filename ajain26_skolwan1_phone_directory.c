#define GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void MergeSort(int low, int high);
void Merge(int low, int mid, int hight);
void fMergeSort(int low, int high);
void fMerge(int low, int mid, int hight);
//3 functions for randomized quicksort as mentioned in CLRS book.
int partition(int,int);
int randomized_partition(int,int);
int randomized_quicksort(int,int);
void insertEntry(char *name, char *phone_number);
void sortDirectory();
void searchDirectory(char *name);

char pathname[16], arg_fname[32], arg_lname[32], arg_name[2][32], arg_number[16];

char clink[32][3][16], temp_clink[32][3][16], lines[100][64], names[100][32], fnames[100][32], lnames[100][32], numbers[100][16];
unsigned long long int clean_numbers[32], link[32][2], temp_link[32][2];
int phonebook_entries;

struct phonebook{
	char fname[16];
	char lname[16];
	char number[16];
	unsigned long long int rnumber;
}phonebooks[64];

int contacts, no_exist;

void read_file(char *input) {
	//Arbitrary variables
	int i, j=0;
	no_exist = 0;
	//Variables associated with reading the input txt file
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	FILE * fp;

	//Getting input from the user
	//printf("Enter the name of the text file: ");
	//scanf("%s", pathname);
	fp = fopen(input, "r");

	//Error message if file does not exist.
	if (fp == NULL){
		printf("The file doesn't exist!\n");
		no_exist = 1;
		return;
	}
	else{
		j =0;
		//Print the whole file for context.
		while (((read = getline(&line, &len, fp)) != -1)) {
			//Copies the individual characters to the lines 2x2 array and increments after each line ends.
			for(i=0; line[i]!='\0'; i++){
				lines[j][i] = line[i];
			}
			j++;
		}
		//Save the total number of contacts in this global var
		contacts = j;
	}
	fclose(fp);

}

//A function that splits each line of the input into name and number, coded from scratch.
int split_main(){
	int i = 0, j =0, k=0, l=0;
	while(i < contacts){
		while(lines[i][j] != '\n'){
			if(lines[i][j] != ','){
				names[k][l] = lines[i][j];
				l++;
				j++;
			}
			if(lines[i][j] == ','){
				names[k][l] = '\0';
				l = 0;
				j++;
				while(lines[i][j] != '\n'){
					numbers[k][l] = lines[i][j];
					phonebooks[k].number[l] = numbers[k][l];
					l++;
					j++;
				}

			}
		}
		i++;
		numbers[k][l] = '\0';
		k++;
		j = 0;
		l = 0;
	}

	return 0;
}

//Splitting the name in fname and lname using space as delimitter.
int split_name(){
	int i = 0, j = 0, k = 0, comma = 0;
	while (i < contacts){
		while (j < strlen(names[i])){
			if(names[i][j] == ' '){
				comma = 1;
				phonebooks[i].fname[k] = '\0';
				k = 0;
				j++;
			}
			if(comma == 0){
				phonebooks[i].fname[k] = names[i][j];
				fnames[i][k] = phonebooks[i].fname[k];
				k++;
			}
			else{
				phonebooks[i].lname[k] = names[i][j];
				lnames[i][k] = phonebooks[i].lname[k];
				k++;
			}
			j++;
		}
		phonebooks[i].lname[k] = '\0';
		names[i][j] = '\0';
		i++;
		j = 0;
		k = 0;
		comma = 0;
	}
	phonebook_entries = i;

	/*for(int i = 0; i < phonebook_entries; i++){
		strcpy(clink[i][0],fnames[i]);
		strcpy(clink[i][1],lnames[i]);
		printf("%d. %s %s\n", i, clink[i][0], clink[i][1]);
	}*/
	return 0;
}

//Cleaning the numbers, converting from string to llu.
void cast_number(){
	int i = 0, j;
	unsigned long long int decimate = 0;
	while(i<phonebook_entries){
		decimate = 0;
		for(j=0; j<strlen(numbers[i]); j++){
			decimate = decimate*10 + numbers[i][j] - '0';
		}
		clean_numbers[i] = decimate;
		i++;
	}
	for(int i=0; i<phonebook_entries; i++){
		link[i][0] = clean_numbers[i];
		phonebooks[i].rnumber = clean_numbers[i];
		link[i][1] = i;
	}
}


//A function that collectively reads input file, splits names and numbers, and then splits names into fname and lname.
void create(char *pathname){
	read_file(pathname);
	split_main();
	split_name();
	cast_number();
}

void MergeSort(int low, int high){
	int mid = 0;
	if(low < high){
		mid = ((low+high)/2);
		MergeSort(low, mid);
		MergeSort(mid+1,high);
		Merge(low,mid,high);
	}
}

void fMergeSort(int low, int high){
	int mid = 0;
	if(low < high){
		mid = ((low+high)/2);
		fMergeSort(low, mid);
		fMergeSort(mid+1,high);
		fMerge(low,mid,high);
	}
}

void Merge(int low, int mid, int high){
	int i = low, j = mid+1, k = low;
	char temp[32][16];

	while(i<=mid && j<= high){
		if(strcmp(lnames[i],lnames[j]) < 0){
			strcpy(temp[k],lnames[i]);
			phonebooks[phonebook_entries+k] = phonebooks[i];
			i++;
		}
		else{
			strcpy(temp[k],lnames[j]);
			phonebooks[phonebook_entries+k] = phonebooks[j];
			j++;
		}
		k++;
	}

	if(i > mid){
		for(int h = j; h<= high; h++){
			strcpy(temp[k],lnames[h]);
			phonebooks[phonebook_entries+k] = phonebooks[h];
			k++;
		}
	}
	else{
		for(int h = i; h <= mid; h++){
			strcpy(temp[k],lnames[h]);
			phonebooks[phonebook_entries+k] = phonebooks[h];
			k++;
		}
	}
	for(int i = low; i<=high; i++){
		strcpy(lnames[i],temp[i]);
		phonebooks[i] = phonebooks[phonebook_entries+i];
	}
}

void fMerge(int low, int mid, int high){
	int i = low, j = mid+1, k = low;
	char temp[32][16];

	while(i<=mid && j<= high){
		if(strcmp(phonebooks[i].fname,phonebooks[j].fname) < 0){
			strcpy(temp[k],fnames[i]);
			phonebooks[phonebook_entries+k] = phonebooks[i];
			i++;
		}
		else{
			strcpy(temp[k],fnames[j]);
			phonebooks[phonebook_entries+k] = phonebooks[j];
			j++;
		}
		k++;
	}

	if(i > mid){
		for(int h = j; h<= high; h++){
			strcpy(temp[k],fnames[h]);
			phonebooks[phonebook_entries+k] = phonebooks[h];
			k++;
		}
	}
	else{
		for(int h = i; h <= mid; h++){
			strcpy(temp[k],fnames[h]);
			phonebooks[phonebook_entries+k] = phonebooks[h];
			k++;
		}
	}
	for(int i = low; i<=high; i++){
		strcpy(fnames[i],temp[i]);
		phonebooks[i] = phonebooks[phonebook_entries+i];
	}

}


//Updates the CSV File after sort
void auto_add(){
	FILE *fp;
	fp = fopen(pathname, "w");
	fseek(fp, 0, SEEK_END);
	for(int i=0; i<phonebook_entries; i++){
		fprintf(fp,"%s %s,%llu\n", phonebooks[i].fname, phonebooks[i].lname, phonebooks[i].rnumber);
	}
	fclose(fp);
}

//Associated with the ADD operation
void insertEntry(char *name, char *phone_number){
	for(int i = 0; i < phonebook_entries; i++){
		if((strcmp(phonebooks[i].fname,arg_fname)==0) && (strcmp(phonebooks[i].lname,arg_lname)==0) && (strcmp(phonebooks[i].number,arg_number)==0)){
			printf("Contact already exists!\n");
			return;
		}
	}
	FILE *fp;
	fp = fopen(pathname, "a+");
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "%s %s,%s\n", arg_fname, arg_lname, arg_number);
	fclose(fp);
	printf("Contact added to the phonebook\n");
	return;
}


//Checks for duplicates
void lduplicates(){
	for(int i = 0; i<phonebook_entries; i++){
		for(int j = i+1; j<phonebook_entries; j++){
			if (strcmp(phonebooks[i].lname,phonebooks[j].lname)==0){
				fMergeSort(i,j);
			}
		}
	}
}

void flduplicates(){
	for(int i = 0; i < phonebook_entries; i++){
		for(int j = i + 1; j<phonebook_entries; j++){
			if ((strcmp(phonebooks[i].lname,phonebooks[j].lname) == 0) && strcmp(phonebooks[i].fname,phonebooks[j].fname) == 0){
				randomized_quicksort(i,j);
			}
		}
	}
}

void sortDirectory(){
	MergeSort(0,phonebook_entries-1);
	lduplicates();
	flduplicates();
	auto_add();
}

void showDirectory(){
	for(int i = 0; i < phonebook_entries; i++) {
		if(strlen(phonebooks[i].fname)+strlen(phonebooks[i].lname) > 14) {
			printf("%d.\t%s %s\t%llu\n", i+1, phonebooks[i].fname, phonebooks[i].lname, phonebooks[i].rnumber);
		}
		else{
			printf("%d.\t%s %s\t\t%llu\n", i+1, phonebooks[i].fname, phonebooks[i].lname, phonebooks[i].rnumber);
		}
	}
}

void deleteEntry(char *name){
	int found = -1;
	for(int i = 0; i < phonebook_entries; i++){
		if((strcmp(phonebooks[i].fname,arg_fname) == 0) && (strcmp(phonebooks[i].lname,arg_lname) == 0)){
			found = i;
		}
	}
	if(found > -1) {
		printf("Contact deleted.\n");
		while(found < phonebook_entries){
			for (int i = found; i < phonebook_entries - 1; i++) {
				phonebooks[found] = phonebooks[found + 1];
			}
			found++;
		}

		phonebook_entries--;
		auto_add();

	}
	else printf("Unable to delete as entry not found\n");
}

void searchDirectory(char *name){
	int range;
	int n;
	int i = 0;
	int m = 3;
	int found = 0;
	int low, high, mid;
	low=0;
	high = phonebook_entries-1;
	while(low <= high)
	{
		mid=(low + high)/2;
		if ((strcmp(arg_fname,phonebooks[mid].fname)==0) && (strcmp(arg_lname,phonebooks[mid].lname) == 0))
		{
			printf("%d\t%s %s\t%s\n",mid+1, phonebooks[mid].fname, phonebooks[mid].lname, phonebooks[mid].number );
			range = mid;
			for(int l = 0; l < m; l++){
				if ((strcmp(arg_fname,phonebooks[range-1].fname)==0) && (strcmp(arg_lname,phonebooks[range-1].lname) == 0)) {
					printf("%d\t%s %s\t%s\n", range, phonebooks[range - 1].fname, phonebooks[range - 1].lname, phonebooks[range - 1].number);
					range = range - 1;
				}
			}
			range = mid;
			for(int l = 0; l < m; l++){
				if(range+1 < phonebook_entries){
					if ((strcmp(arg_fname,phonebooks[range+1].fname)==0) && (strcmp(arg_lname,phonebooks[range+1].lname) == 0)) {
						printf("%d\t%s %s\t%s\n", range + 2, phonebooks[range + 1].fname, phonebooks[range + 1].lname, phonebooks[range + 1].number);
						range = range + 1;
					}
				}
			}
			found = 1;
			return;

		}
		else if(strcmp(arg_lname,phonebooks[mid].lname)>0)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}

	if(found == 0) printf("name not found\n");

}

int main(int argc, char *argv[]){

	if(argc < 2){
		printf("\nPlease enter the command line argument in the specified format:\n <outfile> <filename.csv>: ");
		exit(1);
	}
	else if(argc == 2){
		strcpy(pathname,argv[1]);
	}

	create(pathname);
	sortDirectory();

	if(no_exist == 0) {
		printf("\nCurrent phonebook entries: \n\n");
		for (int i = 0; i < phonebook_entries; i++) {
			if (strlen(phonebooks[i].fname) + strlen(phonebooks[i].lname) > 14) {
				printf("%d.\t%s %s\t%llu\n", i + 1, phonebooks[i].fname, phonebooks[i].lname, phonebooks[i].rnumber);
			} else {
				printf("%d.\t%s %s\t\t%llu\n", i + 1, phonebooks[i].fname, phonebooks[i].lname, phonebooks[i].rnumber);
			}
		}


		int choice;
		char ch = 'y';
		while (ch == 'y') {
			printf("\nPlease provide options:\nInsert\t\t(Press 1)\nSearch\t\t(Press 2)\nDelete\t\t(Press 3)\nShow Directory\t(Press 4)\nExit\t\t(Press 5)\n");
			scanf("%d", &choice);
			switch (choice) {
				case 1:
					printf("\nInsert\n");
					printf("Enter the full name: ");
					scanf("%s %s", &arg_fname, &arg_lname);
					strcpy(arg_name[0], arg_fname);
					strcpy(arg_name[1], arg_lname);
					printf("Enter the phone number (digits only): ");
					scanf("%s", &arg_number);
					insertEntry(*arg_name, arg_number);
					create(pathname);
					sortDirectory();
					break;
				case 2:
					printf("\nSearch\n");
					printf("Enter the full name of the contact: ");
					scanf("%s %s", &arg_fname, &arg_lname);
					strcpy(arg_name[0], arg_fname);
					strcpy(arg_name[1], arg_lname);
					searchDirectory(*arg_name);
					break;
				case 3:
					printf("\nDelete\n");
					printf("Enter the full name of the contact you wish to delete: ");
					scanf("%s %s", &arg_fname, &arg_lname);
					strcpy(arg_name[0], arg_fname);
					strcpy(arg_name[1], arg_lname);
					deleteEntry(*arg_name);
					break;
				case 4:
					printf("\nShow Directory\n");
					showDirectory();
					break;
				case 5:
					printf("\nExit\n");
					exit(1);
					break;
			}
			printf("\nEnter y to continue\n");
			scanf("%s", &ch);
		}
	}
	return 0;
}

//END OF CHANGES

int randomized_quicksort(int x, int y){
	int temp;
	if(x<y)
	{
		temp = randomized_partition(x,y);
		randomized_quicksort(x,temp-1);
		randomized_quicksort(temp+1,y);
	}
	return 0;
}

int randomized_partition(int x, int y){
	//Randomly choose the pivot element
	int i = x+rand()%(y-x+1);
	//Basic swap
	phonebooks[phonebook_entries] = phonebooks[y];
	phonebooks[y] = phonebooks[i];
	phonebooks[i] = phonebooks[phonebook_entries];
	return partition(x,y);
}

int partition(int x, int y){
	int i = x-1;
	unsigned long long int w;
	w  = phonebooks[y].rnumber;
	for(int j=x;j<=y-1;j++){
		if(phonebooks[j].rnumber<=w){
			i = i +1;
			phonebooks[phonebook_entries] = phonebooks[i];
			phonebooks[i] = phonebooks[j];
			phonebooks[j] = phonebooks[phonebook_entries];
		}
	}
	phonebooks[phonebook_entries+1] = phonebooks[i+1];
	phonebooks[i+1] = phonebooks[y];
	phonebooks[y] = phonebooks[phonebook_entries+1];
	return i+1;
}