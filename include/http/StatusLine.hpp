#ifndef STATUSLINE_HPP
# define STATUSLINE_HPP
# include <iostream>
# include <string>
# include <map>

class StatusLine
{
    public:
        static std::string get(int status_code);
    
    private:
        StatusLine();
};

#endif