#include <iostream>
#include <exception>

class FifoNotSettedException : public std::exception
{
	const char * what () const throw ()
    {
    	return "Fifo was not setted for this object. Set it before using";
    }
};