# Time

https://stackoverflow.com/questions/5929419/how-to-get-file-creation-date-in-linux

https://unix.stackexchange.com/questions/50177/birth-is-empty-on-ext4/407305#407305

``` bash
ls -l --time=birth a.out
-rwxrwxr-x 1 ubuntu ubuntu 16432 Sep 17 10:47 a.out

```

> Occasionally, events conspire to turn off a computer. Sometimes, computers are even unplugged; yet, upon boot, they have the correct time. This is because most computers have a battery-powered hardware clock that stores the time and date while the computer is off. When the kernel boots, it initializes its concept of the current time from the hardware clock. Likewise, when the user shuts down the system, the kernel writes the current time back to the hardware clock. The system’s administrator may synchronize time at other points via the hwclock command.


## Time’s Data Structures

``` c
typedef long time_t;

```

> With a 32-bit long type, time_t can represent up to 2,147,483,647 seconds past the epoch. This suggests that we will have the Y2K mess all over again—in 2038! With luck, however, come 22:14:07 on Monday, 18 January 2038, most systems and software will be 64-bit.

https://en.wikipedia.org/wiki/Year_2000_problem#Similar_date_bugs

``` c
#include <sys/time.h>

struct timeval {
    time_t tv_sec;       /* seconds */
    suseconds_t tv_usec; /* microseconds */
};

```

``` c
#include <time.h>

struct timespec {
    time_t tv_sec; /* seconds */
    long tv_nsec;  /* nanoseconds */
};

```


``` c
struct tm {
	int	tm_sec;		/* seconds after the minute [0-60] */
	int	tm_min;		/* minutes after the hour [0-59] */
	int	tm_hour;	/* hours since midnight [0-23] */
	int	tm_mday;	/* day of the month [1-31] */
	int	tm_mon;		/* months since January [0-11] */
	int	tm_year;	/* years since 1900 */
	int	tm_wday;	/* days since Sunday [0-6] */
	int	tm_yday;	/* days since January 1 [0-365] */
	int	tm_isdst;	/* Daylight Savings Time flag */
#ifdef _BSD_SOURCE
	long	tm_gmtoff;	/* offset from UTC in seconds */
	char	*tm_zone;	/* timezone abbreviation */
#endif /* _BSD_SOURCE */
};

```

## POSIX Clocks

The type `clockid_t` represents a specific POSIX clock, five of which Linux supports:

- `CLOCK_REALTIME`

The system-wide real time (wall time) clock. Setting this clock requires special privileges.

- `CLOCK_MONOTONIC`

A monotonically increasing clock that is not settable by any process. It represents the elapsed time since some unspecified starting point, such as system boot.

- `CLOCK_MONOTONIC_RAW`

Similar to `CLOCK_MONOTONIC`, except the clock is not eligible for slewing (correction for clock skew). That is, if the hardware clock runs faster or slower than wall time, it won’t be adjusted when read via this clock. This clock is Linux-specific.

- `CLOCK_PROCESS_CPUTIME_ID`

A high-resolution, per-process clock available from the processor. For example, on the x86 architecture, this clock uses the timestamp counter (TSC) register.

- `CLOCK_THREAD_CPUTIME_ID`

Similar to the per-process clock, but unique to each thread in a process.

## Getting the Current Time of Day

``` c
#include <time.h>
time_t time (time_t *t);

```

> time_t’s representation of “seconds elapsed since the epoch” is not the actual number of seconds that have passed since that fateful moment in time. The Unix calculation assumes leap years are all years divisible by four and ignores leap seconds altogether. The point of the time_t representation is not that it is accurate, but that it is consistent—and it is.

``` c
#include <sys/time.h>

int gettimeofday (struct timeval *tv, struct timezone *tz);
```

> A successful call to gettimeofday() places the current time in the timeval structure pointed at by tv and returns 0. The timezone structure and the tz parameter are obsolete; neither should be used on Linux. Always pass NULL for tz.

``` c
#include <time.h>

int clock_gettime (clockid_t clock_id, struct timespec *ts);
```

> POSIX provides the clock_gettime() interface for obtaining the time of a specific time source. More useful, however, is that the function allows for nanosecond precision.

https://stackoverflow.com/questions/6498972/faster-equivalent-of-gettimeofday

https://tpaschalis.me/golang-time-now/

https://manpages.ubuntu.com/manpages/focal/en/man7/vdso.7.html

https://go-review.googlesource.com/c/go/+/69390

``` c
#include <sys/times.h>

struct tms {
    clock_t tms_utime;  /* user time */
    clock_t tms_stime;  /* system time */
    clock_t tms_cutime; /* user time of children */
    clock_t tms_cstime; /* system time of children */
};

clock_t times (struct tms *buf);
```

> The `tms_cutime` field contains the sum of the `tms_utime` and `tms_cutime` values for all waited-for terminated children. The `tms_cstime` field contains the sum of the `tms_stime` and `tms_cstime` values for all waited-for terminated children.

