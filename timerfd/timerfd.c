#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <dirent.h>
#include <bits/pthreadtypes.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

struct timer_event_data {
	int fd;
        char *message;
};

static int epollfd = -1;
int main()
{
	int buf[2];
        struct itimerspec timer_set;
        timer_set.it_value.tv_sec = 1;
        timer_set.it_value.tv_nsec = 0;

        timer_set.it_interval.tv_sec = 1;
        timer_set.it_interval.tv_nsec = 0;

        int timer_fd;
        timer_fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
        epollfd = epoll_create(1);
        
        struct epoll_event epoll_event;
	struct timer_event_data data;
        data.message = "hello, world";
	data.fd = timer_fd;
        epoll_event.data.ptr = &data;
        epoll_event.events = EPOLLIN | EPOLLPRI;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, timer_fd, &epoll_event);
        timerfd_settime(timer_fd, 0, &timer_set, NULL);
 	
	while (1) {
		struct epoll_event events[1];
		int nevents = epoll_wait(epollfd, events, 1, -1);
		if (nevents > 0) {
			struct timer_event_data *p_data;
			p_data = events[0].data.ptr;
			read(p_data->fd, buf, sizeof(buf));
                        printf("%s\n", p_data->message);
		}
	}

        return 0;
}
