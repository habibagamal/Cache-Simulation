#include<iostream>
#include<cmath>
#include <vector>
#include  <iomanip>
#include <cstdlib> 
#include <ctime> 
#include<queue>
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
	int used = 0;
};

int hit2[4];
int hit1[5];

vector<int> lru;
queue<int> fifo;

//caches for case1
vector<line> cache11(1024/32);
vector<line> cache12(2*1024/32);
vector<line> cache13(4*1024/32);
vector<line> cache14(8*1024/32);
vector<line> cache15(16*1024/32);

//caches for case 2
vector<line> cache1(4*1024);
vector<line> cache2(4*1024);
vector<line> cache3(4*1024);
vector<line> cache4(4*1024);


void solve(vector<line>& C, int tag, int size, int lines, bool full) {

	bool hit = false;
	for (int i = 0; i < C.size(); i++) {
		if (C[i].tag == tag) {
			hit1[int(log2(size/1024))]++;
			hit = true;
			C[i].used++;
			break;
		}
	}

	if (!hit) {
		if (full) {
			int pos = rand() % lines;
			C[pos].tag = tag;
			C[pos].used = 1;
		}
		else {
			int pos;
			int temp = 0;
			do {
				if (C[temp].tag == -1) {
					pos = temp;
					break;
				}
				temp = (temp + 1) % lines;
			} while (temp != 0);
			C[temp].tag = tag;
			C[temp].used++;
		}
	}

}

void cache_fully(int line_size, int cache_size, int address, bool full) {
	int offset_bits = log2(line_size);
	int tag = address / line_size;

	switch (int (log2(cache_size/1024))) {
	case 0:
		solve(cache11, tag, cache_size, cache_size / line_size, full);
		break;

	case 1:
		solve(cache12, tag, cache_size, cache_size / line_size, full);
		break;

	case 2:
		solve(cache13, tag, cache_size, cache_size / line_size, full);
		break;

	case 3:
		solve(cache14, tag, cache_size, cache_size / line_size, full);
		break;

	case 4:
		solve(cache15, tag, cache_size, cache_size / line_size, full);
		break;
	}

}

void main() {
	unsigned int address;
	bool full = 0;
	for (int i = 0; i < 100000; i++) {
		//cout << i << endl;
		address = memGen1();
		
		for (int j = 0; j < 5; j++) {
			
			switch (j)
			{
			case 0:
				full = (i >= 32);
				break;
			case 1:
				full = (i >= 64);
				break;
			case 2:
				full = (i >= 128);
				break;
			case 3:
				full = (i >= 256);
				break;
			case 4:
				full = (i >= 512);
				break;
			default:
				break;
			}


			cache_fully(32, int(pow(2.0, j) * 1024), address, full);
		}
		
				}

	for (int i = 0; i < 4; i++) {
		cout << hit2[i] << endl;
	}
	cout << endl;
	for (int i = 0; i < 5; i++) {
		cout << hit1[i] / pow(10.0, 6.0) << endl;
	}
	system("pause");
}