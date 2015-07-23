#include "CResult.h"

CResult::CResult():
   _type( REDIS_REPLY_NIL )
{

}

CResult::CResult(const std::string &value):
    std::string( value ),
    _type( REDIS_REPLY_NIL )
{

}

CResult::~CResult()
{

}

void CResult::setType(const ReplyType e)
{
    _type = e;
}

ReplyType CResult::getType() const
{
    return _type;
}

void CResult::clear()
{
    _type = REDIS_REPLY_NIL;
    std::string::clear();
}


string CResult::getTypeString( ReplyType e )
{
    string type;
    switch ( e )
    {
    case REDIS_REPLY_ARRAY:
        type = "REPLY_ARRY";
        break;
    case REDIS_REPLY_ERROR :
        type = "REPLY_ERROR";
        break;
    case REDIS_REPLY_INTEGERER:
        type = "REPLY_INTEGERER";
    case REDIS_REPLY_NIL :
        type = "REPLY_NIL";
        break;
    case REDIS_REPLY_STATUS :
        type = "REPLY_STATUS";
        break;
    case REDIS_REPLY_STRING :
        type = "REPLY_STRING";
        break;
    default:
        type = "REPLY_UNKNOW";
        break;
    }
    return type;
}

std::ostream &operator<<(std::ostream& out,  CResult &value)
{
    string type =CResult::getTypeString( value._type );
    out << "( type: " << type << " , value:" << std::string( value ) << " )";
    return out;
}




