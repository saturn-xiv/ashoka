#include "common.h"
#include "server.h"

int main(int argc, char **argv)
{
    ashoka::ServerApplication app;
    return app.run(argc, argv);
}