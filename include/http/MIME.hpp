#ifndef MIME_HPP
# define MIME_HPP
# include <iostream>
# include <string>
# include <map>

class MIME
{
    public:
        static std::string get(const std::string &file);

    private:
        MIME();
};

#endif