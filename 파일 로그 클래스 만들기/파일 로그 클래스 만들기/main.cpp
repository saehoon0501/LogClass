#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <iostream>
#include <stdlib.h>

//로그 레벨에 따른 값들 정의
#define LOG_LV_TRACE 50
#define LOG_LV_DEBUG 40
#define LOG_LV_INFO 30
#define LOG_LV_WARNING 20
#define LOG_LV_ERROR 10
#define LOG_LV_FATAL 0

using namespace std;

class LogFile{
public:
    FILE *file; //LogFile
    char filename[1024];
    
    LogFile(){}// 생성자
    ~LogFile(){} // 소멸자
    
    void logFileCreate(){ //로그 내용을 담을 파일 이름 생성
        char tempName[1024];
        struct timeval tv;
       struct tm *tm1;
        static pid_t pid = -1;
        gettimeofday(&tv, NULL);
       tm1 = localtime(&tv.tv_sec);
        if(pid == -1) { pid = getpid(); }
        
        snprintf(filename, 1024, "%s-%04d%02d%02d.log", __FILE__,1900 + tm1->tm_year, tm1->tm_mon+1,tm1->tm_mday);
        
        if (this->file = fopen(filename, "r")) {//기존에 같은 log파일 존재하면 대체
            fclose(file);
            remove(filename);
            file == NULL;
        }
        
        for (int i = 1; i < 12; i++) {//같은 해 한달 이후 파일들은 전부 삭제된다.
            if (tm1->tm_mon+1 - i > 0) {
                for (int j =0; j <30; j++) {
                        snprintf(tempName, 1024, "%s-%04d%02d%02d.log", __FILE__,1900 + tm1->tm_year, tm1->tm_mon+1-i,j);
                        if (this->file = fopen(tempName, "r")) {//기존에 같은 log파일 존재하면 대체
                            fclose(file);
                            remove(tempName);
                            file == NULL;
                    }
                }
            }
        }
        
    }
    
    void logFileWrite(char log_type, char message[]){ //생성된 파일에 log내용 작성
        struct timeval tv;
       struct tm *tm1;
        static pid_t pid = -1;
        gettimeofday(&tv, NULL);
       tm1 = localtime(&tv.tv_sec);
        if(pid == -1) { pid = getpid(); }
        
        this->file = fopen(filename, "a");
        fprintf(file, "(%c) ", log_type);
        fprintf(file, "%04d년%02d월%02d일:%02d시%02d분%02d%02ld초:", 1900 + tm1->tm_year, tm1->tm_mon + 1, tm1->tm_mday, tm1->tm_hour, tm1->tm_min, tm1->tm_sec, tv.tv_usec/10000, pid);
        fprintf(file, "%s[소스파일]:(%d)[라인]", __FILE__, __LINE__);
        fprintf(file, ":\t\t%s\n ", message);
        fclose(file);
    }
    
};

class Log{
private:
    //로그를 출력하는 기능
    void logPrint(char log_type, char message[]){
     struct timeval tv;
    struct tm *tm1;
    static pid_t pid = -1;
     gettimeofday(&tv, NULL);
    tm1 = localtime(&tv.tv_sec);
    if(pid == -1) { pid = getpid(); }
        
    printf("(%c) ", log_type);
    printf("%04d년%02d월%02d일:%02d시%02d분%02d%02ld초:", 1900 + tm1->tm_year, tm1->tm_mon + 1, tm1->tm_mday, tm1->tm_hour, tm1->tm_min, tm1->tm_sec, tv.tv_usec/10000, pid);
    printf("%s[소스파일]:(%d)[라인]", __FILE__, __LINE__);
    printf(":\t\t%s\n ", message);
    }
    
    //로그 레벨 확인
    bool isTrace(){
        return log_LV >= LOG_LV_TRACE;
    }
    
    bool isDebug(){
        return log_LV >= LOG_LV_DEBUG;
    }
    
    bool isInfo(){
        return log_LV >= LOG_LV_INFO;
    }
    
    bool isWarning(){
        return log_LV >= LOG_LV_WARNING;
    }
    
    bool isError(){
        return log_LV >= LOG_LV_ERROR;
    }
    
    bool isFatal(){
        return log_LV >= LOG_LV_FATAL;
    }
    
public:
    LogFile file;
    static int log_LV; //로그 레벨 field
    static int mode; // 모드 설정 값

