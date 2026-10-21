#ifndef ISELECTABLE_HPP
# define ISELECTABLE_HPP
# include <string>

class ISelectable
{
    public:
        virtual ~ISelectable() {};
        virtual int getFD() const = 0;
        virtual const std::string getType() const = 0;
        virtual void onRead() = 0;
        virtual void onWrite() = 0;
        virtual bool isClosed() const = 0;
};

#endif