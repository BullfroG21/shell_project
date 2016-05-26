/*
 ============================================================================
 Name        : shell.c
 Author      : André Schaiter Florian Gwechenberger
 Version     :
 Copyright   : made by André Schaiter Florian Gwechenberger
 Description : FHS Lite Shell in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

struct tm *tmnow;
char tasks[5][8] ={"fhsdate","fhstime","echo","help","exit"};
char helps[5][100]={{"Befehl [fhsdate] gibt das Datum von heute aus"},
                    {"Befehl [fhstime] gibt die jetztige Uhrzeit aus"},
                    {"Befehl [echo] gibt die nachstehenden Zeichen nochmal aus"},
                    {"Befehl [help] gibt alle Befehle aus"},
                    {"Befehl [exit] schließt das Programm"}};

int main(void) {
	 DIR * dir;
	 struct dirent * currentdir;
	 FILE * history_file;
	 int counter = 0;
	 history_file = fopen("history.txt","r+");
	 //Fehlerkontrolle für fopen missing
	 char * charpwd = malloc(sizeof(char)* 1024);
	 if(charpwd == NULL){
		 printf("Fehler beim allokieren!");
		 return -1;
	 }
	 strcpy(charpwd,"/home/flo"); //copy of starting directory
	 int exit = 0; //0= false; 1=true for action "exit"
	 //loop until user writes "exit"
	 if ((dir = opendir (charpwd)) != NULL) {
		  currentdir = readdir (dir);
		 // printf ("%s\n", currentdir->d_name);
	 }
	 else{
		  printf("Failed to open dir");
	 }
	 while(exit==0){
		  //echo; fhsdate; fhstime; exit; help;
		 int i;
		  int inputlength;
		  char arguments[100];
		  int attachment = 0;
		  char * input = malloc(sizeof(char)*1000);
		  //tries to open dir
		  printf("%s> ",charpwd);


		  //File * stream;
		  fgets(input,1000, stdin);
		  char input_history [strlen(input)];
		  fseek(history_file,0,SEEK_SET);
		  fscanf(history_file, "%i", &counter);
		  counter++;
		  char tmpzahl[10];
		  sprintf(tmpzahl, "%i", counter);
		  fseek(history_file,0,SEEK_SET);
		  fprintf(history_file,tmpzahl);
		  //strcpy(input_history,(char *)counter);
		  fseek(history_file,0,SEEK_END);
		  strcpy(input_history,input);
		  fprintf(history_file,input_history);



		  inputlength= strlen(input);
		  realloc(input,strlen(input));
		  int pos;
		  pos = strcspn(input," ");
		  //"Switch"
		  if(pos<strlen(input)){
			  attachment = 1;
			  strcpy(arguments,input+pos+1);
			  realloc(input,pos);
			  input[pos]='\0';
			  arguments[inputlength-pos-2]='\0';
		  }
		  else{
			  input[inputlength-1]='\0';
		  }
		  //action echo
		  if(strcmp(input,"echo")==0){
			  printf("%s\n",arguments);
		  }
		  //action cd - change directory
		  if(strcmp(input,"cd")==0){
			  char * tmpstring = malloc(sizeof(char)*1024);
			  if(tmpstring==0){
				  printf("Fehler beim allokieren!");
				  return -1;
			  }
			  strcpy(tmpstring,charpwd);
			  //check which direction
			  //checko on cd .. - return to one dir back
			  if(strcmp(arguments,"..")==0){
				  int tmppos = 0;
				  //check every single position if there a the last valid position
				  for(i=0;i<strlen(tmpstring)-1;i++){
					  if(tmpstring[i] == '/'){
						  tmppos = i;
					  }
				  }
				  tmpstring[tmppos+1]='\0';
			  }
			  //check if requested whole path
			  else if(arguments[0]=='/'){
				  strcpy(tmpstring,arguments);
			  }
			  //add folder to path
			  else{
				  strcat(tmpstring,arguments);
			  }

			  //check if last char == /, if not add /
			  if(tmpstring[strlen(tmpstring)-1]!='/'){
				  strcat(tmpstring,"/");
			  }
			  //check if path is reachable
			  if ((dir = opendir (tmpstring)) != NULL) {
			  		  strcpy(charpwd,tmpstring);
			  	 }
			  	 else{
			  		  printf("Failed to open dir\n");
			  }
			  free(tmpstring);
		  }
		  //action help
		  else if(strcmp(input,"help")==0){
			   int i;
			   if(attachment==1){
					for(i=0; i<(sizeof(tasks)/8);i++){
						 if(strcmp(arguments,tasks[i])==0){
							  printf("%s",helps[i]);
							  break;
						 }
					}
			   }
			   else{
				   for(i=0; i<(sizeof(tasks)/8);i++){
					printf("-%s\n",tasks[i]);
				}
			   }
		  }
		  else if(strcmp(input,"pushd")==0){
			  FILE * pushing;
			  pushing = fopen("path.txt","w");
			  fprintf(pushing, charpwd);
			  fclose(pushing);
		  }
		  else if(strcmp(input,"history")==0){

		  }
		  //action fhsdate
		  else if(strcmp(input,"fhsdate")==0){
			 getdate();
		  }
		  //action fhstime
		  else if(strcmp(input,"fhstime")==0){
			 getTime();
		  }
		  else if(strcmp(input,"pwd")==0){
			 printf("%s\n",charpwd);
		  }
		  //action exit
		  else if(strcmp(input,"exit")==0){
			 exit = 1;
		  }
		  else{
			 printf("Unknown input: %s.\n", input);
		  }
	 }

	 fclose(history_file);
	 return 0;
}
void getdate(){
   time_t tnow;

   time(&tnow);
   tmnow = localtime(&tnow);
   printf("Heute ist der ");
   printf("%d.%d.%d\n",
      tmnow->tm_mday, tmnow->tm_mon + 1, tmnow->tm_year + 1900);
}

void getTime() {
   struct tm *zeit;
   time_t sekunde;
   char string[80];
   time(&sekunde);
   zeit = localtime(&sekunde);
   strftime(string, 80,"Es ist %H Uhr und %M Minuten",zeit);
   printf("%s\n",string);
}

void getecho(int size, char arguments[size]){
 printf("%s",arguments);
}
void getexit(){
 exit;
}
void gethelp(int size, char argument[size]){

}