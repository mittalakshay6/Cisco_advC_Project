//
//  menuSystem.c
//  Adv_C_Track_Cisco
//
//  Created by Akshay Mittal on 29/06/19.
//  Copyright © 2019 Akshay Mittal. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SHOW_BIRD 1
#define FILTER_BY_KEYWORD 2
#define SIGHTINGS_BY_YR 3
#define STATS_BY_YR 4
#define YEAR_MONTH_STATS 5
#define DELETE_OBS 6
#define STATS_BY_KEYWORD 7
#define EXIT 8

#define MAX_BIRD_NAME_LEN 100

// Kindly supply the required paths below

#define OBS_FILE_PATH ..
#define BIRD_FILE_PATH ..
#define AFTER_DELETION_OBS_FILE ..
#define AFTER_DELETION_BRD_FILE ..

//**************************************//

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

void printMenu(void);
void printFinished(void);
int getChoice(void);
int getRecordObs(obs_record*, unsigned long long);
bool isLiesInYrRange(int, int, int);
void printStats(bird_stat);
int performActionOnChoice(int, FILE*, FILE*);
int actionShowBird(FILE*, int);
int actionFilterBirdsByKeyword(FILE*, char*);
int actionFilterByYr(FILE*, int);
int actionStatsByYrRange(FILE*, FILE*, int, int);
int actionStatsByYrRangeAndMonth(FILE*, FILE*, int, int, int);
int actionDeleteObservationsByIDRange(FILE*, FILE*, int, int);
int actionDeleteObservationsByYrRange(FILE*, FILE*, int, int);
int actionDeleteObservationsByMonthRange(FILE*, FILE*, int, int);
int actionStatsByBirdKeyword(FILE*, FILE*, char*);

int main(int argc, char* argv[]){
    int ch;
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
    printMenu();
    while(true){
        ch = getChoice();
        if(ch==EXIT) break;
        performActionOnChoice(ch, brd_file, obs_file);
    }
    fclose(obs_file);
    fclose(brd_file);
    return 0;
}

void printMenu(){
    printf("\n\n");
    printf("Select appropriate option from the menu: \n");
    printf("1. Show Bird - List Bird Name identified by user supplied bird ID\n");
    printf("2. Filter birds by keyword - List all birds for the supplied keyword appearing in bird name\n");
    printf("3. Sightings by year - Filter birds by supplied year\n");
    printf("4. Show Stats by year - Show bird stats by supplied year range\n");
    printf("5. Show Year/Month Stats - Show bird stats by supplied year range and month\n");
    printf("6. Delete Observations - Delete bird observations reported in the supplied bird id, year and month range\n");
    printf("7. Stats by bird keyword - Search bird by supplied keyword and list the statistics one below the other\n");
    printf("8. Exit\n++++++++++\n");
    
    return;
}

int getChoice(){
    int ch;
    scanf("%d", &ch);
    return ch;
}

int performActionOnChoice(int ch, FILE* brd_file, FILE* obs_file){
    int id;
    int yr;
    int syr, eyr;
    int month;
    int smth, emth;
    int sid, eid;
    int ch2 = 0;
    char keyword[MAX_BIRD_NAME_LEN];
    fseek(brd_file, 0, SEEK_SET);
    if(ch==SHOW_BIRD){
        printf("Bird ID?\n");
        scanf("%d", &id);
        actionShowBird(brd_file, id);
        printFinished();
    }
    else if(ch==FILTER_BY_KEYWORD){
        printf("Keyword?");
        scanf("%s", keyword);
        actionFilterBirdsByKeyword(brd_file, keyword);
        printFinished();
    }
    else if(ch==SIGHTINGS_BY_YR){
        printf("Year?");
        scanf("%d", &yr);
        actionFilterByYr(obs_file, yr);
        printFinished();
    }
    else if(ch==STATS_BY_YR){
        printf("Start Year?");
        scanf("%d", &syr);
        printf("End Year?");
        scanf("%d", &eyr);
        actionStatsByYrRange(brd_file, obs_file, syr, eyr);
        printFinished();
    }
    else if(ch==YEAR_MONTH_STATS){
        printf("Start Year?");
        scanf("%d", &syr);
        printf("End Year?");
        scanf("%d", &eyr);
        printf("Month?");
        scanf("%d", &month);
        actionStatsByYrRangeAndMonth(brd_file, obs_file, syr, eyr, month);
        printFinished();
    }
    else if(ch==DELETE_OBS){
        printf("1. Delete by ID\n2. Delete by Year\n3. Delete by Month\n");
        scanf("%d", &ch);
        if(ch2==1){
            printf("Start ID?\n");
            scanf("%d", &sid);
            printf("End ID?\n");
            scanf("%d", &eid);
            actionDeleteObservationsByIDRange(brd_file, obs_file, sid, eid);
            printFinished();
        }
        else if(ch2==2){
            printf("Start Year?\n");
            scanf("%d", &syr);
            printf("End Year?\n");
            scanf("%d", &eyr);
            actionDeleteObservationsByYrRange(brd_file, obs_file, syr, eyr);
            printFinished();
        }
        else if(ch2==3){
            printf("Start Month?\n");
            scanf("%d", &smth);
            printf("End Month?\n");
            scanf("%d", &emth);
            actionDeleteObservationsByMonthRange(brd_file, obs_file, smth, emth);
            printFinished();
        }
        else {
            printf("Invalid choice\n");
        }
    }
    else if(ch==STATS_BY_KEYWORD){
        printf("Keyword?");
        scanf("%s", keyword);
        actionStatsByBirdKeyword(brd_file, obs_file, keyword);
        printFinished();
    }
    else{
        printf("Invalid selection\n");
    }
    return 0;
}

