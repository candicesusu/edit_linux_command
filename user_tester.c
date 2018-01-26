#include <stdio.h>
#include <linux/unistd.h>
#include "prinfo.h"
#include <sys/syscall.h>

#define __NR_prinfo_ 328

int main()
{
        struct prinfo info;
        info.pid = getpid();

        syscall(__NR_prinfo_, &info);
        return 0;
}

