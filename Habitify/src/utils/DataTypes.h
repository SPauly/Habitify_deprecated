#pragma once

#include <cstdint>
namespace Habitify
{
    template <typename T>
    class Parameter
    {
    public:
        Parameter<T>(){};
        Parameter<T>(T _t) : data(_t){};
        virtual ~Parameter<T>(){};

        inline void set(const T &_t)
        {
            data = _t;
        }

        inline T &get()
        {
            return data;
        }

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

        inline void set(const T &_t)
        {
            this->flag = _t;
        }

        inline T &get()
        {
            return flag;
        }

    private:
        T flag;
    };

    enum RELEVANCE
    {
        REQUIRED,
        OPTIONAL
    };

    enum NODE_TYPE
    {
        BOOLEAN,
        INT,
        FLOAT,
        STRING
    };

    enum NODE_TYPE_PRESENTATION
    {
        INPUT,
        SLIDER,
        ONE_LINER,
        LONG
    };
}