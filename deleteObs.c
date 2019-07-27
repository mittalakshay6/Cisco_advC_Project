//
//  deleteObs.c
//  Adv_C_Track_Cisco
//
//  Created by Akshay Mittal on 30/06/19.
//  Copyright © 2019 Akshay Mittal. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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
typedef struct Obs_Record obs_record;

int actionDeleteObservationsByIDRange(FILE*, int, int, int, int, int);
int getRecordObs(obs_record*, unsigned long long);
bool validateMthYr(int, int, int, int);
bool validateYr(int, int);

int main(int argc, char* argv[]){
    
    int id, syr, eyr, smth, emth;
    char buf[MAX_INPUT_LEN];
    FILE* obs_file = fopen(OBS_FILE_PATH, "rb");
    
    printf("Bird ID?\n");
    fgets(buf, MAX_INPUT_LEN, stdin);
    id=atoi(buf);
    if(id==0){
        fprintf(stderr, "Bird ID not supplied\n");
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
    memset(buf, 0, MAX_INPUT_LEN);
    eyr = atoi(buf);
    if(eyr==0){
        eyr = 2018;
    }
    printf("Start Month? (Default: 1)\n");
    fgets(buf, MAX_INPUT_LEN, stdin);
    smth = atoi(buf);
    memset(buf, 0, MAX_INPUT_LEN);
    if(smth==0){
        smth = 1;
    }
    printf("End Month? (Default: 12)\n");
    fgets(buf, MAX_INPUT_LEN, stdin);
    emth = atoi(buf);
    if(emth==0){
        emth = 12;
    }
    if(!(validateMthYr(smth, emth, syr, eyr))){
        fprintf(stderr, "Inputs out of bound");
        return 1;
    }
    actionDeleteObservationsByIDRange(obs_file, id, syr, eyr, smth, emth);
    return 0;
}

int actionDeleteObservationsByIDRange(FILE* obs_file, int id, int syr, int eyr, int smth, int emth){
    FILE* newObsFile = fopen(AFTER_DELETION_OBS_FILE, "wb");
    
    unsigned long long obsi;
    obs_record obsr;
    
    while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
        getRecordObs(&obsr, obsi);
        if(!(obsr.id==id && obsr.yr>=syr && obsr.yr<=eyr && obsr.month>=smth && obsr.month<=emth)){
            fwrite(&obsi, sizeof(obsi), 1, newObsFile);
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

bool validateMthYr(int sm, int em, int syr, int eyr){
    if(validateYr(syr, eyr) && sm>0 && em<=12)
        return true;
    return false;
}
bool validateYr(int syr, int eyr){
    if(syr>=1997 && eyr<=2018)
        return true;
    return false;
}
