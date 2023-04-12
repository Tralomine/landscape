#include "json.hpp"
#include <map>
#include <vector>
#include <string>
//pour le parser
#include <sstream>
#include <cmath>

using namespace json;
using namespace std;

size_t Value::indent = 0;
bool Value::minify = false;

Value::Value():
_valueType(NIL)
{}

Value::Value(const ValueType& valTyp):
_valueType(valTyp)
{}

Value::Value(const long long& val):
_valueType(INT),
_Int(val)
{}

Value::Value(const long& val):
_valueType(INT),
_Int(val)
{}

Value::Value(const int& val):
_valueType(INT),
_Int(val)
{}

Value::Value(const long double& val):
_valueType(FLOAT),
_Float(val)
{}

Value::Value(const double& val):
_valueType(FLOAT),
_Float(val)
{}

Value::Value(const float& val):
_valueType(FLOAT),
_Float(val)
{}

Value::Value(const string& val):
_valueType(STRING),
_String(val)
{}

Value::Value(const char* val):
_valueType(STRING),
_String(string(val))
{}

Value::Value(const bool& val):
_valueType(BOOL),
_Bool(val)
{}

Value::Value(const Object& val):
_valueType(OBJECT),
_Object(val)
{}

Value::Value(const Array& val):
_valueType(ARRAY),
_Array(val)
{}

Value::~Value()
{}

Value& Value::operator[] (const string& index)
{
  if(_valueType != OBJECT)
    throw logic_error("not a JSON object");
  return _Object[index];
}

Value& Value::operator[] (size_t i)
{
  if(_valueType != ARRAY)
    throw logic_error("not a JSON array");
  return _Array[i];
}

string valueToString(ValueType valT){
  switch(valT){
    case NIL:
      return "NIL";
      break;
    case STRING:
      return "STRING";
      break;
    case INT:
      return "INT";
      break;
    case FLOAT:
      return "FLOAT";
      break;
    case OBJECT:
      return "OBJECT";
      break;
    case ARRAY:
      return "ARRAY";
      break;
    case BOOL:
      return "BOOL";
      break;
    default:
      return "UNKNOW";
  }
}

Value::operator long long() const
{
  if(_valueType == INT)
    return _Int;
  if(_valueType == FLOAT)
    return (int)_Float;
  else throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of INT");
}

Value::operator long() const
{
  if(_valueType == INT)
    return _Int;
  if(_valueType == FLOAT)
    return (int)_Float;
  else throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of INT");
}

Value::operator int() const
{
  if(_valueType == INT)
    return _Int;
  if(_valueType == FLOAT)
    return (int)_Float;
  else throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of INT");
}

Value::operator long double() const
{
  if(_valueType == FLOAT)
    return _Float;
  if(_valueType == INT)
    return (double)_Int;
  else throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of FLOAT");
}

Value::operator double() const
{
  if(_valueType == FLOAT)
    return _Float;
  if(_valueType == INT)
    return (double)_Int;
  else throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of FLOAT");
}

Value::operator float() const
{
  if(_valueType == FLOAT)
    return _Float;
  if(_valueType == INT)
    return (double)_Int;
  else throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of FLOAT");
}

Value::operator bool() const
{
  if(_valueType != BOOL)
    throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of BOOL");
  return _Bool;
}

Value::operator std::string () const
{
  if(_valueType != STRING)
    throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of STRING");
  return _String;
}

Value::operator Object () const
{
  if(_valueType != OBJECT)
    throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of OBJECT");
  return _Object;
}

Value::operator Array () const
{
  if(_valueType != ARRAY)
    throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of ARRAY");
  return _Array;
}

size_t Value::size() const
{
  switch (_valueType) {
    case ARRAY:
      return _Array.size();
    case OBJECT:
      return _Object.size();
    default:
      throw std::logic_error("bad value type requested : " + valueToString(_valueType) + " instead of ARRAY or OBJECT");
  }
}


ostream& operator<<(ostream& flux, const Value& val)
{
    switch(val.getType()){
        case NIL:
            flux << "null";
            break;
        case STRING:
            flux << '"' << string(val) << '"';
            break;
        case INT:
            flux << (int)val;
            break;
        case FLOAT:
            flux << (long double)val;
            break;
        case OBJECT:
            flux << Object(val);
            break;
        case ARRAY:
            flux << Array(val);
            break;
        case BOOL:
            bool(val)? flux << "true":flux << "false";
            break;
    }
    return flux;
}

