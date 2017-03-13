#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <chrono>
#include <vector>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace chrono;

inline void createTime(char* buff)
{
	int month = (rand() % 12) + 1;
	int day = (rand() % 26) + 1;
	int hour = rand() % 24;
	int min = rand() % 60;
	int sec = rand() % 60;

	sprintf(buff, "1986.%d.%d-%d-%d-%d", month, day, hour, min, sec);
}

double calcAvg(const vector<vector<unsigned long long>>& timings, int ix)
{
	unsigned long long sum = 0;
	for(int t : timings[ix])
	{
		sum+=t;
	}

	return sum/ (double)timings[ix].size();
}

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}


unsigned long long estimate_tsc_per_sec()
{
	unsigned long long s = rdtsc();
	usleep(1000000);
	unsigned long long e = rdtsc();
	return e - s;
}

int main()
{
	char buff[256];
	struct tm t;

	const char* fmt = "%Y.%m.%d-%H-%M-%S";

	vector<vector<unsigned long long>> timings(2);
	vector<time_t> times;

	unsigned long cycles_per_sec = estimate_tsc_per_sec();
	double cycles_per_micro_sec = cycles_per_sec / (double)1000000;

	srand(time(NULL));
	for(int i=0;i<10000;i++)
	{
		createTime(buff);
		//auto start = high_resolution_clock::now();
		for(int j=0;j<1;j++)
		{
			unsigned long long s1 = rdtsc();
			strptime(buff,fmt, &t);
			unsigned long long s2 = rdtsc();
			time_t time = mktime(&t);
			unsigned long long s3 = rdtsc();

			timings[0].push_back(s2-s1);
			timings[1].push_back(s3-s2);

			times.push_back(time);
		}
		//auto end = high_resolution_clock::now();
		//auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
		//timings.push_back(microseconds.count());
	}

	double avgTime_strp = calcAvg(timings, 0);
	double avgTime_mktime = calcAvg(timings, 1);

	for(time_t ti : times)
	{
		printf("%ld\n", ti);
	}

	printf("Cycles per sec: %lu\n", cycles_per_sec);
	printf("Cycles per micro_sec: %f\n", cycles_per_micro_sec);

	printf("Avg_strp:   %f which is %f\n", avgTime_strp, avgTime_strp/cycles_per_micro_sec);
	printf("Avg_mktime: %f which is %f\n", avgTime_mktime, avgTime_mktime/cycles_per_micro_sec);
	printf("Total avg:  %f which is %f\n", (avgTime_strp+avgTime_mktime), (avgTime_strp+avgTime_mktime)/cycles_per_micro_sec);

}
