//
// Created by gloomy on 2019/12/3.
//

#ifndef STORAGEBUFFERMANAGER_CACHE_H
#define STORAGEBUFFERMANAGER_CACHE_H
#include "../include/env.h"
namespace BufferManager{}
    class Cache{
    public:
    
    virtual ~Cache();
    
    virtual const char*Name();

    struct Handle{};

    virtual void Insert() = 0;

    virtual Handle * Lookup() = 0;

    virtual bool Ref(Handle* handle) = 0;

    virtual bool Release(Handle* handle, bool force_erase = false) = 0;

    virtual void* Value(Handle* handle) = 0;

    virtual void Erase() = 0;

    virtual size_t GetCapacity() const = 0;

    // returns the memory size for the entries residing in the cache.
    virtual size_t GetUsage() const = 0;

    // returns the memory size for a specific entry in the cache.
    virtual size_t GetUsage(Handle* handle) const = 0;

    // returns the memory size for the entries in use by the system
    virtual size_t GetPinnedUsage() const = 0;

    

    };

}
#endif //STORAGEBUFFERMANAGER_CACHE_H
