#include <QtMath>
#include <QScreen>
#include <QGuiApplication>
#include "wormbody.h"
#include "wormengine.h"

WormEngine::WormEngine(QObject *parent) :
    QObject(parent)
{
    QScreen* screen = QGuiApplication::primaryScreen();
    m_pagewidth = screen->size().width();
    m_pageheight = screen->size().height();

    m_isPaused = true;
    m_gameOver = false;
    m_finish = false;
    m_theend = false;
    m_pauseMenu = false;
    bodyr = m_pagewidth/13.5;

    for (int i = 0; i < 6; i++) {
        WormBody *wormBody = new WormBody((m_pagewidth/3.375)+i*(m_pagewidth/10.8), m_pageheight/2.4);
        m_wormBody.append(wormBody);
    }

    m_updater.setInterval(50);
    connect(&m_updater, SIGNAL(timeout()), this, SLOT(update()));
    m_updater.start();
    lowPx = m_pagewidth/20;
    highPx = m_pagewidth/1.04;
    lowPy = m_pageheight/48;
    highPy = m_pageheight/1.3;
    m_apples = 0;
    distCounter = 0;
    newFoodPos();
}

QQmlListProperty<WormBody> WormEngine::wormBody()
{
    return QQmlListProperty<WormBody>(this, m_wormBody);
}

void WormEngine::setTouch(float tX, float tY)
{
    // takes touch coords from QML
    m_tX = tX;
    m_tY = tY;
}

void WormEngine::update()
{
    if(!m_isPaused) {

        float cur_len, len, newdx, newdy;

        // resets worms body movement counter to zero
        if (distCounter == 5) {
            distCounter = 0;
            m_newDest.clear();
        }

        // stores worms head pos at the beginning of every update
        cur_headx = m_wormBody.at(0)->px();
        cur_heady = m_wormBody.at(0)->py();

        // calcs distance from touch to head
        len = qSqrt((cur_headx-m_tX)*(cur_headx-m_tX)+(cur_heady-m_tY)*(cur_heady-m_tY));

        // sets the speed of worm
        cur_len = len/(bodyr/3.08);
        newdx = (m_tX-cur_headx)/cur_len;
        newdy = (m_tY-cur_heady)/cur_len;

        WormBody *wormBody = new WormBody(cur_headx+newdx, cur_heady+newdy);
        m_wormBody.replace(0, wormBody);

        // stops the the end animation
        if (m_wormBody.count() <= 2) {
            m_isPaused = true;
            newGame();
        }

        // loops worms body parts from next after the head to the tail
        for (int i = 1; i < m_wormBody.count(); i++) {

            // stores new movement coords in every sixth update
            if (distCounter == 0) {
                float destx, desty, curx, cury, newx, newy;
                destx = m_wormBody.at(i-1)->px();
                desty = m_wormBody.at(i-1)->py();
                curx = m_wormBody.at(i)->px();
                cury = m_wormBody.at(i)->py();
                newx = destx-curx;
                newy = desty-cury;
                WormBody *wormBody = new WormBody(newx, newy);
                m_newDest.append(wormBody);
            }

            // this makes the actual movement
            float destx, desty, curx, cury;
            destx = m_newDest.at(i-1)->px();
            desty = m_newDest.at(i-1)->py();
            curx = m_wormBody.at(i)->px();
            cury = m_wormBody.at(i)->py();

            WormBody *wormBody = new WormBody(curx+(destx/5), cury+(desty/5));
            m_wormBody.replace(i, wormBody);
        }

        distCounter += 1;
        emit wormBodyChanged();


        // checks if the head hits to the body, looping from 6. body part
        for (int i = 5; i < m_wormBody.count(); i++) {
            float bx, by, disthb;
            bx = m_wormBody.at(i-1)->px();
            by = m_wormBody.at(i-1)->py();
            disthb = qSqrt((((cur_headx+bodyr)-(bx+bodyr))*((cur_headx+bodyr)-(bx+bodyr)))+(((cur_heady+bodyr)-(by+bodyr))*((cur_heady+bodyr)-(by+bodyr))));
            if (disthb <= bodyr*1.5) {
                m_isPaused = true;
                if (!m_theend) {
                    m_gameOver = true;
                    emit gameOverChanged();
                }
            }
        }

        // picks apples and grows with new body part
        foodDist = qSqrt((((cur_headx+bodyr)-(m_foodx+bodyr/2))*((cur_headx+bodyr)-(m_foodx+bodyr/2)))+(((cur_heady+bodyr)-(m_foody+bodyr/2))*((cur_heady+bodyr)-(m_foody+bodyr/2))));

        if (foodDist <= bodyr*1.5) {
            float curTx, curTy;
            curTx = m_wormBody.at((m_wormBody.count()-1))->px();
            curTy = m_wormBody.at((m_wormBody.count()-1))->py();

            WormBody *wormBody = new WormBody(curTx, curTy);
            WormBody *wormDest = new WormBody(0, 0);
            m_wormBody.append(wormBody);
            m_newDest.append(wormDest);
            m_apples += 1;
            emit applesChanged();
            newFoodPos();

        }

        // checks hits to the wall
        if ((cur_headx+bodyr) <= m_pagewidth/20 || (cur_heady+bodyr) <= m_pageheight/48 || (cur_headx+bodyr) >= m_pagewidth/1.04 || (cur_heady+bodyr) >= m_pageheight/1.3) {
            if (!m_theend) {
                m_isPaused = true;
                m_gameOver = true;
                emit gameOverChanged();
            }
        }

        // counts eated apples
        if (m_apples >= 70) {
            m_finish = true;
            emit finishChanged();
        }

        // when the worm goes to the "door" to freedom
        if (m_finish == true && cur_headx+bodyr >= m_pagewidth/2.842 && cur_headx+bodyr <= m_pagewidth/1.543 && cur_heady+bodyr >= 0 && cur_heady+bodyr <= m_pagewidth/18) {
            m_theend = true;
            emit theendChanged();
            m_wormBody.removeFirst();
            emit wormBodyChanged();
        }
    }
}

