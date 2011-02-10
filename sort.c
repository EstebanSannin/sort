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

#define MAX 100

typedef struct st_ex { 
	char *string_order;
	char *full_string;
} Structs;

char *get_order_string(char *, char *, int, int);
void print_Structs(Structs *filemap, int filemap_len);
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
	puts("*** Struct sorting ...");
	// sort using comparision function 
	if(flag)
		qsort(all, len, sizeof(Structs), struct_cmp_reverse);
	else
		qsort(all, len, sizeof(Structs), struct_cmp);
	
	// print sorted struct array 
	//print_struct_array(all, len);
} 

void print_order_file(struct st_ex *array, size_t len) 
{ 
	size_t i;

	for(i=0; i<len; i++) 
		printf("%s", array[i].full_string);
	//puts("--");
} 

void usage(void){
	printf("\nUtility Sort by estebanSannin v0.1\n"
			"EXAMPLE: esort [option1] [option2]\n\n"
			"OPTION:\n"
			"-k select the char separator\n"
			"-t select the start and the end token to sort: -t2,3\n"
			"-v verbose mode\n"
			"-r reverse mode\n"
			"-n numeric mode\n");
}



/* MAIN program */ 
int main(int argc, char **argv) 
{ 	
	if(argc<3 || argc>5){
		usage();
		exit(-1);
	}
	int start = 0, end = 0;
	char *temp;
	char *separator;
	int verbose = 0;
	int reverse = 0;
	/* Read Argument*/
	while (--argc > 0){
		if(!(strncmp(argv[argc],"-t",2))){
			temp = &argv[argc][2];
			start = atoi(strtok(temp,","));
			end = atoi(strtok(NULL,","));
		}else if(!(strncmp(argv[argc],"-k",2))){
			separator = &argv[argc][2];
		}else if(!(strcmp(argv[argc],"-v"))){
			verbose = 1;
		}else if(!strcmp(argv[argc],"-r")){
			reverse = 1;
		}
		else{
			usage();
			exit(-1);
		}
	}
	char line[1024];	
	char **strarray = NULL;
	Structs *full_file;
	int i;
	int strcount = 0;  // number of line
	while((fgets(line, 1024, stdin)) != NULL) {
		strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
		strarray[strcount++] = strdup(line);

	}
	full_file=malloc(strcount * sizeof(Structs));
	for(i=0;i<strcount;i++){
		full_file[i].string_order = get_order_string(strdup(strarray[i]),separator,start,end);
		full_file[i].full_string = strarray[i];
	}
	sort_structs(full_file,strcount,reverse);
	if(verbose)
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










