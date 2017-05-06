#include "jyc_log.h"

int jyc_log_init(const char* curr_path, const char* prj_name, int log_lvl){
  int ret = 0;
  if (curr_path) {
    strncpy(jyc_log_base_path, curr_path, 8192);

    if (access(curr_path, F_OK) == -1){
      fprintf(stderr, "[jyc-log] log path is not existing (curr_path : %s)\n", curr_path);
      ret =-1;
      goto ret;
    }
  }

  if (prj_name) {
    strncpy(jyc_log_prj_name, prj_name, 8192);
  }

  jyc_log_level = log_lvl;

 ret:
  return ret;
}


int jyc_log_write(int level, const char* message_template, ...){
  int ret = 0;
  FILE* file;
  time_t cur_time;
  struct tm *ptm;
  char file_path[8192] = {0};
  char buf[8192] = {0};
  char message[8192] = {0};
  char log_tag[64] = {0};
  va_list argptr;

  cur_time = time((time_t *)0);

  ptm = localtime(&cur_time);

  if (strcmp(jyc_log_base_path, "") && strcmp(jyc_log_prj_name, ""))
  {
    snprintf(file_path, 8192, "%s/[%d-%d-%d]%s.log", jyc_log_base_path, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_year+1900, jyc_log_prj_name);
  }
  else
  {
    fprintf(stderr, "[jyc-log] A base path, and project name was not initialized...");
    ret = -1;
    goto ret;
  }

  va_start(argptr, message_template);
  vsprintf(message, message_template, argptr);
  va_end(argptr);

  if (level == JYC_LOG_DEBUG){
    sprintf(log_tag, "Dbg");
  }
  else if(level == JYC_LOG_ERROR){
    sprintf(log_tag, "Err");
  }
  else if(level == JYC_LOG_NORMAL){
    sprintf(log_tag, "Nor");
  }
  else if(level == JYC_LOG_WARNING){
    sprintf(log_tag, "War");
  }

  snprintf(buf, 8192, "[%d/%d/%d %d:%d:%d][%s] %s\n", ptm->tm_mon+1, ptm->tm_mday, ptm->tm_year+1900, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, log_tag, message);

  if (level <= jyc_log_level){
    if((file = fopen(file_path, "a+")) == NULL){
      perror("[jyc-log] fopen() log file was failed.");
      ret = -1;
      goto ret;
    }
    else{
      fputs(buf, file);
      fflush(file);
      fclose(file);
    }
  }

 ret:
  return ret;
}
