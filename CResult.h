#ifndef CRESULT_H
#define CRESULT_H

#include "common.h"


typedef enum ReplyType
{
    REDIS_REPLY_STRING,
    REDIS_REPLY_ARRAY,
    REDIS_REPLY_INTEGERER,
    REDIS_REPLY_NIL,
    REDIS_REPLY_STATUS,
    REDIS_REPLY_ERROR
} ReplyType;

class CResult : public std::string
{
public:

    CResult();

    CResult( const CResult& other );

    CResult( const string& value );

    ~CResult();

    void setType( const ReplyType e );

    ReplyType getType( void ) const ;


    void assign( CResult& other );

    CResult& operator= ( CResult& other );

    void clear();
    /**
     * @brief getTypeString
     * @param e
     * @return get a string to discribe a type of CResult.
     */
    static string getTypeString(ReplyType e);


    friend std::ostream &operator<<(std::ostream& out,  CResult &value);
private:
    ReplyType _type;
};

#endif // CRESULT_H
