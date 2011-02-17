/*
 *  NAME:	sort
 *  AUTHOR:	Stefano Viola aka estebanSannin
 *  CONTACT: 	stefanoviola@sannioglug.org
 *  SITE: 	http://hacklabproject.org
 *
 *  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX 100
#define BUF 1024
typedef struct st_ex { 
	char *string_order;
	char *full_string;
} Structs;

char *get_order_string(char *, char *, int, int);
void print_Structs(Structs *filemap, int filemap_len);

/* qsort int comparison function */ 
int int_cmp(const void *a, const void *b) 
{ 
	const int *ia = (const int *)a; // casting pointer types 
	const int *ib = (const int *)b;
	return *ia  - *ib; 
	/* integer comparison: returns negative if b > a 
	 * 	   and positive if a > b */ 
} 

/* qsort struct comparision function */ 
int struct_cmp_by_number(const void *a, const void *b) 
{ 
	struct st_ex *ia = (struct st_ex *)a;
	struct st_ex *ib = (struct st_ex *)b;
	return (int)((atoi(ia->string_order)) - (atoi(ib->string_order)));
	/* float comparison: returns negative if b > a 
	 * 	   and positive if a > b
	 * */ 
}


/* qsort struct comparision function */ 
int struct_cmp_by_number_reverse(const void *a, const void *b) 
{ 
	struct st_ex *ia = (struct st_ex *)a;
	struct st_ex *ib = (struct st_ex *)b;
	int number = ((atoi(ia->string_order)) - (atoi(ib->string_order)));
	if(number < 0)
		return 1;
	else if(number > 0)
		return -1;
	/* float comparison: returns negative if b > a 
	 * 	   and positive if a > b
	 * */ 
}



/* qsort struct comparision function */
int struct_cmp_reverse(const void *a, const void *b){
	Structs *ia = (Structs *)a;
	Structs *ib = (Structs *)b;
	int r = strcmp(ia->string_order, ib->string_order);
	if(r == 1)
		r = -1;
	else if(r == -1)
		r = 1;
	return r;
}
int struct_cmp(const void *a, const void *b)
{
	Structs *ia = (Structs *)a;
	Structs *ib = (Structs *)b;
	return strcmp(ia->string_order, ib->string_order);
	/* strcmp functions works exactly as expected from
	   comparison function */
}

/*struct array printing function */ 
void print_struct_array(struct st_ex *array, size_t len) 
{ 
	size_t i;

	for(i=0; i<len; i++) 
		printf("order string: %s \nfull string: %s\n", array[i].string_order, array[i].full_string);

	puts("--");
} 

/* sorting structs using qsort() example */

void sort_structs(Structs *all, int len, int flag) 
{ 
	// sort using comparision function 
	if(flag)
		qsort(all, len, sizeof(Structs), struct_cmp_reverse);
	else
		qsort(all, len, sizeof(Structs), struct_cmp);
} 

void sort_structs_numeric(Structs *all, int len, int flag) 
{ 
	// sort using comparision function 
	if(flag)
		qsort(all, len, sizeof(Structs), struct_cmp_by_number_reverse);
	else
		qsort(all, len, sizeof(Structs), struct_cmp_by_number);

} 

void print_order_file(struct st_ex *array, size_t len) 
{ 
	size_t i;

	for(i=0; i<len; i++) 
		printf("%s", array[i].full_string);
} 

void usage(void){
	printf("\nUtility Sort by estebanSannin v0.1\n"
			"EXAMPLE: esort [option1] [option2]\n\n"
			"OPTION:\n"
			"-k select the char separator\n"
			"-t select the start and the end token to sort: -t2,3\n"
			"-v verbose mode\n"
			"-r reverse mode\n"
			"-n numeric mode\n"
			"-h this message\n");
}

/* MAIN program */ 
int main(int argc, char **argv) 
{ 	

	char line[BUF];			//for reading line 
	char **strarray = NULL;		//for full line file
	Structs *full_file;		//struct for file
	int i;				//counter
	int strcount = 0;		//number of line

	int start = 0; 			
	int end = 0;
	char *separator = NULL;		//separator character
	int c;
	int index;

	/*flags*/
	int tflag = 0;
	int kflag = 0;
	int vflag = 0;
	int rflag = 0;
	int hflag = 0;
	int nflag = 0;
	opterr = 0;	
	while((c = getopt(argc, argv, "t:k:vrnh")) != -1){
		switch(c){
			case 't':
				start = atoi(strtok(optarg, ","));
				end = atoi(strtok(NULL, ","));
				tflag = 1;
				break;
			case 'k':
				kflag = 1;
				separator = optarg;
				break;
			case 'v':
				vflag = 1;
				break;
			case 'r':
				rflag = 1;
				break;
			case 'n':
				nflag = 1;
				break;
			case 'h':
				hflag = 1;
				break;
			case '?':
				if(optopt == 't' || optopt == 'k')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if(isprint(optopt)){
					fprintf(stderr, "Unknown option `-%c'.\nTry 'sort -h' for usage message.\n", optopt);

				}
				else
					fprintf(stderr, "Unknown option character `\\x%x'.\n",optopt);
				return 1;
			default:
				exit(1);
		}
	}
	for(index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);
	if(tflag == 1 && kflag == 0){
		printf("-r requires -k parameter\n");
		exit(1);
	}
	if(hflag){
		usage();
		exit(1);
	}

	while((fgets(line, 1024, stdin)) != NULL) {
		strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
		strarray[strcount++] = strdup(line);

	}
	full_file=malloc(strcount * sizeof(Structs));
	for(i=0;i<strcount;i++){
		if(tflag == 0 || (tflag == 0 && kflag == 0))
			full_file[i].string_order = strarray[i];
		else
			full_file[i].string_order = get_order_string(strdup(strarray[i]),separator,start,end);
		full_file[i].full_string = strarray[i];
	}
	if(nflag)
		sort_structs_numeric(full_file,strcount,rflag);
	else
		sort_structs(full_file,strcount,rflag);
	if(vflag)
		print_struct_array(full_file,strcount);
	print_order_file(full_file,strcount);
	return 0;
}

char *get_order_string(char *string, char *token, int start, int end){
	char *result;
	int count = 0;
	char *order_string=malloc(100 * sizeof(char *));
	result = strtok(string,token);
	while(result){
		count++;
		if(count >= start && count <= end){
			strcat(order_string,result);
		}
		result = strtok(NULL,token);
	}
	return order_string;

}










