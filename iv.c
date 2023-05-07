#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int calculate(int stat, int base, int level, float nature, int ev){
	int ivmin, ivmax, i=1;
	ivmin = ceil((ceil(stat/nature)-5)*100.0/level)-base*2-ev/4;
	while(floor(((2*base+ivmin+i+ev/4)*level/100+5)*nature)==stat){++i;}
	ivmax=ivmin+i-1;
	if(ivmin<0){ivmin=0;}
	if(ivmax>31){ivmax=31;}
	return (ivmin*100+ivmax);
}

int calculateHP(int stat, int base, int level, int ev){
	int ivmin, ivmax, i=1;
	ivmin = ceil((stat-10.0-level)*100/level)-base*2-ev/4;
	while((2*base+ivmin+i+ev/4)*level/100+level+10==stat){++i;}
	ivmax=ivmin+i-1;
	if(ivmin<0){ivmin=0;}
	if(ivmax>31){ivmax=31;}
	return (ivmin*100+ivmax);
}

void output(int i, int base, int stat, int ev, double nature_val[], char nature[], int level){
	int ivmin, ivmax, statmin, statmax;
	char nature_boost[4], nature_hinder[4], current_nat[4], inv_stat_message[20];
	strcpy(inv_stat_message, "");
	if(i==0){
		ivmin = calculateHP(stat, base, level, ev)/100;
		ivmax = calculateHP(stat, base, level, ev)%100;
		statmin = (2*base+0+ev/4)*level/100+10+level;
		statmax = (2*base+31+ev/4)*level/100+10+level;
		if(stat<statmin||stat>statmax){strcpy(inv_stat_message, "	INVALID STAT"); ivmin=0; ivmax=0;}
		if(ev>255||ev<0){strcat(inv_stat_message, "/EV"); statmin=0; statmax=0;}
		printf("\n 	IVs	RANGE	STAT 	BASE\n");
		printf("HP/ 	%d-%d	%d-%d	%d 	%d%s\n", ivmin, ivmax, statmin, statmax, stat, base, inv_stat_message);
	}
	statmin = ((2*base+ev/4)*level/100+5)*nature_val[i];
	statmax = ((2*base+31+ev/4)*level/100+5)*nature_val[i];
	ivmin = calculate(stat, base, level, nature_val[i], ev)/100;
	ivmax = calculate(stat, base, level, nature_val[i], ev)%100;
	if(stat<statmin||stat>statmax){strcpy(inv_stat_message, "	INVALID STAT"); ivmin=0; ivmax=0;}
	if(ev>255||ev<0){strcat(inv_stat_message, "/EV"); statmin=0; statmax=0;}
	if(i==1){
		printf("ATT/ 	%d-%d	%d-%d	%d 	%d%s\n", ivmin, ivmax, statmin, statmax, stat, base, inv_stat_message);}
	else if(i==2){
		printf("DEF/ 	%d-%d	%d-%d	%d 	%d%s\n", ivmin, ivmax, statmin, statmax, stat, base, inv_stat_message);}
	else if(i==3){
		printf("SPA/ 	%d-%d	%d-%d	%d 	%d%s\n", ivmin, ivmax, statmin, statmax, stat, base, inv_stat_message);}
	else if(i==4){
		printf("SPD/ 	%d-%d	%d-%d	%d 	%d%s\n", ivmin, ivmax, statmin, statmax, stat, base, inv_stat_message);}
	else if(i==5){
		printf("SPE/ 	%d-%d	%d-%d	%d 	%d%s\n", ivmin, ivmax, statmin, statmax, stat, base, inv_stat_message);

		if(strcmp(nature, "HARDY")==0){strcpy(nature_boost, "ATT");strcpy(nature_hinder, "ATT");}
		else if(strcmp(nature, "DOCILE")==0){strcpy(nature_boost, "DEF");strcpy(nature_hinder, "DEF");}
		else if(strcmp(nature, "BASHFUL")==0){strcpy(nature_boost, "SPA");strcpy(nature_hinder, "SPA");}
		else if(strcmp(nature, "QUIRKY")==0){strcpy(nature_boost, "SPD");strcpy(nature_hinder, "SPD");}
		else if(strcmp(nature, "SERIOUS")==0){strcpy(nature_boost, "SPE");strcpy(nature_hinder, "SPE");}
		for(int i=1; i<6; ++i){
			if(i==1){strcpy(current_nat, "ATT");}
			else if(i==2){strcpy(current_nat, "DEF");}
			else if(i==3){strcpy(current_nat, "SPA");}
			else if(i==4){strcpy(current_nat, "SPD");}
			else if(i==5){strcpy(current_nat, "SPE");}
			if(fabs(nature_val[i]-1.1)<0.001){strcpy(nature_boost, current_nat);}
			else if(fabs(nature_val[i]-0.9)<0.001){strcpy(nature_hinder, current_nat);}
		}
		printf("+%s -%s\n",nature_boost, nature_hinder);
	}
}

int main(){
	int level, valid_name=0, stat, base, ev;
	double nature_val[6];
	char name[50], stats[50], evs[50], nature[20], line[150], base_stats[150], temp[150];
	char *p;
	FILE *textfile = fopen("data", "r");
	if(textfile==NULL){printf("ERROR: DATA FILE MISSING\n"); return 1;}

	printf("NAME: ");scanf("%s", name);
	for(int i=0; i<strlen(name); ++i){name[i]=toupper(name[i]);}
	while(fgets(line, 100, textfile)){
		strcpy(base_stats, line);
		p=strtok(line, " ");
		for(int i=0; i<strlen(p); ++i){p[i]=toupper(p[i]);}
		if(strcmp(p, name)==0){
			valid_name=1;
			break;
		}
	}
	fclose(textfile);
	if(valid_name==0){printf("INVALID NAME\n");exit(0);}
	printf("LEVEL: ");scanf("%d", &level);
	if(level<1||level>100){printf("INVALID LEVEL\n");exit(0);}
	printf("STATS: ");scanf("%s", stats);
	printf("EVS: ");scanf("%s", evs);
	printf("NATURE: ");scanf("%s", nature);
	for(int i=0; i<strlen(nature); ++i){nature[i]=toupper(nature[i]);}
	textfile = fopen("data", "r");
	while(fgets(line, 100, textfile)){
		if(strcmp(line, "end\n")==0){printf("INVALID NATURE\n");exit(0);}
		p=strtok(line, " ");
		if(strcmp(p, nature)==0){
			for(int i=1; i<6; ++i){
				p=strtok(NULL, " ");
				if(atoi(p)==2){nature_val[i]=(double)(1.1);}
				else if(atoi(p)==0){nature_val[i]=(double)(0.9);}
				else{nature_val[i]=(double)(1.0);}
			}
			break;
		}
	}
	for(int i=0; i<6; ++i){
		strcpy(temp, base_stats);
		p=strtok(temp, " ");
		for(int j=0; j<i+1; ++j){p=strtok(NULL, " ");}
		base=atoi(p);
		strcpy(temp, stats);
		p=strtok(temp, "-");
		for(int j=0; j<i; ++j){p=strtok(NULL, "-");}
		if(p!=NULL){stat=atoi(p);}
		else{stat=0;}
		strcpy(temp, evs);
		p=strtok(temp, "-");
		for(int j=0; j<i; ++j){p=strtok(NULL, "-");}
		if(p!=NULL){ev=atoi(p);}
		else{ev=atoi(temp);}
		output(i, base, stat, ev, nature_val, nature, level);
	}
}