ostream& operator<<(ostream& flux, const Object& obj)
{
  Value::indent++;
  flux << '{';
  if(!Value::minify) flux << '\n';
  for(Object::const_iterator i(obj.cbegin());i!=obj.cend();++i){
    if(!Value::minify)
      for(size_t ind(0);ind<Value::indent;ind++){
          flux << '\t';
      }
    flux << '"' << i->first << "\":";
    if(!Value::minify) flux << ' ';
    flux << i->second;
    if(i != --obj.cend()) //si ce n'est pas le dernier
      flux << ',';
    if(!Value::minify) flux << '\n';
  }
  Value::indent--;
  if(!Value::minify)
    for(size_t ind(0);ind<Value::indent;ind++){
        flux << '\t';
    }
  flux << '}';
  return flux;
}

ostream& operator<<(ostream& flux, const Array& arr)
{
  Value::indent++;
  flux << '[';
  if(!Value::minify) flux << '\n';
  for(size_t i(0);i<arr.size();i++){
    if(!Value::minify)
      for(size_t ind(0);ind<Value::indent;ind++){
          flux << '\t';
      }
    flux << arr[i];
    if(i != arr.size()-1)   //si c'est pas le dernier
      flux << ',';
    if(!Value::minify) flux << '\n';
  }
  Value::indent--;
  if(!Value::minify)
    for(size_t ind(0);ind<Value::indent;ind++){
        flux << '\t';
    }
  flux << "]";
  return flux;
}


///=================================PARSER=================================///


void skipUselessChar(std::string const& string, size_t& i){
  i = string.find_first_not_of("\t\n\r\b\f ", i);
}

json::Value json::parse(std::string const& string, size_t& i)
{
  skipUselessChar(string, i);
  if(string[i] == '{'){  //si c'est un objet
    i++;  //on saute l'{ qu'on viens de parser
    skipUselessChar(string, i);
    json::Object object;
    if(string[i] != '}'){   //si l'objet continue
      do{
        skipUselessChar(string, i);
        if(string[i] == ',') i++; //on saute la virgule si y en a une
        skipUselessChar(string, i);
        i++; //on saute l'ouverture des guillemets
        std::string name("");
        if(string[i] == '\"') throw std::invalid_argument("malformed json exception at char " + std::to_string(i) + ": invalid value name");
        do{  //on enregistre le nom de la valeur
          name += string[i];
          i++;
        }while(string[i] != '\"');
        i++; // on saute l'autre \"
        skipUselessChar(string, i);
        if(string[i] == ':') i++; // on saute les :
        else throw std::invalid_argument("malformed json exception at char " + std::to_string(i) + ": missing ':'");
        skipUselessChar(string, i);
        object[name] = parse(string, i);    //on parse la valeur et on la stock
        skipUselessChar(string, i);
      }while(string[i] == ',');   //si ça termine par une virgule, on reprend une autre valeur
    }
    if(string[i] == '}') i++;  //on saute le }
    else throw std::invalid_argument("malformed json exception at char " + std::to_string(i) + ": missing '}' or ','");
    return json::Value(object);
  }
  else if(string[i] == '['){  //liste
    i++; //on saut le [
    skipUselessChar(string, i);
    json::Array array;
    if(string[i] != ']'){
      do {
        if(string[i] == ',') i++; //on saute la virgule si c'est pas le premier
        skipUselessChar(string, i);
        array.push_back(parse(string, i));  //on parse la valeur et on la mets dans l'array
        skipUselessChar(string, i);
      } while(string[i] == ',');
    }
    if(string[i] == ']') i++;  //on saute le ]
    else throw std::invalid_argument("malformed json exception at char " + std::to_string(i) + ": missing ']' or ','");
    return json::Value(array);
  }
  else if(std::string("-0123456789").find(string[i]) != std::string::npos){ //nombres
    std::string number("");
    do{//on récup la premiere partie du nombre
      number += string[i];
      i++;
    }while(std::string("0123456789eE+-").find(string[i]) != std::string::npos);
    if(string[i] == '.'){ //si double
      number += string[i];  //on récup la virgule
      i++;
      do{
        number += string[i];  //on recup après la virgule
        i++;
      }while(std::string("0123456789eE+-").find(string[i]) != std::string::npos);
      return json::Value(strtod(number.data(), NULL));
    }
    return json::Value((long)strtod(number.data(), NULL));
  }
  else if(string.substr(i, 4) == "null"){
      i += 4;
      return json::Value();
  }
  else if(string.substr(i, 4) == "true"){
      i += 4;
      return json::Value(true);
  }
  else if(string.substr(i, 5) == "false"){
      i += 5;
      return json::Value(false);
  }
  else if(string[i] == '\"'){ //strings
    i++;  //on saute le "
    int j(i);
    do{
      i++;
    }while(string[i] != '\"' || string[i-1] == '\\' );
    i++;  //on saute le " fermant
    return  json::Value(string.substr(j, (i-j-1)));
  }
  else{
    throw std::invalid_argument("malformed json exception at char " + std::to_string(i) + ": invalid value type");
  }

}
