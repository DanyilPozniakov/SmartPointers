#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include <unordered_map>
#include <atomic>
#include <mutex>


class ExternalRefCounter
{
public:
    std::unordered_map<void*, std::atomic<unsigned int>> ref_map;
};

static ExternalRefCounter ref_counter;

//SHARED POINTER
template <class Type>
class SharedPointer
{
public:
    constexpr SharedPointer() noexcept = default;


    explicit SharedPointer(Type* ptr)
    {
        auto it = ref_counter.ref_map.find(ptr);

        if (it != ref_counter.ref_map.end())
        {
            ref_counter.ref_map[ptr].fetch_add(1, std::memory_order_acq_rel);
        }
        else
        {
            ref_counter.ref_map[ptr].store(1);
        }
        pointer_ = ptr;
    }

    explicit SharedPointer(const SharedPointer& other)
    {
        pointer_ = other.pointer_;
        ref_counter.ref_map[pointer_].fetch_add(1);
    }

    SharedPointer(SharedPointer&& other) noexcept
    {
        pointer_ = other.pointer_;
        other.pointer_ = nullptr;
    }

    ~SharedPointer()
    {
        if (ref_counter.ref_map[pointer_].load() == 1)
        {
            ref_counter.ref_map.erase(pointer_);
            delete pointer_;
            pointer_ = nullptr;
        }
        else
        {
            ref_counter.ref_map[pointer_].fetch_sub(1);
        }
    }

    SharedPointer& operator=(const SharedPointer& other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (pointer_ == other.pointer_)
        {
            return *this;
        }

        if (pointer_ == nullptr)
        {
            pointer_ = other.pointer_;
            ref_counter.ref_map[pointer_].fetch_add(1);
        }
        else
        {
            ref_counter.ref_map[pointer_].fetch_sub(1);
            pointer_ = other.pointer_;
            ref_counter.ref_map[pointer_].fetch_add(1);
        }
    }

    SharedPointer& operator=(SharedPointer&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        if(pointer_ == other.pointer_)
        {
            other.pointer_ = nullptr;
            return *this;
        }

        if(pointer_ == nullptr)
        {
            pointer_ = other.pointer_;
            other.pointer_ = nullptr;
        }
        else
        {
            if(ref_counter.ref_map[pointer_].fetch_sub(1) == 1)
            {
                ref_counter.ref_map.erase(pointer_);
                delete pointer_;
            }
            pointer_ = other.pointer_;
            other.pointer_ = nullptr;
        }
    }

    Type* operator->() const
    {
        return pointer_;
    }

    Type& operator*() const
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

    void reset()
    {
        if (pointer_ != nullptr, ref_counter.ref_map[pointer_].load() == 1)
        {
            delete pointer_;
            ref_counter.ref_map.erase(pointer_);
            pointer_ = nullptr;
        }
        else
        {
            ref_counter.ref_map[pointer_].fetch_sub(1);
            pointer_ = nullptr;
        }
    }

private:
    Type* pointer_ = nullptr;
};


//WEAK POINTER
template <class Type>
class WeakPointer
{
public:
    constexpr WeakPointer() = default;

    explicit WeakPointer(const SharedPointer<Type>& shared_ptr) noexcept
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

    SharedPointer<Type> lock() const
    {
        return SharedPointer<Type>(pointer_);
    }

private:
    Type* pointer_ = nullptr;
};


#endif //SMARTPOINTER_H