    //생성자
    Log(){this->file = LogFile();}
    
    //소멸자
    ~Log(){}
    
    //로그 레벨 출력
    void trace(char message[]){
        if (isTrace()) {
            if (mode == 0) {
                logPrint('T', message);
            }else if(mode == 1){
                file.logFileWrite('T', message);
            }else if(mode == 2){
                logPrint('T', message);
                file.logFileWrite('T', message);
            }
        }else{
            return;
        }
    }
    
    void debug(char message[]){
        if (isDebug()) {
            if (mode == 0) {
                logPrint('D', message);
            }else if(mode == 1){
                file.logFileWrite('D', message);
            }else if(mode == 2){
                logPrint('D', message);
                file.logFileWrite('D', message);
            }
        }else{
            return;
        }
    }
    
    void info(char message[]){
        if (isInfo()) {
            if (mode == 0) {
                logPrint('I', message);
            }else if(mode == 1){
                file.logFileWrite('I', message);
            }else if(mode == 2){
                logPrint('I', message);
                file.logFileWrite('I', message);
            }
        }else{
            return;
        }
    }
    
    void warning(char message[]){
        if (isWarning()) {
            if (mode == 0) {
                logPrint('W', message);
            }else if(mode == 1){
                file.logFileWrite('W', message);
            }else if(mode == 2){
                logPrint('W', message);
                file.logFileWrite('W', message);
            }
        }else{
            return;
        }
    }
    
    void error(char message[]){
        if (isError()) {
            if (mode == 0) {
                logPrint('E', message);
            }else if(mode == 1){
                file.logFileWrite('E', message);
            }else if(mode == 2){
                logPrint('E', message);
                file.logFileWrite('E', message);
        }
        }else{
            return;
        }
    }
    
    void fatal(char message[]){
        if (isFatal()) {
            if (mode == 0) {
                logPrint('F', message);
            }else if(mode == 1){
                file.logFileWrite('F', message);
            }else if(mode == 2){
                logPrint('F', message);
                file.logFileWrite('F', message);
            }
        }else{
            return;
        }
    }
    
    //로그 레벨과 모드 설정
    void basicConfig(char mode[], char log_Lv[]){
        if (strcmp("print", mode)==0) {
            this->mode = 0;
        }else if(strcmp("file", mode)==0){
            this->mode = 1;
        }else if(strcmp("both", mode)==0){
            this->mode = 2;
        }else{
            cout << "Keyword: print, file, both 3가지중 하나를 입력하세요" <<endl;
        }
        
        if (strcmp("TRACE", log_Lv)==0) {
            this->log_LV = 50;
        }else if(strcmp("DEBUG", log_Lv)==0) {
            this->log_LV = 40;
        }else if(strcmp("INFO", log_Lv)==0) {
            this->log_LV = 30;
        }else if(strcmp("WARNING", log_Lv)==0) {
            this->log_LV = 20;
        }else if(strcmp("ERROR", log_Lv)==0) {
            this->log_LV = 10;
        }else if(strcmp("FATAL", log_Lv)==0) {
            this->log_LV = 0;
        }else{
            cout <<"log_Lv : TRACE, DEBUG, INFO, WARNING, ERROR, FATAL" <<endl;
        }
        return;
    }
    
    
};


int Log::mode = 0;
int Log::log_LV = 30;

int main(int argc, char* argv[]){
    Log logger = Log();
    logger.file.logFileCreate(); // 먼저 Log가 저장될 파일과 파일 날짜 업데이트를 해준다.
    logger.basicConfig("print", "WARNING"); // Mode 파일 출력, Warning level보다 위에 존재하는 TRACE,DEBUG,INFO는 출력,파일 생성 불가
    logger.trace("ahOOH!");
    logger.debug("Haaha!");
    logger.info("Haaha!");
    logger.warning("complete");
    logger.error("error occured!!");
    logger.fatal("critical error, emergency protocol initiated");
    
    logger.basicConfig("file", "WARNING");
    logger.trace("ahOOH!");
    logger.debug("Haaha!");
    logger.info("Haaha!");
    logger.warning("complete");
    logger.error("error occured!!");
    logger.fatal("critical error, emergency protocol initiated");
}
