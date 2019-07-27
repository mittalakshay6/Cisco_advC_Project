//
//  list_of_birds_seen_in_a_given_year.c
//  Adv_C_Track_Cisco
//
//  Created by Akshay Mittal on 29/06/19.
//  Copyright © 2019 Akshay Mittal. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BIRD_NAME_LEN 100

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

int getListOfBirdsSeenInYr(FILE*, FILE*, int);
int getRecordObs(obs_record*, unsigned long long);
void printStats(bird_stat stat);

int main(int argc, char* argv[]){
    int yr;
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
    printf("Year?\n");
    scanf("%d",&yr);
    getListOfBirdsSeenInYr(brd_file, obs_file, yr);
    return 0;
}

int getListOfBirdsSeenInYr(FILE* brd_file, FILE* obs_file, int yr){
    unsigned long long obsi;
    bird_stat stat;
    obs_record obsr;
    short bid;
    char len;
    char b_name[MAX_BIRD_NAME_LEN];
    bool found = false;
    bool found_yr=false;
    
    while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
        getRecordObs(&obsr, obsi);
        if(obsr.yr == yr){
            found_yr=true;
            stat.id=obsr.id;
            stat.day=obsr.day;
            stat.month=obsr.month;
            stat.num=obsr.num;
            stat.yr=obsr.yr;
            fseek(brd_file, 0, SEEK_SET);
            while((fread(&bid, sizeof(bid), 1, brd_file))==1){
                if((fread(&len, sizeof(len), 1, brd_file))==1){
                    fread(b_name, sizeof(char), len, brd_file);
                }
                if(bid==stat.id){
                    found=true;
                    b_name[len]=0;
                    strcpy(stat.b_name, b_name);
                    break;
                }
            }
            if(!found_yr){
                fprintf(stderr, "No bird found for the given year\n");
                return 1;
            }
            if(!found){
                fprintf(stderr, "Bird ID from Observations file not found in Bird File\n");
                return 1;
            }
            printStats(stat);
        }
    }
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
