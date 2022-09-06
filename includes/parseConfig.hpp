#ifndef PARSECONFIG_HPP
# define PARSECONFIG_HPP

#include "../includes/includes.hpp"

class parseConfig
{
    public:

    //* Contructor - Destructor

        parseConfig( std::string path );
        ~parseConfig( void );

        std::pair<std::string, std::string> insert_port( std::string raw_address );
        bool    fill_file( void );
        void	parse_file( void );
        int     exit_on_error( void );

        std::list<std::string> ft_split(std::string header, std::string charset );
        void    remove_tab( std::string & str );
        std::list<std::string>::iterator remove_empty_line(  std::list<std::string>::iterator it );



        //* Getter

        bool state( void );

    private:
        //! Parsing
        size_t _closed;
        bool _inside;
        std::list<std::string> _file;
        std::string            _file_path;
        struct config _config;

        //! Exit Parsing
        std::string _actual_error;
        bool _state;


        parseConfig( void );
};

#endif