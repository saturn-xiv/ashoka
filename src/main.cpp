#include "application.h"

int main(int argc, char **argv)
{
    ashoka::Application app;
    try
    {
        return app.run(argc, argv);
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << boost::current_exception_diagnostic_information();
    }
}