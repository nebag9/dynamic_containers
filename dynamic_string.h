#include <iostream>
#include <memory>
#include <cstring>
#include "dynamic_array.h"

namespace it_step
{
    template<typename T = char>
    class dynamic_string
    {
    private:
        std::unique_ptr<T[]> _str;
        size_t _capacity;
        size_t _size;
        void swap(dynamic_string<T> & right);
    public:
        dynamic_string();
        dynamic_string(const T* value);

        dynamic_string(const dynamic_string<T>& right);
        dynamic_string(dynamic_string<T>&& right);

        dynamic_string<T>& operator=(const dynamic_string<T>& right);
        dynamic_string<T>& operator=(dynamic_string<T>&& right);

        ~dynamic_string();

        T& operator[](size_t idx);
        const T& operator[](size_t idx) const;

        size_t size();
        size_t capacity();

        dynamic_string<T> substr(size_t pos, size_t size);
        dynamic_array<dynamic_string<T>> split(T separator);

        dynamic_string<T>& operator+(const dynamic_string<T>& right);
        void operator+=(const dynamic_string<T>& right);
        bool operator==(const dynamic_string<T>& right);

        size_t size() const;
        size_t capacity() const;

        void resize(size_t newSize);
        void reserve(size_t newCapacity);
    };


    template<typename T>
    dynamic_string<T>::dynamic_string()
    {
        _str = std::make_unique<T[]>(1);
        _size = 0;
        _capacity = 1;
    }

    template<typename T>
    dynamic_string<T>::dynamic_string(const T* value)
    {
        _str = std::make_unique<T[]>(strlen(value));
        _size = strlen(value);
        _capacity = _size * 2;
        for (size_t i = 0; i < strlen(value); i++)
        {
            _str[i] = value[i];
        }   
    }

    template <typename T>
    dynamic_string<T>::dynamic_string(dynamic_string<T> const& right)
    {
        _capacity = right._capacity;
        _size = right._size;
        _str = std::make_unique<T[]>(_capacity);
        
        for (size_t i = 0; i < _size; i++)
        {
            _str[i] = right._str[i];
        } 
    }

    template <typename T>
    dynamic_string<T>::dynamic_string(dynamic_string<T>&& right)
    {
        _capacity = right._capacity;
        _size = right._size;
        _str = std::make_unique<T[]>(1);
        swap(right);
    }

    template <typename T>
    dynamic_string<T>& dynamic_string<T>::operator=(dynamic_string<T> const& right)
    {
        _str.release();
        _str = nullptr;
        _str = right._capacity ? std::make_unique<T[]>(right._capacity * 2) : nullptr;
        _size = right._size;
        _capacity = right._capacity;
        for (size_t i = 0; i < _size; i++)
        {
            _str[i] = right._str[i];
        }
        return *this;
    }

    template <typename T>
    dynamic_string<T>& dynamic_string<T>::operator=(dynamic_string<T>&& right)
    {
        _str.release();
        _size = 0;
        _capacity = 0;
        _str = nullptr;
        swap(right);
        return *this;
    }

    template <typename T>
    T& dynamic_string<T>::operator[](size_t idx){
        if (idx < _size && idx >= 0)
        {
            return _str[idx];
        }
        else
        {
            throw std::out_of_range("Out of range");
        }
    }
    template <typename T>
    const T& dynamic_string<T>::operator[](size_t idx) const{
        if (idx < _size && idx >= 0)
        {
            return _str[idx];
        }
        else
        {
            throw std::out_of_range("Out of range");
        }
    }

    template <typename T>
    size_t dynamic_string<T>::size(){ return _size;}

    template <typename T>
    size_t dynamic_string<T>::capacity(){ return _capacity;}

    template <typename T>
    size_t dynamic_string<T>::size() const { return _size;}

    template <typename T>
    size_t dynamic_string<T>::capacity() const { return _capacity;}

    template <typename T>
    dynamic_string<T> dynamic_string<T>::substr(size_t pos, size_t size)
    {   
        dynamic_string<T> ss;
        if(pos + size > _size)
        {
            int i = 0;
            while (pos!=size+1)
            {
                ss._str[i++] = _str[pos++];
            }
            ss._size = i;
            ss._capacity = ss._size * 2;
        }
        else
        {
            ss._size  = size;
            ss._capacity = size * 2;
            int j = 0;
            for (size_t i = pos; i <= size; i++)
            {
                ss._str[j++] = _str[i];
            } 
        }         
        return ss;
    }

    template <typename T>
    dynamic_array<dynamic_string<T>> dynamic_string<T>::split(T separator)
    {
        dynamic_array<dynamic_string<T>> arr;
        char c[_capacity+1];         
        int j = 0;
        for (size_t i = 0; i < _size; i++)
        {
            if(_str[i] != separator)
            {               
                c[j++]=_str[i];
            }
            else
            {
                if (j!=0)
                {
                    char s[j];
                    for (size_t k = 0; k < j; k++)
                    {
                        s[k] = c[k];
                    }
                    dynamic_string<T> temp(s);
                    arr.push_back(temp);
                    j=0;
                    temp.~dynamic_string();
                }   
            }
        }
        return arr;        
    }

    template <typename T>
    dynamic_string<T>& dynamic_string<T>::operator+(const dynamic_string<T>& right)
    {
        size_t newsize = _size + right._size;
        size_t newcapacity = _capacity + right._capacity * 2;       
        std::unique_ptr<T[]> temp = std::make_unique<T[]>(newcapacity);

        for (size_t i = 0; i < _size; i++)
        {
            temp[i] = _str[i];
        }
        int j =_size;
        for (size_t i = 0; i < right._size; i++)
        {
            temp[j++] = right._str[i];
        }
        std::swap(_str, temp);
        _size = newsize;
        _capacity = newcapacity;
             
        return *this;
    }

    template <typename T>
    void  dynamic_string<T>::operator+=(const dynamic_string<T>& right)
    {
        size_t newsize = _size + right._size;
        size_t newcapacity = _capacity + right._capacity * 2;       
        std::unique_ptr<T[]> temp = std::make_unique<T[]>(newcapacity);

        for (size_t i = 0; i < _size; i++)
        {
            temp[i] = _str[i];
        }
        int j =_size;
        for (size_t i = 0; i < right._size; i++)
        {
            temp[j++] = right._str[i];
        }
        std::swap(_str, temp);
        _size = newsize;
        _capacity = newcapacity;
    }

    template <typename T>
    bool dynamic_string<T>::operator==(const dynamic_string<T>& right)
    {
        if (_size == right._size)
        {
            for (size_t i = 0; i < _size; i++)
            {
                if (_str[i]!=right._str[i])
                {
                    return false;
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename T>
    dynamic_string<T>::~dynamic_string()
    {
        _str.release();
        _size = 0;
        _capacity = 1;
    }

    template <typename T>
    void dynamic_string<T>::swap(dynamic_string<T> &right)
    {
        std::swap(_size, right._size);
        std::swap(_capacity, right._capacity);
        std::swap(_str, right._str);
    }
    template <typename T>
    void dynamic_string<T>::resize(size_t newSize)
    {
        reserve(newSize * 2);
        _size = newSize;
    }

    template <typename T>
    void dynamic_string<T>::reserve(size_t newCapacity)
    {
        std::unique_ptr<T[]> newdata(new T[newCapacity]);
        for (size_t i = 0; i < _size; i++)
        {
            newdata[i] = _str[i];
        }
        _capacity = newCapacity;
        _str.release();
        _str = std::move(newdata);     
    }
}