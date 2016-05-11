#include "CResult.h"
#include <sstream>
#include "RdException.h"
namespace Taiji {
namespace Redis {



CResult::CResult():
   _type( REDIS_REPLY_NIL )
{
    _arry.clear();
}

CResult::CResult( const CResult& other ):
    std::string( other ),
    _type( other._type ),
  _arry( other._arry )
{
}

CResult::CResult(const std::string &value):
    std::string( value ),
    _type( REDIS_REPLY_NIL )
{
    _arry.clear();
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

bool CResult::addElement(const CResult &ele)
{
    if ( _type != REDIS_REPLY_ARRAY )
    {
        return false;
    }

    _arry.push_back( ele );
    return true;
}

const CResult::ListCResult &CResult::getArry( void ) const
{
    if ( _type != REDIS_REPLY_ARRAY )
    {
        throw ExceptType( "Data is not arry type" );
    }
    return _arry;
}

int64_t CResult::getInt(void) const
{
    if ( _type != REDIS_REPLY_INTEGERER )
    {
         throw ExceptType( "Data is not int type" );
    }

    int64_t value;
    std::istringstream istr( std::string(*this) );
    istr >> value;

    if ( istr.fail() )
    {
         throw ExceptType( "Data is not int type" );
    }

    return value;
}

string CResult::getString( void ) const
{
    if ( _type != REDIS_REPLY_STRING )
    {
         throw ExceptType( "Data is not string type" );
    }
    return *this;
}

string CResult::getErrorString( void ) const
{
    if ( _type != REDIS_REPLY_ERROR )
    {
         throw ExceptType( "Data is not error type" );
    }
    return *this;
}

string CResult::getStatus( void ) const
{
    if ( _type != REDIS_REPLY_STATUS )
    {
         throw ExceptType( "Data is not error type" );
    }
    return *this;
}


std::string CResult::display( const CResult &ele, int indent )
{
    ReplyType e = ele.getType( );
    string type =CResult::getTypeString( e );
    std::stringstream out;
    if ( REDIS_REPLY_ARRAY == e )
    {
       CResult::ListCResult::const_iterator it = ele.getArry().begin() ;
        indent += 3;

        out <<"{\n";
        for ( int i = 0; i < indent; i++ )
            out << " ";

        indent += 3;
        out << "type:" << type <<", value:[" << std::endl;
        for ( ; it!=ele.getArry().end(); it++ )
        {
            for ( int i = 0; i < indent; i++ )
                out << " ";

            out << display( *it, indent )  << std::endl;
        }

        for ( int i = 0; i < indent-3; i++ )
            out << " ";
        out << "]\n";

        for ( int i = 0; i < indent-6; i++ )
            out << " ";
        out << "};";
    }else
    {
        out << "{type: " << type << " ,value: " << std::string( ele ) << "};";
    }
    return out.str() ;
}


void CResult::assign( const CResult &other)
{
    if ( this == &other )
    {
        return;
    }

    _type = other._type;
    string::assign( other );
    _arry = other._arry;
    return;
}

CResult &CResult::operator=( const std::string &value )
{
    if ( this == &value )
    {
        return *this;
    }

    string::assign( value );
    return *this;
}

CResult &CResult::operator=( const CResult &other)
{
    assign( other );
    return *this;
}

void CResult::clear()
{
    _type = REDIS_REPLY_NIL;
    std::string::clear();
    _arry.clear();
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
        break;
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



std::ostream &operator<<(std::ostream& out,  const CResult &value)
{
    string str = CResult::display( value, 0 );
    out << str;
    return out ;
}




}
}
