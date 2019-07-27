//
//  birdSearch.c
//  Adv_C_Track_Cisco
//
//  Created by Akshay Mittal on 30/06/19.
//  Copyright © 2019 Akshay Mittal. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BIRD_NAME_LEN 100
#define NUM_YRS 22 // 2018 - 1997 + 1
#define NUM_STATS 100

// Supply the required paths below
#define OBS_FILE_PATH ..
#define BIRD_FILE_PATH ..
//************************************


struct Obs_Record{
    unsigned long long free:12;
    unsigned long long yr:11;
    unsigned long long month:4;
    unsigned long long day:5;
    unsigned long long id:16;
    unsigned long long num:16;
};

struct sightsYrPair{
    unsigned long long sights;
    int yr;
};

struct sighting_stats{
    short bid;
    char b_name[MAX_BIRD_NAME_LEN];
    struct sightsYrPair sightings[NUM_YRS];
};

typedef struct sighting_stats sighting_stats;
typedef struct Obs_Record obs_record;

bool isContainsWord(char*, char*);
void swap(struct sightsYrPair* a, struct sightsYrPair* b);
int partition(sighting_stats arr[], int l, int h, int index);
void quickSort(sighting_stats arr[], int l, int h, int index);
int getRecordObs(obs_record*, unsigned long long);
sighting_stats* searchStats(sighting_stats*, unsigned long long, int);
int cmpfunc (const void*, const void*);
void printSightingsYrWise(sighting_stats*, unsigned long long);

int main(int argc, char* argv[]) {
    short bid;
    char len;
    char b_name[MAX_BIRD_NAME_LEN];
    char keyword[MAX_BIRD_NAME_LEN];
    unsigned long long n = 0;
    unsigned long long obsi;
    obs_record obsr;
    unsigned long long limit_stats = NUM_STATS;
    
    sighting_stats* availableStat;
    sighting_stats* stats = (sighting_stats*)malloc(sizeof(sighting_stats)*NUM_STATS);
    
    FILE* obs_file = fopen(OBS_FILE_PATH, "rb");
    if(obs_file == NULL){
        fprintf(stderr, "Unable to open file:");
        fprintf(stderr, OBS_FILE_PATH);
        return 1;
    }
    FILE* brd_file = fopen(BIRD_FILE_PATH, "rb");
    if(brd_file == NULL){
        fprintf(stderr, "Unable to open file:");
        fprintf(stderr, BIRD_FILE_PATH);
        return 1;
    }
    
    printf("Keyword?\n");
    scanf("%s", keyword);
    
    while((fread(&bid, sizeof(bid), 1, brd_file))==1){
        if((fread(&len, sizeof(len), 1, brd_file))==1){
            fread(b_name, sizeof(char), len, brd_file);
            b_name[len]=0;
        }
        if(isContainsWord(b_name, keyword)){
            fseek(obs_file, 0, SEEK_SET);
            while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
                getRecordObs(&obsr, obsi);
                if((availableStat=searchStats(stats, n, bid))!=NULL){
                    availableStat->sightings[obsr.yr-1997].sights+=obsr.num;
                    availableStat->sightings[obsr.yr-1997].yr=obsr.yr;
                    continue;
                }
                else{
                    strcpy(stats[n].b_name, b_name);
                    stats[n].bid=bid;
                    stats[n].sightings[obsr.yr-1997].sights=obsr.num;
                    stats[n].sightings[obsr.yr-1997].yr=obsr.yr;
                    n++;
                    if(n==limit_stats-2){
                        stats = realloc(stats, limit_stats+NUM_STATS);
                        limit_stats+=NUM_STATS;
                    }
                }
            }
        }
    }
    for(int i=0; i<n; i++){
        quickSort(stats, 0, NUM_YRS-1, i);
    }
    printSightingsYrWise(stats, n);
    fclose(obs_file);
    fclose(brd_file);
    free(stats);
    return 0;
}

void swap(struct sightsYrPair* a, struct sightsYrPair* b)
{
    struct sightsYrPair t = *a;
    *a = *b;
    *b = t;
}

int partition(sighting_stats arr[], int l, int h, int index){
    unsigned long long x = arr[index].sightings[h].sights;
    int i=l-1;
    for (int j = l; j <= h - 1; j++) {
        if (arr[index].sightings[j].sights <= x) {
            i++;
            swap(&arr[index].sightings[i], &arr[index].sightings[j]);
        }
    }
    swap(&arr[index].sightings[i+1], &arr[index].sightings[h]);
    return (i + 1);
}

void quickSort(sighting_stats arr[], int l, int h, int index){
    int stack[h-l+1];
    int top = -1;
    stack[++top] = l;
    stack[++top] = h;
    while (top >= 0) {
        h = stack[top--];
        l = stack[top--];
        int p = partition(arr, l, h, index);
        if (p - 1 > l) {
            stack[++top] = l;
            stack[++top] = p - 1;
        }
        if(p+1<h){
            stack[++top]=p+1;
            stack[++top]=h;
        }
    }
}

int cmpfunc (const void * a, const void * b) {
    unsigned long long l = ((struct sightsYrPair*)a)->sights;
    unsigned long long r = ((struct sightsYrPair*)b)->sights;
    if(l<r) return -1;
    else if(l==r) return 0;
    else return 1;
}

sighting_stats* searchStats(sighting_stats* stats, unsigned long long max, int id){
    sighting_stats* ptr;
    for(int i=0; i<max; i++){
        if(stats[i].bid==id){
            ptr=stats+i;
            return ptr;
        }
    }
    return NULL;
}

bool isContainsWord(char* fullStr, char* keyword){
    unsigned long len = strlen(keyword);
    int n=0, m=0;
    while(fullStr[n] != '\0'){
        if(fullStr[n] == keyword[m]){
            while(fullStr[n] == keyword[m] && fullStr[n]!='\0'){
                n++;
                m++;
            }
            if(m == len && (fullStr[n] == ' ' || fullStr[n] == '\0')){
                return true;
            }
        }
        else {
            while(fullStr[n]!=' '){
                n++;
                if(fullStr[n]=='\0')
                    break;
            }
        }
        n++;
        m=0;
    }
    return false;
}

int getRecordObs(obs_record* obsr, unsigned long long rcrd){
    unsigned long long yr, mth, day, id, num;
    rcrd=rcrd<<12;
    rcrd>>=12;
    num=rcrd&65535;
    rcrd>>=16;
    id=rcrd&65535;
    rcrd>>=16;
    day=rcrd&31;
    rcrd>>=5;
    mth=rcrd&15;
    rcrd>>=4;
    yr=rcrd;
    
    obsr->day=day;
    obsr->free=0;
    obsr->id=id;
    obsr->month=mth;
    obsr->num=num;
    obsr->yr=yr;
    
    return 0;
}

void printSightingsYrWise(sighting_stats* stats, unsigned long long max){
    for(int i=0;i < max; i++){
        printf("ID: %d\n", stats[i].bid);
        printf("Bird Name: %s\n", stats[i].b_name);
        printf("-------------------------\n");
        for(int j=0;j<NUM_YRS; j++){
            printf("%20d %20llu\n", stats[i].sightings[j].yr, stats[i].sightings[j].sights);
        }
    }
}
