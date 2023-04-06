#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <iostream>


namespace json{

    enum ValueType{
      NIL,
      STRING,
      INT,
      FLOAT,
      OBJECT,
      ARRAY,
      BOOL
    };

    class Value;

    typedef std::map<std::string, Value> Object;
    typedef std::vector<Value> Array;

    class Value{
    public:
        Value();
        Value(const ValueType& valTyp);
        Value(const long long& val);
        Value(const long& val);
        Value(const int& val);
        Value(const long double& val);
        Value(const double& val);
        Value(const float& val);
        Value(const std::string& val);
        Value(const char* val);
        Value(const bool& val);
        Value(const Object& val);
        Value(const Array& val);
        ~Value();

        ValueType type() const {return _valueType;}

        explicit operator long long() const;
        explicit operator long() const;
        explicit operator int() const;
        explicit operator long double() const;
        explicit operator double() const;
        explicit operator float() const;
        explicit operator bool() const;
        explicit operator std::string () const;
        operator Object () const;
        operator Array () const;

        Value& operator[] (const std::string& index);
        Value& operator[] (size_t i);

        ValueType getType() const {return _valueType;}
        size_t size() const;

        static size_t indent;
        static bool minify;

    protected:
        ValueType _valueType;

        int _Int;
        long double _Float;
        std::string _String;
        bool _Bool;
        Object _Object;
        Array _Array;
    };

    Value parse(std::string const& string, size_t& i);
}

std::ostream& operator<<(std::ostream&, const json::Value&);
std::ostream& operator<<(std::ostream&, const json::Object&);
std::ostream& operator<<(std::ostream&, const json::Array&);
