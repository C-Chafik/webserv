#ifndef T_POST_HPP
# define T_POST_HPP

# include "includes.hpp"

class T_POST
{
    public:
        T_POST( const std::string & header );
        ~T_POST( void );

    private:
        std::string     _header;
        struct parsePOST p_POST;


        bool    parse_POST_request( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );

        T_POST( void );
};

#endif