int actionShowBird(FILE* file, int id){
    short bid;
    char len;
    bool found=false;
    char b_name[MAX_BIRD_NAME_LEN];
    
    while((fread(&bid, sizeof(bid), 1, file))==1){
        if((fread(&len, sizeof(len), 1, file))==1){
            fread(b_name, sizeof(char), len, file);
        }
        if(bid==id){
            found = true;
            b_name[len]=0;
            break;
        }
    }
    if(!found){
        printf("ID not Found\n");
        return 1;
    }
    else{
        printf("%d: %s\n", id, b_name);
        return 0;
    }
}

int actionFilterBirdsByKeyword(FILE* file, char* str){
    short bid;
    char len;
    char b_name[MAX_BIRD_NAME_LEN];
    
    while((fread(&bid, sizeof(bid), 1, file))==1){
        if((fread(&len, sizeof(len), 1, file))==1){
            fread(b_name, sizeof(char), len, file);
            b_name[len]=0;
            if((strstr(b_name, str))!=NULL){
                printf("%s\n", b_name);
            }
        }
    }
    return 0;
}

int actionFilterByYr(FILE* file, int yr){
    unsigned long long obsi;
    obs_record obsr;
    while((fread(&obsi, sizeof(obsi), 1, file))==1){
        getRecordObs(&obsr, obsi);
        if(obsr.yr == yr){
            printf("%10d\t%10d\n", obsr.id, yr);
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

int actionStatsByYrRange(FILE* brd_file, FILE* obs_file, int syr, int eyr){
    unsigned long long obsi;
    obs_record obsr;
    bird_stat stat;
    char b_name[MAX_BIRD_NAME_LEN];
    short bid;
    char len;
    bool found;
    
    while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
        getRecordObs(&obsr, obsi);
        if(isLiesInYrRange(obsr.yr, syr, eyr)){
            found=false;
            fseek(brd_file, 0, SEEK_SET);
            stat.id=obsr.id;
            stat.day=obsr.day;
            stat.month=obsr.month;
            stat.num=obsr.num;
            stat.yr=obsr.yr;
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
            if(!found){
                fprintf(stderr, "Bird ID from Observations file not found in Bird File\n");
                return 1;
            }
            printStats(stat);
        }
    }
    return 0;
}

int actionStatsByYrRangeAndMonth(FILE* brd_file, FILE* obs_file, int syr, int eyr, int month){
    unsigned long long obsi;
    obs_record obsr;
    bird_stat stat;
    char b_name[MAX_BIRD_NAME_LEN];
    short bid;
    char len;
    bool found;
    
    while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
        getRecordObs(&obsr, obsi);
        if(isLiesInYrRange(obsr.yr, syr, eyr) && obsr.month==month){
            found=false;
            fseek(brd_file, 0, SEEK_SET);
            stat.id=obsr.id;
            stat.day=obsr.day;
            stat.month=obsr.month;
            stat.num=obsr.num;
            stat.yr=obsr.yr;
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
            if(!found){
                fprintf(stderr, "Bird ID from Observations file not found in Bird File\n");
                return 1;
            }
            printStats(stat);
        }
    }
    return 0;
}

int actionDeleteObservationsByIDRange(FILE* brd_file, FILE* obs_file, int sid, int eid){
    FILE* newObsFile = fopen(AFTER_DELETION_OBS_FILE, "wb");
    FILE* newBrdFile = fopen(AFTER_DELETION_BRD_FILE, "wb");
    
    unsigned long long obsi;
    obs_record obsr;
    char b_name[MAX_BIRD_NAME_LEN];
    short bid;
    char len;
    
    while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
        getRecordObs(&obsr, obsi);
        if(!(obsr.id>=sid && obsr.id<=eid)){
            fwrite(&obsi, sizeof(obsi), 1, newObsFile);
        }
    }
    while((fread(&bid, sizeof(bid), 1, brd_file))==1){
        if((fread(&len, sizeof(len), 1, brd_file))==1){
            fread(b_name, sizeof(char), len, brd_file);
        }
        if(bid>=sid && bid<=eid){
            continue;
        }
        else{
            fwrite(&bid, sizeof(bid), 1, newBrdFile);
            fwrite(&len, sizeof(len), 1, newBrdFile);
            fwrite(&b_name, len, 1, newBrdFile);
        }
    }
    return 0;
}
int actionDeleteObservationsByYrRange(FILE* brd_file, FILE* obs_file, int syr, int eyr){
    FILE* newObsFile = fopen(AFTER_DELETION_OBS_FILE, "wb");
    FILE* newBrdFile = fopen(AFTER_DELETION_BRD_FILE, "wb");
    
    unsigned long long obsi;
    obs_record obsr;
    char b_name[MAX_BIRD_NAME_LEN];
    short bid;
    char len;
    
    while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
        getRecordObs(&obsr, obsi);
        if(!(obsr.yr>=syr && obsr.yr<=eyr)){
            fwrite(&obsi, sizeof(obsi), 1, newObsFile);
        }
    }
    while((fread(&bid, sizeof(bid), 1, brd_file))==1){
        if((fread(&len, sizeof(len), 1, brd_file))==1){
            fread(b_name, sizeof(char), len, brd_file);
        }
        if(bid>=syr && bid<=eyr){
            continue;
        }
        else{
            fwrite(&bid, sizeof(bid), 1, newBrdFile);
            fwrite(&len, sizeof(len), 1, newBrdFile);
            fwrite(&b_name, len, 1, newBrdFile);
        }
    }
    return 0;
}
int actionDeleteObservationsByMonthRange(FILE* brd_file, FILE* obs_file, int smth, int emth){
    FILE* newObsFile = fopen(AFTER_DELETION_OBS_FILE, "wb");
    FILE* newBrdFile = fopen(AFTER_DELETION_BRD_FILE, "wb");
    
    unsigned long long obsi;
    obs_record obsr;
    char b_name[MAX_BIRD_NAME_LEN];
    short bid;
    char len;
    
    while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
        getRecordObs(&obsr, obsi);
        if(!(obsr.month>=smth && obsr.month<=emth)){
            fwrite(&obsi, sizeof(obsi), 1, newObsFile);
        }
    }
    while((fread(&bid, sizeof(bid), 1, brd_file))==1){
        if((fread(&len, sizeof(len), 1, brd_file))==1){
            fread(b_name, sizeof(char), len, brd_file);
        }
        if(bid>=smth && bid<=emth){
            continue;
        }
        else{
            fwrite(&bid, sizeof(bid), 1, newBrdFile);
            fwrite(&len, sizeof(len), 1, newBrdFile);
            fwrite(&b_name, len, 1, newBrdFile);
        }
    }
    return 0;
}

