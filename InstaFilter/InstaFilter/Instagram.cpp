using namespace std;
#include "Manager.h"

int main()
{
    Manager* manager = new Manager;

    manager->initialize(800, 600);

    manager->run();

    manager->finish();

    return 0;
}