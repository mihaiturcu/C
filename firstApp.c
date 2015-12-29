#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Angajati {
	char nume[50];
	char departament[50];
	int salar;
	struct tm angajare;
	struct tm plecare;
};

FILE *filehandler;

void printAngajat(struct Angajati *angajat);
Angajati getAngajatiFromFile(int nrangajat);
int countLines(char* numefis);
void printNumesiSalar(struct Angajati *angajat);
int ein6luni(struct Angajati *angajat);

void printMenu(){
	printf("%s", "##############################");
	printf("%s", "##############################");
	printf("%s", "1 - Afiseaza toti angajatii");
	printf("%s", "0 - Exit");
	printf("%s", "##############################");
	printf("%s", "##############################");
}

int main(){
	struct Angajati angajat[100];
	int i = 0;
	for (i = 0; i <= countLines("angajati.txt"); i++){
		angajat[i] = getAngajatiFromFile(i);
		printAngajat(&angajat[i]);
		printf("%s", "---------------------\n");
	}


	char s[] = "########################\n";
	printf("%s", s);
	printf("%s", "Angajati in ultimele 6 luni:\n");

	struct tm *current;
	time_t timenow;
	int temp;
	time(&timenow);
	current = localtime(&timenow);
	current->tm_year += 1900;
	current->tm_mon += 1;
	for (i = 0; i <= countLines("angajati.txt"); i++){
		if (ein6luni(&angajat[i]))
		{
			printNumesiSalar(&angajat[i]);
			printf("%s", "---------------------\n");
		}
	}

	return 1;

}
int ein6luni(struct Angajati *angajat)
{
	struct tm *current;
	time_t timenow;
	int temp;
	time(&timenow);
	current = localtime(&timenow);
	current->tm_year += 1900;
	current->tm_mon += 1;
	if (((current->tm_year * 12 + current->tm_mon) - (angajat->angajare.tm_year * 12 + angajat->angajare.tm_mon)) <= 6){
		if (((current->tm_year * 12 + current->tm_mon) - (angajat->angajare.tm_year * 12 + angajat->angajare.tm_mon)) == 6){
			//belesc pula la comparat zile
		}
		else
		{
			return 1;
		}

	}
	return 0;
}
void printNumesiSalar(struct Angajati *angajat){
	printf("Nume angajat: %s\n", angajat->nume);
	printf("Salar angajat: %d\n", angajat->salar);
}
void printAngajat(struct Angajati *angajat){
	printf("Nume angajat: %s\n", angajat->nume);
	printf("Departament angajat: %s\n", angajat->departament);
	printf("Salar angajat: %d\n", angajat->salar);
	printf("Data angajare angajat: %d.%d.%d\n", angajat->angajare.tm_mday, angajat->angajare.tm_mon, angajat->angajare.tm_year);
	printf("Data incheiere angajat: %d.%d.%d\n", angajat->plecare.tm_mday, angajat->plecare.tm_mon, angajat->plecare.tm_year);
}

tm strToTm(char *in){
	tm out;
	char *token = strtok(in, ".");
	char *buffer[40];
	int i = 0;
	while (token != NULL){
		buffer[i] = strdup(token);
		token = strtok(NULL, ".");
		i++;
	}
	out.tm_mday = atoi(buffer[0]);
	out.tm_mon = atoi(buffer[1]);
	out.tm_year = atoi(buffer[2]);
	return out;
}
int countLines(char* numefis)
{
	//also stupid as fuck, deschid fisierul intai ca sa numar cate linii am ca sa stiu cate elemente din vector instantiez
	FILE *f = fopen(numefis, "r");
	int ch = 0, lines = 0;
	while (!feof(f))
	{
		ch = fgetc(f);
		if (ch == '\n')
		{
			lines++;
		}
	}
	fclose(f);
	return lines;
}
Angajati getAngajatiFromFile(int nrangajat){
	// very fucking stupid implementation
	// de fiecare data cand apelez functia, parsez TOT fisierul, dar eu returnez ca un retardat doar cate 1 angajat :D
	// smarter way but also dumb = pui struct Angjati angajatulcitit[100] ca variabila globala, schimbi tipul metodei in void si nu mai faci return. apoi o apelezi doar o data.
	struct Angajati angajatulcitit[100];
	int i,x=0;
	FILE* f = fopen("angajati.txt", "r");
	char line[256], word[40],*buffer[40];
	while (fgets(line, sizeof(line), f)){
		i = 0;
		char *token = strtok(line, ";");
		while (token != NULL){
			buffer[i] = strdup(token);
			//printf("[%s]\n", token);
			token = strtok(NULL, ";");
			i++;
		}
		strcpy(angajatulcitit[x].nume, buffer[0]);
		strcpy(angajatulcitit[x].departament, buffer[1]);
		angajatulcitit[x].salar = atoi(buffer[2]);
		angajatulcitit[x].angajare = strToTm(buffer[3]);
		angajatulcitit[x].plecare = strToTm(buffer[4]);
		x++;
	}
	fclose(f);
	return angajatulcitit[nrangajat];
}
