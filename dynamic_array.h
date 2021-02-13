#include <iostream>
#include <memory>

namespace it_step
{
    template<typename T>
    class dynamic_array
    {
    private:
        std::unique_ptr<T[]> _data;
        size_t _capacity;
        size_t _size;
        void swap(dynamic_array<T> & right);
    public:
        dynamic_array();
        dynamic_array(size_t size);
        
        dynamic_array(size_t size, const T& defaultValue);

        dynamic_array(const dynamic_array<T>& right);
        dynamic_array(dynamic_array<T>&& right);

        dynamic_array<T>& operator=(const dynamic_array<T>& right);
        dynamic_array<T>& operator=(dynamic_array<T>&& right);

        ~dynamic_array();

        T& at(size_t idx);
        const T& at(size_t idx) const;

        T& operator[](size_t idx);
        const T& operator[](size_t idx) const;

        size_t size();
        size_t capacity();

        void push_back(T&& value);
        void push_back(const T& value);


        ////// T?
        void pop_back();
        void erase(T* value);
        void erase(size_t idx);

        void clear();
        void resize(size_t newSize);
        void reserve(size_t newCapacity);

        T* begin();
        const T* begin() const;
        T* end();
        const T* end() const;
    };


    template<typename T>
    dynamic_array<T>::dynamic_array()
    {
        _data = std::make_unique<T[]>(1);
        _size = 0;
        _capacity = 1;
    }

    template <typename T>
    dynamic_array<T>::dynamic_array(size_t size)
    {
        if (size < 0)
        {
            throw std::out_of_range("leght can not be negative.");
        }
        else if (size == 0)
        {
            _data = std::make_unique<T[]>(1);
            _size = 0;
            _capacity = 1;
        }
        else
        {
            _capacity = size * 2;
            _size = size;
            _data = std::make_unique<T[]>(_capacity);
        }
    }

    template <typename T>
    dynamic_array<T>::dynamic_array(size_t size, const T& defaultValue)
    {
        if (size < 0)
        {
            throw std::out_of_range("leght can not be negative.");
        }
        else if (size == 0)
        {
            _data = std::make_unique<T[]>(1);
            _size = 0;
            _capacity = 1;
            _data[0] = 0;
        }
        else
        {
            _capacity = size * 2;
            _size = size;
            _data = std::make_unique<T[]>(_capacity);
        }
        for (size_t i = 0; i < _size; i++)
        {
            _data[i] = defaultValue;
        }  
    }
    template <typename T>
    dynamic_array<T>::dynamic_array(dynamic_array<T> const& right)
    {
        _capacity = right._capacity;
        _size = right._size;
        _data = std::make_unique<T[]>(_capacity);

        for (size_t i = 0; i < _size; i++)
        {
            _data[i] = right._data[i];
        }
    }

    template <typename T>
    dynamic_array<T>::dynamic_array(dynamic_array<T>&& right)
    {
        _capacity = right._capacity;
        _size = right._size;
        _data = std::make_unique<T[]>(1);
        swap(right);
    }

    template <typename T>
    dynamic_array<T>& dynamic_array<T>::operator=(dynamic_array<T> const& right)
    {
        _data.release();
        _data = nullptr;
        _data = right._capacity ? std::make_unique<T[]>(right._capacity * 2) : nullptr;
        _size = right._size;
        _capacity = right._capacity;
        for (size_t i = 0; i < _size; i++)
        {
            _data[i] = right._data[i];
        }
        return *this;
    }

    template <typename T>
    dynamic_array<T>& dynamic_array<T>::operator=(dynamic_array<T>&& right)
    {
        _data.release();
        _size = 0;
        _capacity = 0;
        _data = nullptr;
        swap(right);
        return *this;
    }


    template <typename T>
    T& dynamic_array<T>::operator[](size_t idx){
        if (idx < _size && idx >= 0)
        {
            return _data[idx];
        }
        else
        {
            throw std::out_of_range("Out of range");
        }
    }
    template <typename T>
    const T& dynamic_array<T>::operator[](size_t idx) const{
        if (idx < _size && idx >= 0)
        {
            return _data[idx];
        }
        else
        {
            throw std::out_of_range("Out of range");
        }
    }

