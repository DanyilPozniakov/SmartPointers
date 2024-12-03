#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include <unordered_map>
#include <atomic>


namespace
{
    class ExternalRefCounter
    {
    public:
        std::unordered_map<void*, std::atomic<unsigned int>> ref_map;
    };
}

static ExternalRefCounter ref_counter;


//SHARED POINTER
template <class Type>
class SharedPointer
{
public:
    constexpr SharedPointer() noexcept = default;


    explicit SharedPointer(Type* ptr)
    {
        if (ref_counter.ref_map.find(ptr) != ref_counter.ref_map.end())
        {
            ++ref_counter.ref_map[ptr];
        }
        else
        {
            ref_counter.ref_map[ptr].store(1, std::memory_order_relaxed);
        }
    }

    explicit SharedPointer(const SharedPointer& other)
    {
        pointer_ = other.pointer_;
        ++ref_counter.ref_map[pointer_];
    }

    ~SharedPointer()
    {
        if (ref_counter.ref_map[pointer_].load() == 1)
        {
            delete pointer_;
            pointer_ = nullptr;
            ref_counter.ref_map.erase(pointer_);
        }
        else
        {
            --ref_counter.ref_map[pointer_];
        }
    }

    SharedPointer& operator=(const SharedPointer& other)
    {
        if (this == &other)
        {
            return *this;
        }
        pointer_ = other.pointer_;
        ++ref_counter.ref_map[pointer_];
        return *this;
    }

    Type* operator->() const
    {
        return pointer_;
    }

private:
    Type* pointer_ = nullptr;
};

std::weak_ptr<int> b;

template <class T>
class WeakPointer
{
public:
    constexpr WeakPointer() noexcept = default;

    explicit WeakPointer(const SharedPointer<T>& shared_ptr) noexcept
    {
        pointer_ = shared_ptr.pointer_;
    }

    ~WeakPointer() = default;

    WeakPointer& operator=(const WeakPointer& other)
    {
        if (this == &other)
        {
            return *this;
        }
        pointer_ = other.pointer_;
        return *this;
    }

    bool expired() const
    {
        return ref_counter.ref_map.find(pointer_) == ref_counter.ref_map.end();
    }

    T* operator->() const
    {
        return pointer_;
    }

private:
    T* pointer_ = nullptr;
};


#endif //SMARTPOINTER_H
