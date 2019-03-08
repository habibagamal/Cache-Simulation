#include<iostream>
#include<cmath>
#include<vector>
#include<ctime>
#include<cstdlib>
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
////////////////////////////////////////////////////////////////////////////////************************************

int cache1[2048 / 2][2];
int cache2[2048 / 4][4];
int cache3[2048 / 8][8];
int cache4[2048 / 16][16];

int hit[4];
bool found(int index, int tag, int way, bool& full, int &pos) {

	for (int i = 0; i < way; i++) {
		if (way == 2) {
			if (cache1[index][i] == -1) { full = 0; pos = i; }
			if (cache1[index][i] == tag) { hit[0]++; return true; }
		}
		else if (way == 4) {
			if (cache2[index][i] == -1) { full = 0; pos = i; }
			if (cache2[index][i] == tag) { hit[1]++; return true; }
		}
		else if (way == 8) {
			if (cache3[index][i] == -1) { full = 0; pos = i; }
			if (cache3[index][i] == tag) { hit[2]++; return true; }
		}
		else if (way == 16) {
			if (cache4[index][i] == -1) { full = 0; pos = i; }
			if (cache4[index][i] == tag) { hit[3]++; return true; }
		}
	
	}

	return false;
}

void cache_set(int way, int address) {
	int offset_bits = 5;
	int index_bits = log2(2048 / way);
	int sets = 2048 / way;

	int index = (address / 32) % (int(pow(2.0, double(index_bits))));
	int tag = address / (int(pow(2.0, double(5 + index_bits))));
	bool full = 1;
	int pos;
	if (!found(index, tag, way, full, pos)) {
		if (full) {
			int x = rand() % way;
			switch (way)
			{
			case 2:
				cache1[index][x] = tag;
				break;

			case 4:
				cache2[index][x] = tag;
				break;

			case 8:
				cache3[index][x] = tag;
				break;

			case 16:
				cache4[index][x] = tag;
				break;
			default:
				break;
			}
		}
		else {
			switch (way)
			{
			case 2:
				cache1[index][pos] = tag;
				break;

			case 4:
				cache2[index][pos] = tag;
				break;

			case 8:
				cache3[index][pos] = tag;
				break;

			case 16:
				cache4[index][pos] = tag;
				break;
			default:
				break;
			}
		}
	}

}


int main() {
	memset(cache1, -1,  sizeof cache1);
	memset(cache2, -1, sizeof cache2);
	memset(cache3, -1, sizeof cache3);
	memset(cache4, -1, sizeof cache4);

	unsigned address;
	memset(cache1, -1, 1024);
	for (int i = 0; i < 1000000; i++) {
		
		address = memGen1();
		
		for (int j = 2; j < 32; j *= 2) {
			cache_set(j, address);
		}
	}

	for (int i = 0; i < 4; i++) {
		cout << hit[i] << endl;
	}
	system("pause");
}
