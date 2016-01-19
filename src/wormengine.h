#ifndef WORMENGINE
#define WORMENGINE

#include <QObject>
#include <QQmlListProperty>
#include <QTimer>
#include "wormbody.h"

class WormEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<WormBody> wormBody READ wormBody NOTIFY wormBodyChanged)
    Q_PROPERTY(bool isPaused READ isPaused WRITE setisPaused NOTIFY isPausedChanged)
    Q_PROPERTY(float foodx READ foodx WRITE setFoodx NOTIFY foodxChanged)
    Q_PROPERTY(float foody READ foody WRITE setFoody NOTIFY foodyChanged)
    Q_PROPERTY(bool gameOver READ gameOver WRITE setgameOver NOTIFY gameOverChanged)
    Q_PROPERTY(bool finish READ finish WRITE setfinish NOTIFY finishChanged)
    Q_PROPERTY(bool theend READ theend WRITE settheend NOTIFY theendChanged)
    Q_PROPERTY(int apples READ apples WRITE setapples NOTIFY applesChanged)
    Q_PROPERTY(bool pauseMenu READ pauseMenu WRITE setpauseMenu NOTIFY pauseMenuChanged)

public:

   explicit WormEngine(QObject *parent = 0);
   QQmlListProperty<WormBody> wormBody();
   Q_INVOKABLE void setTouch(float tX, float tY);
   Q_INVOKABLE void newGame();
   void newFoodPos();
   bool isPaused() const;   
   float foodx() const;
   float foody() const;
   int randPosx(int lowPx, int highPx);
   int randPosy(int lowPy, int highPy);
   bool gameOver() const;   
   int apples() const;
   bool finish() const;
   bool theend() const;
   bool pauseMenu() const;   

public slots:

   void setisPaused(bool isPaused);
   void update();   
   void setFoodx(float foodx);
   void setFoody(float foody);
   void setgameOver(bool gameOver);   
   void setapples(int apples);
   void setfinish(bool finish);
   void settheend(bool theend);
   void setpauseMenu(bool pauseMenu);   

signals:

   void wormBodyChanged();   
   void isPausedChanged();   
   void foodxChanged();
   void foodyChanged();   
   void gameOverChanged();   
   void applesChanged();
   void finishChanged();
   void theendChanged();
   void pauseMenuChanged();   

private:

   QList<WormBody*> m_wormBody;
   QList<WormBody*> m_newDest;
   bool m_isPaused;
   QTimer m_updater;
   float m_tX;
   float m_tY;
   float distCounter;
   float m_foodx;
   float m_foody;
   int lowPx;
   int highPx;
   int lowPy;
   int highPy;
   float foodDist;
   float cur_headx;
   float cur_heady;
   bool m_gameOver;
   int m_apples;
   bool m_finish;
   bool m_theend;
   bool m_pauseMenu;
   int m_pagewidth;
   int m_pageheight;
   int bodyr;
};

#endif // WORMENGINE

