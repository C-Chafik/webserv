#ifndef T_POST_HPP
# define T_POST_HPP

# include "includes.hpp"

class T_POST
{
    public:
        T_POST( const std::string & header );
        ~T_POST( void );

    private:
        std::string         _header;
        struct parsePOST    p_POST;

        void    print_all_informations( void );

        bool    get_header_informations( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
        bool    get_content( const std::string & encoding, std::list<std::string> & s_header );

        //? Data manipulation
        std::string     insertion( std::string & line, const std::string & name );
        std::string     insertion_semicolon( std::string & line, const std::string & name );

        //?  Retrieve Content, encoded, and not encoded version
        // bool            e_retrieve_content( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
        bool            retrieve_content( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
        

        T_POST( void );
};

#endif