// sets the new game
void WormEngine::newGame()
{
    m_gameOver = false;
    m_finish = false;
    m_theend = false;
    m_apples = 0;

    emit gameOverChanged();
    emit finishChanged();
    emit theendChanged();
    emit applesChanged();
    m_newDest.clear();
    m_wormBody.clear();

    for (int i = 0; i < 6; i++) {
        WormBody *wormBody = new WormBody((m_pagewidth/3.375)+i*(m_pagewidth/10.8), m_pageheight/2.4);
        m_wormBody.append(wormBody);
    }

    emit wormBodyChanged();

    distCounter = 0;
    newFoodPos();
}

// calculates with random gen new pos of apples
void WormEngine::newFoodPos()
{
    do {
        m_foodx = randPosx(lowPx, highPx);
        m_foody = randPosy(lowPy, highPy);
        foodDist = qSqrt(((cur_headx-m_foodx)*(cur_headx-m_foodx))+((cur_heady-m_foody)*(cur_heady-m_foody)));
    } while (foodDist <= bodyr*3);

    emit foodxChanged();
    emit foodyChanged();
}

int WormEngine::randPosx(int lowPx, int highPx)
{
    return qrand() %  ((highPx + 1) - lowPx) + lowPx;
}

int WormEngine::randPosy(int lowPy, int highPy)
{
    return qrand() %  ((highPy + 1) - lowPy) + lowPy;
}

//setters and getters

bool WormEngine::isPaused() const
{
    return m_isPaused;
}

float WormEngine::foodx() const
{
    return m_foodx;
}

float WormEngine::foody() const
{
    return m_foody;
}

bool WormEngine::gameOver() const
{
    return m_gameOver;
}

int WormEngine::apples() const
{
    return m_apples;
}

bool WormEngine::finish() const
{
    return m_finish;
}

bool WormEngine::theend() const
{
    return m_theend;
}

bool WormEngine::pauseMenu() const
{
    return m_pauseMenu;
}

void WormEngine::setFoodx(float foodx)
{
    if (m_foodx == foodx)
        return;

    m_foodx = foodx;
    emit foodxChanged();
}

void WormEngine::setFoody(float foody)
{
    if (m_foody == foody)
        return;

    m_foody = foody;
    emit foodyChanged();
}

void WormEngine::setgameOver(bool gameOver)
{
    if (m_gameOver == gameOver)
        return;

    m_gameOver = gameOver;
    emit gameOverChanged();
}

void WormEngine::setapples(int apples)
{
    if (m_apples == apples)
        return;

    m_apples = apples;
    emit applesChanged();
}

void WormEngine::setfinish(bool finish)
{
    if (m_finish == finish)
        return;

    m_finish = finish;
    emit finishChanged();
}

void WormEngine::settheend(bool theend)
{
    if (m_theend == theend)
        return;

    m_theend = theend;
    emit theendChanged();
}

void WormEngine::setpauseMenu(bool pauseMenu)
{
    if (m_pauseMenu == pauseMenu)
        return;

    m_pauseMenu = pauseMenu;
    emit pauseMenuChanged();
}



void WormEngine::setisPaused(bool isPaused)
{
    if (m_isPaused == isPaused)
        return;

    m_isPaused = isPaused;
    emit isPausedChanged();
}




