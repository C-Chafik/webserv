#ifndef PARSECONFIG_HPP
# define PARSECONFIG_HPP

#include "../includes/includes.hpp"

class parseConfig
{
    public:

    //* Contructor - Destructor

        parseConfig( std::string path );
        ~parseConfig( void );

        bool    fill_file( void );
        int     exit_on_error( void );

        std::list<std::string> ft_split(std::string header, std::string charset );



        //* Getter

        bool state( void );

    private:
        std::list<std::string> _file;
        std::string            _file_path;

        std::string _actual_error;
        bool _state;

        parseConfig( void );
};

#endif