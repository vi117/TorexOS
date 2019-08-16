#pragma once
namespace util
{
    template<typename Mutex>
    class lock_guard
    {
        Mutex & mutex;
    public:
        explicit lock_guard(Mutex & m):mutex(m){
            mutex.lock();
        }
        lock_guard(const lock_guard &) = delete;
        ~lock_guard(){
            mutex.unlock();
        }
    };
} // namespace util