https://man7.org/linux/man-pages/man2/getrusage.2.html


## Setting the Current Time of Day

``` c
#include <sys/time.h>

int settimeofday (const struct timeval *tv, const struct timezone *tz);

```

> As with `gettimeofday()`, passing NULL for tz is the best practice.

``` c
#include <time.h>

int clock_settime (clockid_t clock_id, const struct timespec *ts);

```

> Not all of the system’s clocks can be changed. For instance, the CLOCK_REALTIME clock can be changed (with the appropriate privileges), but the CLOCK_MONOTONIC clock cannot.

https://www.gnu.org/software/libc/manual/html_node/Setting-and-Adjusting-the-Time.html


## Sleeping and Waiting

``` c
#include <unistd.h>

unsigned int sleep(unsigned int seconds);
```

`sleep()` causes the calling thread to sleep either until the number of real-time seconds specified in seconds have elapsed or until a signal arrives which is not ignored.

``` bash
kill -s USR1 5232
remaining: 4 secs
[1]  + 5232 done       ./a.out
```

If sleeping the entire specified time is truly a concern, you can continue calling `sleep()` with its return value until it `returns 0`:

``` c
unsigned int s = 5;

/* sleep five seconds: no ifs, ands, or buts about it */
while ((s = sleep (s)))
	;
```

> Linux deprecates the `usleep()` function, replacing it with `nanosleep()`, which provides nanosecond resolution and a smarter interface:

``` c
#define _POSIX_C_SOURCE 199309
#include <time.h>

int nanosleep (const struct timespec *req, struct timespec *rem);
```

`nanosleep()` has several advantages over `sleep()` and `usleep()`:
- Nanosecond, as opposed to second or microsecond, resolution
- Standardized by POSIX.1b
- Not implemented via signals

``` c
#include <time.h>

int clock_nanosleep (clockid_t clock_id, int flags, const struct timespec *req, struct timespec *rem);
```

https://man7.org/linux/man-pages/man2/clock_nanosleep.2.html

### A Portable Way to Sleep

> For a long time, portable Unix programs were stuck with sleep() for their naptime needs: usleep() was not widely available, and nanosleep() was as of yet unwritten. Developers discovered that passing select() 0 for n, NULL for all three of the fd_set pointers, and the desired sleep duration for timeout resulted in a portable and efficient way to put processes to sleep:

``` c
struct timeval tv = { .tv_sec = 0,
                      .tv_usec = 757 };

/* sleep for 757 us */
select (0, NULL, NULL, NULL, &tv);

```

### Alternatives to Sleeping

> If possible, you should avoid sleeping. Often, you cannot, and that’s fine—particularly if your code is sleeping for less than a second. Code that is laced with sleeps in order to “busy-wait” for events is usually of poor design. Code that blocks on a file descriptor, allowing the kernel to handle the sleep and wake up the process, is better. Instead of the process spinning in a loop until the event hits, the kernel can block the process from execution and wake it up only when needed.


## Timers

``` c
#include <unistd.h>

unsigned int alarm (unsigned int seconds);

```

``` c
#include <sys/time.h>

#define ITIMER_REAL      0
#define ITIMER_VIRTUAL   1
#define ITIMER_PROF      2

int
getitimer(int which, struct itimerval *value);

int
setitimer(int which, const struct itimerval *restrict value, struct itimerval *restrict ovalue);
```

> Some Unix systems implement `sleep()` and `usleep()` via SIGALRM. `alarm()` and `setitimer()` also use SIGALRM. Therefore, programmers must be careful not to overlap calls to these functions; the results are undefined. For the purpose of brief waits, programmers should use `nanosleep()`, which POSIX dictates will not use signals. For timers, programmers should use `setitimer()` or `alarm()`.

``` bash
$ strace -e clock_nanosleep  sleep 10s
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=10, tv_nsec=0}, 0x7ffd2c2419d0) = 0
+++ exited with 0 +++

```

``` c
#include <signal.h>           /* Definition of SIGEV_* constants */
#include <time.h>

int timer_create(clockid_t clockid,
				struct sigevent *_Nullable restrict sevp,
				timer_t *restrict timerid);
```

https://man7.org/linux/man-pages/man2/timer_create.2.html

> Timers are not inherited by the child of a fork(2), and are disarmed and deleted during an execve(2).


https://blog.gopheracademy.com/advent-2016/go-timers/
https://topic.alibabacloud.com/a/implementation-of-nginx-timer-and-usage-of-scheduled-events_8_8_31813369.html
https://github.com/golang/go/issues/14410#issuecomment-277413169
https://github.com/golang/go/issues/25471


``` c
#include <sys/timerfd.h>

int timerfd_create(int clockid, int flags);

int timerfd_settime(int fd, int flags,
					const struct itimerspec *new_value,
					struct itimerspec *_Nullable old_value);
int timerfd_gettime(int fd, struct itimerspec *curr_value);
```

https://man7.org/linux/man-pages/man2/timerfd_create.2.html

