#ifndef DUELENGINE
#define DUELENGINE

#include <QObject>
#include <QQmlListProperty>
#include <QTimer>
#include "wormbody.h"

class DuelEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<WormBody> wormBody READ wormBody NOTIFY wormBodyChanged)
    Q_PROPERTY(QQmlListProperty<WormBody> enemyBody READ enemyBody NOTIFY enemyBodyChanged)
    Q_PROPERTY(bool isPaused READ isPaused WRITE setisPaused NOTIFY isPausedChanged)
    Q_PROPERTY(float foodx READ foodx WRITE setFoodx NOTIFY foodxChanged)
    Q_PROPERTY(float foody READ foody WRITE setFoody NOTIFY foodyChanged)
    Q_PROPERTY(bool gameOver READ gameOver WRITE setgameOver NOTIFY gameOverChanged)
    Q_PROPERTY(bool finish READ finish WRITE setfinish NOTIFY finishChanged)
    Q_PROPERTY(bool theend READ theend WRITE settheend NOTIFY theendChanged)
    Q_PROPERTY(int playerwon READ playerwon WRITE setplayerwon NOTIFY playerwonChanged)
    Q_PROPERTY(int enemywon READ enemywon WRITE setenemywon NOTIFY enemywonChanged)
    Q_PROPERTY(int playerapples READ playerapples WRITE setplayerapples NOTIFY playerapplesChanged)
    Q_PROPERTY(int enemyapples READ enemyapples WRITE setenemyapples NOTIFY enemyapplesChanged)
    Q_PROPERTY(bool playerspoint READ playerspoint WRITE setplayerspoint NOTIFY playerspointChanged)
    Q_PROPERTY(bool enemyspoint READ enemyspoint WRITE setnnemyspoint NOTIFY enemyspointChanged)
    Q_PROPERTY(bool pauseMenu READ pauseMenu WRITE setpauseMenu NOTIFY pauseMenuChanged)

public:

   explicit DuelEngine(QObject *parent = 0);
   QQmlListProperty<WormBody> wormBody();
   QQmlListProperty<WormBody> enemyBody();
   Q_INVOKABLE void setTouch(float tX, float tY);
   Q_INVOKABLE void newGame();
   Q_INVOKABLE void resetGame();
   void newFoodPos();
   bool isPaused() const;
   float foodx() const;
   float foody() const;
   int randPosx(int lowPx, int highPx);
   int randPosy(int lowPy, int highPy);
   bool gameOver() const;
   bool finish() const;
   bool theend() const;  
   int playerwon() const;
   int enemywon() const;
   int playerapples() const;
   int enemyapples() const;
   bool playerspoint() const;
   bool enemyspoint() const;
   bool pauseMenu() const;

public slots:

   void setisPaused(bool isPaused);
   void update();
   void setFoodx(float foodx);
   void setFoody(float foody);
   void setgameOver(bool gameOver);
   void setfinish(bool finish);
   void settheend(bool theend);   
   void setplayerwon(int playerwon);
   void setenemywon(int enemywon);
   void setplayerapples(int playerapples);
   void setenemyapples(int enemyapples);
   void setplayerspoint(bool playerspoint);
   void setnnemyspoint(bool enemyspoint);
   void setpauseMenu(bool pauseMenu);

signals:

   void wormBodyChanged();
   void enemyBodyChanged();
   void isPausedChanged();
   void foodxChanged();
   void foodyChanged();
   void gameOverChanged();
   void finishChanged();
   void theendChanged();  
   void playerwonChanged();
   void enemywonChanged();
   void playerapplesChanged();
   void enemyapplesChanged();
   void playerspointChanged();
   void enemyspointChanged();
   void pauseMenuChanged();

private:

   QList<WormBody*> m_wormBody;
   QList<WormBody*> m_newDest;
   QList<WormBody*> m_enemyBody;
   QList<WormBody*> m_enemyDest;
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
   float cur_enemhx;
   float cur_enemhy;
   bool m_gameOver;
   bool m_finish;
   bool m_theend;
   int m_playerwon;
   int m_enemywon;
   bool m_omission;
   int m_omissionFrames;
   float m_omissionx;
   float m_omissiony;
   int m_playerapples;
   int m_enemyapples;
   bool m_playerspoint;
   bool m_enemyspoint;
   bool m_pauseMenu;
   int bodyr;
   int m_pagewidth;
   int m_pageheight;
};

#endif // DUELENGINE

