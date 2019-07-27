//
//  main.c
//  Adv_C_Track_Cisco
//
//  Created by Akshay Mittal on 24/06/19.
//  Copyright © 2019 Akshay Mittal. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>

struct Obs_Record{
    unsigned long long free:12;
    unsigned long long yr:11;
    unsigned long long month:4;
    unsigned long long day:5;
    unsigned long long id:16;
    unsigned long long num:16;
};

struct Stats{
    unsigned long long totalRecords;
    unsigned long long matchedRecords;
    unsigned long long unmatchedRecords;
};

typedef struct Obs_Record obs_record;
typedef struct Stats stats;

int getRecordObs(obs_record*, unsigned long long);
void printStats(stats);

int main(int argc, const char * argv[]) {
    if(argc<4){
        fprintf(stderr, "Required args: 4\nSupplied args: %d", argc);
        return 1;
    }
    
    FILE* oldFIle;
    FILE* file;
    FILE* newFile;
    const char* newName;
    obs_record obsr;
    unsigned long long obsi;
    short bid;
    char len;
    bool found;
    stats stat={0, 0, 0};
    
    newName = argv[3];
    oldFIle = fopen(argv[1], "rb");
    if(oldFIle==NULL){
        fprintf(stderr, "Failed to open file: %s", argv[1]);
        return 1;
    }
    file = fopen(argv[2], "rb");
    if(file==NULL){
        fprintf(stderr, "Failed to open file: %s", argv[2]);
        return 1;
    }
    newFile = fopen(newName, "wb");
    if(newFile==NULL){
        fprintf(stderr, "Failed to open file: %s", argv[3]);
        return 1;
    }
    printf("Sit back and relax while the new observations file is being prepared...\n");
    while((fread(&obsi, sizeof(obsi), 1, oldFIle))==1){
        getRecordObs(&obsr, obsi);
        found=false;
        fseek(file, 0, SEEK_SET);
        while((fread(&bid, sizeof(bid), 1, file))==1){
            if(bid==obsr.id){
                found=true;
                break;
            }
            if((fread(&len, sizeof(len), 1, file))==1){
                fseek(file, len, SEEK_CUR);
            }
            else break;
        }
        if(found){
            fwrite(&obsi, sizeof(obsi), 1, newFile);
        }
    }
    printf("Task Completed\n");
    fclose(oldFIle);
    fclose(file);
    fclose(newFile);
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
