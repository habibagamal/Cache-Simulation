#include<iostream>
#include<cmath>
#include <vector>
#include  <iomanip>
#include <cstdlib> 
#include <ctime> 
#include<queue>
#include<queue>
#include<algorithm>
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

vector<int> cache1(4 * 1024);
vector<int> cache2(4 * 1024);
vector<int> cache3(4 * 1024);
vector<int> cache4(4 * 1024);

//vector<int> cache11(4 * 1024);
//vector<int> cache22(4 * 1024);
//vector<int> cache33(4 * 1024);
//vector<int> cache44(4 * 1024);

queue<int> Fifo;
vector<int> Used (4 * 1024);
vector<int> Lru;
vector<int> Lru_pos;

int hit2[4];

bool search(vector<int> myvector, int tag, int &pos) {
	std::vector<int>::iterator it;

	// using default comparison:
	it = std::search_n(myvector.begin(), myvector.end(), 1, tag);

	if (it != myvector.end())
	{
		pos = it - myvector.begin();
		return true;
	}
	else
		return false;
}


void random(vector<int>&C, int tag) {
	int x = rand() % C.size();
	C[x] = tag;
}

void fifo(vector<int>&C, int tag, queue<int>& F) {
	int x;
	int i;
	bool f;
	do {
	x = F.front();
	F.pop();
	f = search(C, x, i);
	} while (!f);
	C[i] = tag;
}

void used(vector<int>&C, int tag, vector<int>& U) {
	vector<int>::iterator x;
	x = min_element(U.begin(), U.end());
	int i;
	bool f = search(U, x[0], i);
	U[i] = 1;
	C[i] = tag;
}

void lru(vector<int>&C, int tag, vector<int>& Lru, vector<int>& Lru_pos) {
	for (int i = 0; i < Lru.size(); i++) {
		if (Lru[i] != -1) {
			C[Lru_pos[i]] = tag;
			Lru[i] = -1;
			Lru_pos[i] = -1;
			break;
		}
	}
}

void solve1(vector<int>& C, int tag, int cache_size, int policy, int lines ){
	int pos;
	bool found = search(C, tag, pos);

	if (found) {
		Fifo.push(tag);

		if (policy == 2)
		Used[pos] ++;

		else if (policy == 3) {

			if (!Lru.empty()) {
				int i;
				bool g = search(Lru, tag, i);
				if (g) {
					Lru[i] = -1; 
					Lru_pos[i] = -1;
				}
			}

			Lru.push_back(tag);
			Lru_pos.push_back(pos);
		}

		hit2[policy]++;
		//C[pos].used++;
	}
	else
	{
		bool spare = search(C, -1, pos);

		if (spare) {
			C[pos] = tag;
			return;
		}

		switch (policy) {
		case 0:
			random(C, tag);
			break;
		case 1:
			fifo(C, tag, Fifo);
			break;
		case 2:
			used(C, tag, Used);
			break;
		case 3:
			lru(C, tag, Lru, Lru_pos);
			break;
		}

	}

}

void cache_fully2(int line_size, int cache_size, int policy, int address) {
	int offset_bits = log2(line_size);
	int tag = address / line_size;

	switch (policy) {
	case 0:
		solve1(cache1, tag, cache_size, policy, cache_size / line_size);
		break;

	case 1:
		solve1(cache2, tag, cache_size, policy, cache_size / line_size);
		break;

	case 2:
		solve1(cache3, tag, cache_size, policy, cache_size / line_size);
		break;

	case 3:
		solve1(cache4, tag, cache_size, policy, cache_size / line_size);
		break;
	}
}


void main() {
	cache1.assign(4 * 1024, -1);
	cache2.assign(4 * 1024, -1);
	cache3.assign(4 * 1024, -1);
	cache4.assign(4 * 1024, -1);
	

	unsigned int address;
	bool full;
	for (int i = 0; i < 1000000; i++) {
		/*cout << i << endl;
		if (i == 2) {
			cout << "f\n";
		}*/
		
		for (int i = 0; i < 4; i++) {
			cache_fully2(32, 4 * 1024, i, address);
		}
		
	}
	
	for (int i = 0; i < 4; i++) {
		cout << endl << hit2[i];
	}


	system("pause");
}