    template <typename T>
    size_t dynamic_array<T>::size(){ return _size;}

    template <typename T>
    size_t dynamic_array<T>::capacity(){ return _capacity;}


    template <typename T>
    void dynamic_array<T>::push_back(T&& value)
    {   
        if(_size >= _capacity)
        {   
            reserve(_capacity * 2);
        }
        _data[_size++] = value; 
    }

    template <typename T>
    void dynamic_array<T>::push_back(const T& value)
    {
        if(_size >= _capacity)
        {
            reserve(_capacity * 2);
        }
        _data[_size++] = value; 
    }

    template <typename T>
    void dynamic_array<T>::pop_back()
    {
        _size -=1;
        _data[_size] = 0;
    }
    template <typename T>
    void dynamic_array<T>::erase(T* value)
    {   
        size_t idx = -1;
        for (size_t i = 0; i < _size; i++)
        {
            if(_data[i] == *value)
                idx = i;
        }
        
        if (idx !=-1)
        {
            std::unique_ptr<T[]> temp = std::make_unique<T[]>(_capacity);
            int j = 0;
            for (size_t i = 0; i < idx; i++)
            {
                temp[j++] = _data[i];
            }
            for (size_t i = idx+1; i < _size; i++)
            {
                temp[j++] = _data[i];
            }
            _data[idx] = 0;
            _size-=1;
            _capacity-=1;
            for (size_t i = 0; i < _size; i++)
            {
                _data[i] = temp[i];
            }
        }
        else
        {
            std::cout<<"Can't delete this element. Not found!\n"; 
        }       
    }

    template <typename T>
    void dynamic_array<T>::erase(size_t idx)
    {
        if (idx < _size && idx >= 0)
        {
            std::unique_ptr<T[]> temp = std::make_unique<T[]>(_capacity);
            int j = 0;
            for (size_t i = 0; i < idx; i++)
            {
                temp[j++] = _data[i];
            }
            for (size_t i = idx+1; i < _size; i++)
            {
                temp[j++] = _data[i];
            }
            _data[idx] = 0;
            _size-=1;
            _capacity-=1;
            for (size_t i = 0; i < _size; i++)
            {
                _data[i] = temp[i];
            }
        }
        else
        {
            throw std::out_of_range("Out of range");
        }        
    }

    template <typename T>
    void dynamic_array<T>::clear()
    {
        _data = std::make_unique<T[]>(1);
        _size = 0;
        _capacity = 1;
    }

    template <typename T>
    dynamic_array<T>::~dynamic_array()
    {
        _data.release();
        _size = 0;
        _capacity = 1;
    }

    template <typename T>
    T& dynamic_array<T>::at(size_t idx)
    {
        if (idx < _size && idx >= 0)
        {
            return _data[idx];
        }
        else
        {
            throw std::out_of_range("Out of range");
        }
    }

    template <typename T>
    const T& dynamic_array<T>::at(size_t idx) const
    {
        if (idx < _size && idx >= 0)
        {
            return _data[idx];
        }
        else
        {
            throw std::out_of_range("Out of range");
        }
    }

    template <typename T>
    T* dynamic_array<T>::begin()
    {
        return &_data[0];
    }

    template <typename T>
    const T* dynamic_array<T>::begin() const
    {
        return &_data[0];
    }

    template <typename T>
    T* dynamic_array<T>::end()
    {
        return &_data[_size];
    }

    template <typename T>
    const T* dynamic_array<T>::end() const
    {
        return &_data[_size];
    }

    template <typename T>
    void dynamic_array<T>::swap(dynamic_array<T> &right)
    {
        std::swap(_size, right._size);
        std::swap(_capacity, right._capacity);
        std::swap(_data, right._data);
    }
    template <typename T>
    void dynamic_array<T>::resize(size_t newSize)
    {
        reserve(newSize * 2);
        _size = newSize;
    }

    template <typename T>
    void dynamic_array<T>::reserve(size_t newCapacity)
    {
        std::unique_ptr<T[]> newdata(new T[newCapacity]);
        for (size_t i = 0; i < _size; i++)
        {
            newdata[i] = _data[i];
        }
        _capacity = newCapacity;
        _data.release();
        _data = std::move(newdata);     
    }
}