int actionStatsByBirdKeyword(FILE* brd_file, FILE* obs_file, char* keyword){
    short bid;
    char len;
    char b_name[MAX_BIRD_NAME_LEN];
    bird_stat stat = {0, 0, 0 , 0 , 0, 0};
    unsigned long long obsi;
    obs_record obsr;
    bool found;
    
    fseek(brd_file, 0, SEEK_SET);
    found=false;
    while((fread(&bid, sizeof(bid), 1, brd_file))==1){
        if((fread(&len, sizeof(len), 1, brd_file))==1){
            fread(b_name, sizeof(char), len, brd_file);
            b_name[len]=0;
            if((strstr(b_name, keyword))!=NULL){
                found=true;
                stat.id=bid;
                strcpy(stat.b_name, b_name);
                fseek(obs_file, 0, SEEK_SET);
                while((fread(&obsi, sizeof(obsi), 1, obs_file))==1){
                    getRecordObs(&obsr, obsi);
                    if(obsr.id==bid){
                        stat.day=obsr.day;
                        stat.month=obsr.month;
                        stat.num=obsr.num;
                        stat.yr=obsr.yr;
                        printStats(stat);
                        break;
                    }
                }
            }
        }
    }
    if(!found){
        printf("No bird with supplied keyword found.\n");
    }
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

bool isLiesInYrRange(int yr, int syr, int eyr){
    if(yr>=syr && yr<=eyr){
        return true;
    }
    else{
        return false;
    }
}

void printFinished(void){
    printf("=================\n");
}
