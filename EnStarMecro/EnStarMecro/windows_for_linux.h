#pragma once


#include <sys/time.h>
#include <unistd.h>

typedef long DWORD;

static long timeGetTime() {
    struct timeval now;
    gettimeofday(&now, nullptr);
    return now.tv_usec/1000;
}

static void Sleep(int milliseconds){
    usleep(milliseconds * 1000);
    return;
}