#include "overviewscreen.h"
#include "ui_overviewscreen.h"

OverviewScreen::OverviewScreen(QWidget *parent)
   : QMainWindow(parent)
   , currentNormalPoints(0)
   , currentFAPoints(0)
   , currentSAPoints(0)
   , enableFAP(true)
   , enableSAP(true)
   , ui(new Ui::OverviewScreen)
{
   ui->setupUi(this);
}

OverviewScreen::~OverviewScreen()
{
   delete ui;
}

void OverviewScreen::updateValuesOnScreen() {
   updateScreenMutex.lock();
   ui->normalPoints->setText(QString::number(currentNormalPoints));
   ui->firstAutoPoints->setText(QString::number(currentFAPoints));
   ui->secondAutoPoints->setText(QString::number(currentSAPoints));
   ui->enableFAP->setChecked(enableFAP);
   ui->enableSAP->setChecked(enableSAP);
   updateScreenMutex.unlock();
}

void OverviewScreen::on_normalPoints_released()
{
   pointsMutex.lock();
   currentNormalPoints++;
   updateValuesOnScreen();
   pointsMutex.unlock();
}

void OverviewScreen::on_firstAutoPoints_released()
{
   pointsMutex.lock();
   if (currentNormalPoints >= 10) {
      currentFAPoints++;
      currentNormalPoints -= 10;
      updateValuesOnScreen();
   }
   pointsMutex.unlock();
}

void OverviewScreen::on_secondAutoPoints_released()
{
   pointsMutex.lock();
   if (currentFAPoints >= 10) {
      currentSAPoints++;
      currentFAPoints -= 10;
      updateValuesOnScreen();
   }
   pointsMutex.unlock();
}

void OverviewScreen::idleUpdateFirstAuto() {
   /*
    * Automatically 'clicks' Y times on X if Y > 0.
    * Does so once every 10 seconds or when Y > 0, whichever takes longest.
    */
   bool firstTick = true;
   while (true) {
      while (!enableFAP || currentFAPoints == 0) {
        usleep(100); //do nothing but also wait 0.1 seconds before checking again
      }
      if (firstTick == true) {
        sleep(10); //don't trigger immediately after the condition holds true for the first time
        firstTick = false;
      }
      pointsMutex.lock();
      //we are out of the while so we should have at least one FirstAutoPoint
      currentNormalPoints += currentFAPoints;
      updateValuesOnScreen();
      pointsMutex.unlock();
      sleep(10); //idle add only once every 10 seconds
   }
}

void OverviewScreen::idleUpdateSecondAuto() {
   /*
    * Uses X to automatically buy Y if Z > 0.
    * Can buy either Z or X/10 at most, whichever is lowest.
    * Does so once every 10 seconds or when X >= 10, whichever takes longest.
    */
  unsigned long long extraFAPoints;
  bool firstTick = true;

   while (true) {
      while (!enableSAP || currentSAPoints == 0) {
        usleep(100); //do nothing but also wait 0.1 seconds before checking again
      }
      if (firstTick == true) {
        sleep(10); //don't trigger immediately after the condition holds true for the first time
        firstTick = false;
      }
      pointsMutex.lock();
      //we are out of the while so we should have at least one FirstAutoPoint
      extraFAPoints = std::min(currentNormalPoints / 10, currentSAPoints);
      currentFAPoints += extraFAPoints;
      currentNormalPoints -= (extraFAPoints * 10);
      updateValuesOnScreen();
      pointsMutex.unlock();
      sleep(10); //autobuy only once every 10 seconds
   }
}

void OverviewScreen::on_enableFAP_released() {
   enableFAP = ui->enableFAP->isChecked();
}

void OverviewScreen::on_enableSAP_released() {
   enableSAP = ui->enableSAP->isChecked();
}
