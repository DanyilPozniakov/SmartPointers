#ifndef INTRUSIVEPTR_H
#define INTRUSIVEPTR_H

#include <atomic>
#include <type_traits>

class RefCounter
{
public:
    RefCounter() = default;

    void AddRef()
    {
        ref_count.fetch_add(1);
    }

    void Release()
    {
        if (ref_count.fetch_sub(1) == 1)
        {
            delete this;
        }
    }

#ifdef _DEBUG
    [[nodiscard]] unsigned int GetRefCount() const
    {
        return ref_count.load();
    }
#endif

    virtual ~RefCounter() = default;

private:
    std::atomic<unsigned int> ref_count = 0;
};


template <class T>
class IntrusivePtr
{
public:
    IntrusivePtr() = default;

    explicit IntrusivePtr(T* ref)
    {
        static_assert(std::is_base_of_v<RefCounter, T>, "T must be derived from RefCounter");
        if(ref == nullptr)
        {
            return;
        }

        ref_ = ref;
        ref_->AddRef();
    }

    IntrusivePtr(const IntrusivePtr& other)
    {
        static_assert(std::is_base_of_v<RefCounter, T>, "T must be derived from RefCounter");
        if (ref_)
        {
            ref_->Release();
        }
        if(other.ref_ == nullptr)
        {
            ref_ = nullptr;
            return;
        }
        ref_ = other.ref_;
        ref_->AddRef();
    }

    IntrusivePtr(IntrusivePtr&& other) noexcept
    {
        ref_ = other.ref_;
        other.ref_ = nullptr;
    }


    //TODO: other constructors...

    ~IntrusivePtr()
    {
        if (ref_)
        {
            ref_->Release();
        }
    }

    IntrusivePtr& operator=(const IntrusivePtr& other)
    {
        if (this == &other)
        {
            return *this;
        }

        if (ref_)
        {
            ref_->Release();
        }
        if(other.ref_ == nullptr)
        {
            ref_ = nullptr;
            return *this;
        }
        else
        {
            ref_ = other.ref_;
            ref_->AddRef();
        }

        return *this;
    }

    IntrusivePtr& operator=(IntrusivePtr&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        ref_ = other.ref_;
        other.ref_ = nullptr;

        return *this;
    }

    IntrusivePtr& operator=(T* ref) {

        if(ref == ref_)
        {
            return *this;
        }
        if(ref == nullptr)
        {
            if(ref_)
            {
                ref_->Release();
                ref_ = nullptr;
            }
            return *this;
        }

        if(ref_ != nullptr)
        {
            ref_->Release();
            ref_ = ref;
            ref_->AddRef();
        }
        else
        {
            ref_ = ref;
            ref_->AddRef();
        }

        return *this;
    }

    T& operator*() const
    {
        return *ref_;
    }

    T* operator->() const
    {
        return ref_;
    }

    explicit operator bool() const
    {
        return ref_ != nullptr;
    }

    [[nodiscard]] T* get() const
    {
        return ref_;
    }

    void reset()
    {
        if (ref_)
        {
            ref_->Release();
            ref_ = nullptr;
        }
    }

    void swap(IntrusivePtr& other) noexcept
    {
        if (this == &other)
        {
            return;
        }
        std::swap(ref_, other.ref_);
    }

private:
    T* ref_ = nullptr;
};


//TODO: add argument forwarding
template <class T, class... Args>
static IntrusivePtr<T> make_intrusive()
{
    return IntrusivePtr<T>(new T());
}


#endif //INTRUSIVEPTR_H
