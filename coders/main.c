#include <unistd.h>


int main(int argc, char **argv)
{
    if (argc == 9)
    {
        
    }
    else
    {
        write(2, "Invalid Args\n", 13);
        return (1);
    }

    return (0);
}