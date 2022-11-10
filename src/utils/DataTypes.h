#pragma once

namespace Habitify
{
    template <typename T>
    class Parameter
    {
    public:
        Parameter<T>();
        virtual ~Parameter<T>();

    private:
        T data;
    };

    template <typename T>
    class FLAG
    {
    public:
        FLAG<T>(){};
        FLAG<T>(T _t) : flag(_t){};
        virtual ~FLAG<T>(){};

        inline void set(T &_t)
        {
            flag = _t;
        }

        inline T& get()
        {
            return flag;
        }

    private:
        T flag;
    };
}