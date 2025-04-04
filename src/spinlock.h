#ifndef __SPINLOCK_H_
#define __SPINLOCK_H_

#include <atomic>
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

struct spinlock {
    std::atomic<bool> lock_ = { 0 };

    inline void lock() noexcept {
        for (;;) {
            // Optimistically assume the lock is free on the first try
            if (!lock_.exchange(true, std::memory_order_acquire)) {
                return;
            }
            // Wait for lock to be released without generating cache misses
            while (lock_.load(std::memory_order_relaxed)) {
                // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
                // hyper-threads
#ifdef _MSC_VER
                _mm_pause();
#else
                __builtin_ia32_pause();
#endif
            }
        }
    }

    inline bool try_lock() noexcept {
        // First do a relaxed load to check if lock is free in order to prevent
        // unnecessary cache misses if someone does while(!try_lock())
        return !lock_.load(std::memory_order_relaxed) &&
            !lock_.exchange(true, std::memory_order_acquire);
    }

    inline void unlock() noexcept {
        lock_.store(false, std::memory_order_release);
    }
};


#endif