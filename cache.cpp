#include "cache.h"

Cache::Cache()
{
    reset();
    off = false;
}

// parse client interface functions

// parse commands from file stream
void Cache::parse(ifstream& infile, string command)
{
    if (command == "on") {
        off = false;
    }
    else if (command == "off")
    {
        off = true;
    }
}

// resets CLO to zeero and data to be invalid
void Cache::reset()
{
    off = true;
    clo = 0;
    for (int i = 0; i < CACHE_NUM_OF_BYTES; i++)
        states[i] = 'I';
}

// dump
void Cache::dump(ifstream& infile)
{
    printf("CLO        : 0x%02X\n", clo);  
    printf("cache data :");
    for (int i = 0; i < CACHE_NUM_OF_BYTES; i++)
        printf(" 0x%02X", data[i]);
    printf("\nFlags      :");
    for (int i = 0; i < CACHE_NUM_OF_BYTES; i++)
        printf("   %c ", states[i]);
    printf("\n\n");
}

// cache specific functions

// read word api called from cpu, will return instantly on cache hit
// and redirect to memory api call on cache miss
void Cache::memStartFetch(unsigned int offset, unsigned int count,
                            uint8_t *dataPtr, uint8_t *memDonePtr)
{
    if (!off) {
        // mark all cache data as invalid
        if (offset == 0xFF) {
            for (int i = 0; i < CACHE_NUM_OF_BYTES; i++)
                states[i] = 'I';
            *dataPtr = 0;
            *memDonePtr = true;
        }
        else if (hit(offset) && validData(offset)) {
            *dataPtr = data[getCacheIndex(offset)];
            *memDonePtr = true;
        }
        else {
            // cache miss, change CLO and fill cache
            flush(memDonePtr);
            if (flushing)
                return;     // cache needs to be flushed before filled

            loadedCLO = (offset / CACHE_NUM_OF_BYTES);
            loadedWordIdx = getCacheIndex(offset);
            memory->memStartFetch(loadedCLO * CACHE_NUM_OF_BYTES, CACHE_NUM_OF_BYTES, data, memDonePtr);
        }
    }
    else {
        // cache off, cpu fetches data memory as normal
        memory->memStartFetch(offset, count, &loadedWord, memDonePtr);
    }
}

// store word api called from cpu, will return instantly on cache hit
// and redirect to memory api call on cache miss
void Cache::memStartStore(unsigned int offset, unsigned int count,
                                uint8_t *dataPtr, uint8_t *memDonePtr)
{
    if (!off) {
        if (offset == 0xFF) {
            flush(memDonePtr);
        }
        else if (hit(offset)) {
            // cache hit, write data to cache and mark as written
            data[getCacheIndex(offset)] = *dataPtr;
            states[getCacheIndex(offset)] = 'W';
            *memDonePtr = true;
        }
        else {
            // cache miss, flush cache, set CLO, and store byte in cache
            flush(memDonePtr);
            clo = (offset / CACHE_NUM_OF_BYTES);
            loadedWordIdx = getCacheIndex(offset);
            loadedWord = *dataPtr;
            for (int i = 0; i < CACHE_NUM_OF_BYTES; i++)
                states[i] = 'I';
            states[getCacheIndex(offset)] = 'W';
        }
    }
    else {
        // cache off, cpu stores to data memory as normal
        memory->memStartStore(offset, count, dataPtr, memDonePtr);
    }
}

// used by cpu to get word after cache fetched data
uint8_t Cache::getWord()
{
    if (off)
        return loadedWord;

    clo = loadedCLO;
    for (int i = 0; i < CACHE_NUM_OF_BYTES; i++)
        states[i] = 'V';

    return data[loadedWordIdx];
}

// used by cpu to set word after cache missed on store
void Cache::setWord()
{
    if (off)
        return;

    for (int i = 0; i < CACHE_NUM_OF_BYTES; i++)
        if (loadedStates[i] == 'W')
            states[i] = 'I';

    data[loadedWordIdx] = loadedWord;
    states[loadedWordIdx] = 'W'; 
}

bool Cache::isFlushing()
{
    return flushing;
}

// register memory device
void Cache::registerMemory(Memory* newMemory)
{
    memory = newMemory;
}

// returns if the given address is a cache hit
bool Cache::hit(unsigned int addr)
{
    // computed cache line != cache line offset
    if (addr / CACHE_NUM_OF_BYTES != clo) 
        return false;

    return true;
}

// returns if given data at address in cache is valid
bool Cache::validData(unsigned int addr)
{
    // no valid data at address
    if (states[getCacheIndex(addr)] == 'I')
        return false;
    
    return true;
}

// returns index of data memory value in cache data array
unsigned int Cache::getCacheIndex(unsigned int addr)
{
    return addr - (clo * CACHE_NUM_OF_BYTES);
}

// flushes the cache
void Cache::flush(uint8_t *memDonePtr)
{
    uint8_t dataWrittenCount = 0;
    for (int i = 0; i < CACHE_NUM_OF_BYTES; i++)
        if (states[i] == 'W')
            dataWrittenCount++;

    // no data to be flushed
    if (!dataWrittenCount) {
        *memDonePtr = true;
        flushing = false;
    }
    else {
        // use special memory api to store only data flagged with 'W'
        memory->memStartCacheFlush(clo * CACHE_NUM_OF_BYTES, data, states, memDonePtr);
        flushing = true;
    }
}