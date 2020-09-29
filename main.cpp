#include "overviewscreen.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OverviewScreen w;
    auto fapAsync = std::async(std::launch::async, &OverviewScreen::idleUpdateFirstAuto, &w);
    usleep(1); //to have the async threads not try to mutex at the same time
    auto sapAsync = std::async(std::launch::async, &OverviewScreen::idleUpdateSecondAuto, &w);
    w.show();
    return a.exec();
}
