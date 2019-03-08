#include<iostream>
#include<cmath>
#include <vector>
#include  <iomanip>
using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)


enum cacheResType { MISS = 0, HIT = 1 };

unsigned int m_w = 0xABABAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902;    /* must not be zero, nor 0x9068ffff */

unsigned int rand_()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGen1()
{
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE);
}

unsigned int memGen2()
{
	static unsigned int addr = 0;
	return  rand_() % (128 * 1024);
}

unsigned int memGen3()
{
	return rand_() % (DRAM_SIZE);
}

unsigned int memGen4()
{
	static unsigned int addr = 0;
	return (addr++) % (1024);
}

unsigned int memGen5()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 64);
}

unsigned int memGen6()
{
	static unsigned int addr = 0;
	return (addr += 256) % (DRAM_SIZE);
}


// Cache Simulator
cacheResType cacheSim(unsigned int addr)
{
	// This function accepts the memory address for the read and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}

char *msg[2] = { "Miss","Hit" };

//******************************************************************************************************

struct line {
	bool valid = 0;
	int tag = -1;
};

int hit1[6];
int hit2[7];

//Caches for directmapped case 1
vector<line> Cache1(16384);
vector<line> Cache2(8192);
vector<line> Cache3(4096);
vector<line> Cache4(2048);
vector<line> Cache5(1024);
vector<line> Cache6(512);

//Caches for directmapped case 2
vector<line> Cache11(64);
vector<line> Cache12(128);
vector<line> Cache13(256);
vector<line> Cache14(512);
vector<line> Cache15(1024);
vector<line> Cache16(2048);
vector<line> Cache17(4096);


void solve(vector<line>& Cache, int index, int tag, double offset_bits) {
	if (tag == Cache[index].tag) {
		hit1[int(offset_bits) - 2]++;
	}
	else {
		Cache[index].tag = tag;
	}
}

void solve2(vector<line>& Cache, int index, int tag, int size) {
	if (tag == Cache[index].tag) {
		hit2[int(log2(size/1024))]++;
	}
	else {
		Cache[index].tag = tag;
	}
}

void cache_mapped(int line_size, int cache_size, int address) {
	
	vector<line> CacheTemp;
	double offset_bits = log2(line_size);
	double index_bits = log2(cache_size / line_size);
	int index = (address / line_size) % (cache_size / line_size);
	int tag = address / pow(2.0, offset_bits + index_bits);
	
	switch (int(offset_bits)) {
	case 2:
		solve(Cache1, index, tag, offset_bits);
		break;

	case 3:
		solve(Cache2, index, tag, offset_bits);
		break;

	case 4:
		solve(Cache3, index, tag, offset_bits);
		break;

	case 5:
		solve(Cache4, index, tag, offset_bits);
		break;

	case 6:
		solve(Cache5, index, tag, offset_bits);
		break;

	case 7:
		solve(Cache6, index, tag, offset_bits);
		break;
	}

}

void cache_mapped2(int line_size, int cache_size, int address) {
	
	double offset_bits = 4.0;
	double index_bits = log2(cache_size / line_size);
	int index = (address / line_size) % (cache_size / line_size);
	int tag = address / pow(2.0, offset_bits + index_bits);

	switch (int(log2(cache_size / 1024)))
	{
	case 0:
		solve2(Cache11, index, tag, cache_size);
		break;
	case 1:
		solve2(Cache12, index, tag, cache_size);
		break;
	case 2:
		solve2(Cache13, index, tag, cache_size);
		break;
	case 3:
		solve2(Cache14, index, tag, cache_size);

		break;
	case 4:
		solve2(Cache15, index, tag, cache_size);

		break;
	case 5:
		solve2(Cache16, index, tag, cache_size);
		break;

	case 6:
		solve2(Cache17, index, tag, cache_size);
		break;

	}

}

void main() {
	unsigned int address;
	for (int i = 0; i < 1000000; i++) {
		address = memGen2();
		for (int j = 4; j <= 128; j*=2) {
			cache_mapped(j, 64 * 1024, address);
		}

		for (int k = 0; k <= 6; k++) {
			cache_mapped2(16, int(pow(2.0, k)) * 1024, address);
		}
		//cout << i << endl;
	}

	cout << endl;
	for (int i = 0; i < 6; i++) {
		cout << hit1[i]/pow(10.0,6.0) << endl;
	}
	cout << endl;
	for (int i = 0; i < 7; i++) {
		cout << hit2[i]/pow(10.0,6.0) << endl;
	}
	
	system("pause");
}


