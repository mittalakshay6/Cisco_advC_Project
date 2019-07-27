//
//  sightings_stats_mthwise.c
//  Adv_C_Track_Cisco
//
//  Created by Akshay Mittal on 30/06/19.
//  Copyright © 2019 Akshay Mittal. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BIRD_NAME_LEN 100
#define STATS_RECORDS_INIT_SIZE 100
#define NUM_MTHS 12
#define MAX_INPUT_LEN 8

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
struct Bird_stats{
    unsigned long long yr:11;
    unsigned long long month:4;
    unsigned long long day:5;
    unsigned long long id:16;
    unsigned long long num:16;
    char b_name[MAX_BIRD_NAME_LEN];
};
typedef struct Obs_Record obs_record;
typedef struct Bird_stats bird_stat;

int getRecordObs(obs_record*, unsigned long long);
void printStats(bird_stat stat);
bool validateDayMthYr(int, int, int, int);
bool validateMthYr(int, int, int);
bool validateYr(int, int);

int main(int argc, char* argv[]){ //argv[1] = bid, argv[2] = syr, argv[3] = eyr
    int id, syr, eyr, mth;
    char buf[MAX_INPUT_LEN];
    short bid;
    char len;
    char b_name[MAX_BIRD_NAME_LEN];
    unsigned long long num = 0;
    unsigned long long obsi;
    obs_record obsr;
    
    printf("Bird ID?\n");
    fgets(buf, MAX_INPUT_LEN, stdin);
    id=atoi(buf);
    if(id==0){
        fprintf(stderr, "Bird ID not supplied\n");
        return 1;
    }
    memset(buf, 0, MAX_INPUT_LEN);
    printf("Start Year? (Default: 1997)\n");
    fgets(buf, MAX_INPUT_LEN, stdin);
    syr = atoi(buf);
    memset(buf, 0, MAX_INPUT_LEN);
    if(syr==0){
        syr = 1997;
    }
    printf("End Year? (Default: 2018)\n");
    fgets(buf, MAX_INPUT_LEN, stdin);
    eyr = atoi(buf);
    memset(buf, 0, MAX_INPUT_LEN);
    if(eyr==0){
        eyr = 2018;
    }
    printf("Month?\n");
    fgets(buf, MAX_INPUT_LEN, stdin);
    mth = atoi(buf);
    if(mth == 0){
        fprintf(stderr, "Month not Supplied\n");
        return 1;
    }
    if(!validateMthYr(mth, syr, eyr)){
        fprintf(stderr, "Inputs out of bounds\n");
        return 1;
    }
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
    
    while((fread(&bid, sizeof(bid), 1, brd_file))==1){
        if((fread(&len, sizeof(len), 1, brd_file))==1){
            fread(b_name, sizeof(char), len, brd_file);
        }
        if(bid==id){
            b_name[len]=0;
            break;
        }
    }
    
    printf("ID: %d\n", id);
    printf("Bird Name: %s\n", b_name);
    printf("--------------------\n");
    
    while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
        getRecordObs(&obsr, obsi);
        if(obsr.id==id && obsr.yr>=syr && obsr.yr<=eyr && obsr.month==mth){
            num+=obsr.num;
        }
    }
    printf("%10llu\n", num);
    fclose(brd_file);
    fclose(obs_file);
    return 0;
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

void printStats(bird_stat stat){
    printf("ID: %d\n", stat.id);
    printf("Bird Name: %s\n", stat.b_name);
    printf("Date: %d/%d/%d\n", stat.day, stat.month, stat.yr);
    printf("Number Observed: %d\n", stat.num);
    printf("--------------------\n");
    return;
}

bool validateDayMthYr(int dd, int mm, int syr, int eyr){
    if(validateMthYr(mm, syr, eyr) && dd > 0 && dd <=31)
        return true;
    return false;
}
bool validateMthYr(int mm, int syr, int eyr){
    if(validateYr(syr, eyr) && mm>0 && mm<=12)
        return true;
    return false;
}
bool validateYr(int syr, int eyr){
    if(syr>=1997 && eyr<=2018)
        return true;
    return false;
}
