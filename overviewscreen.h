#ifndef OVERVIEWSCREEN_H
#define OVERVIEWSCREEN_H

#include <QMainWindow>
#include <algorithm>
#include <future>
#include <mutex>
#include <unistd.h>

QT_BEGIN_NAMESPACE
namespace Ui { class OverviewScreen; }
QT_END_NAMESPACE

class OverviewScreen : public QMainWindow
{
    Q_OBJECT

public:
    OverviewScreen(QWidget *parent = nullptr);
    ~OverviewScreen();

    void idleUpdateFirstAuto();
    void idleUpdateSecondAuto();

private slots:
    void on_normalPoints_released();

    void on_firstAutoPoints_released();

    void on_secondAutoPoints_released();

    void on_enableFAP_released();

    void on_enableSAP_released();

private:
    void updateValuesOnScreen();

    unsigned long long currentNormalPoints;
    unsigned long long currentFAPoints; //first auto
    unsigned long long currentSAPoints; //second auto
    bool enableFAP;
    bool enableSAP;

    Ui::OverviewScreen *ui;
    std::mutex pointsMutex;
    std::mutex updateScreenMutex;
};
#endif // OVERVIEWSCREEN_H
