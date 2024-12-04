#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include <unordered_map>
#include <atomic>


class ExternalRefCounter
{
public:
    std::unordered_map<void*, std::atomic<unsigned int>> ref_map;
};

static ExternalRefCounter ref_counter;


/*
 *  SMART POINTER
 *  This class is a simple implementation of a smart pointer.
 */

//SHARED POINTER
template <class Type>
class smart_pointers
{
public:
    constexpr smart_pointers() noexcept = default;


    explicit smart_pointers(Type* ptr)
    {
        auto it = ref_counter.ref_map.find(ptr);

        if (it != ref_counter.ref_map.end())
        {
            ++ref_counter.ref_map[ptr];
        }
        else
        {
            ref_counter.ref_map[ptr].store(1, std::memory_order_release);
        }
    }

    explicit smart_pointers(const smart_pointers& other)
    {
        pointer_ = other.pointer_;
        ++ref_counter.ref_map[pointer_];
    }

    ~smart_pointers()
    {
        if (ref_counter.ref_map[pointer_].load(std::memory_order_acquire) == 1)
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

    smart_pointers& operator=(const smart_pointers& other)
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

    Type operator*() const
    {
        return *pointer_;
    }

    explicit operator bool() const
    {
        return pointer_ != nullptr;
    }

    Type operator[](int index) const
    {
        return pointer_[index];
    }

    Type* get() const
    {
        return pointer_;
    }

    [[nodiscard]] bool unique() const
    {
        return ref_counter.ref_map[pointer_].load() == 1;
    }

    [[nodiscard]] std::size_t use_count() const
    {
        return ref_counter.ref_map[pointer_].load();
    }

private:
    Type* pointer_ = nullptr;
};


//WEAK POINTER ---->
template <class Type>
class WeakPointer
{
public:
    constexpr WeakPointer() = default;

    explicit WeakPointer(const smart_pointers<Type>& shared_ptr) noexcept
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

    Type* operator->() const noexcept
    {
        return pointer_;
    }

    Type operator*() const
    {
        return *pointer_;
    }

    explicit operator bool() const
    {
        return pointer_ != nullptr;
    }

    [[nodiscard]] bool expired() const
    {
        return ref_counter.ref_map.find(pointer_) == ref_counter.ref_map.end();
    }

    [[nodiscard]] std::size_t use_count() const
    {
        return ref_counter.ref_map[pointer_].load();
    }

    smart_pointers<Type> lock() const
    {
        return smart_pointers<Type>(pointer_);
    }

private:
    Type* pointer_ = nullptr;
};


#endif //SMARTPOINTER_H
