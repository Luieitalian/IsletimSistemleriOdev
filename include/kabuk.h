
/**
 * BERKE PİTE
 * ENES SOYLU
 * AHMET PEKDEMİR
 * ÜZEYİR EMRE TÜRKMEN
 * HASAN YASİR ARSLAN
 */

#ifndef KABUK_H
#define KABUK_H

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 100
#define MAX_ARG_COUNT 20
#define MAX_BG_PROCESSES 100

typedef struct {
    pid_t pid;
    int running;
} BackgroundProcess;

extern BackgroundProcess bg_processes[MAX_BG_PROCESSES];
extern int bg_process_count;

#endif