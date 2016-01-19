#include <QtMath>
#include <QScreen>
#include <QGuiApplication>
#include "wormbody.h"
#include "duelengine.h"

DuelEngine::DuelEngine(QObject *parent) :
    QObject(parent)
{
    QScreen* screen = QGuiApplication::primaryScreen();
    m_pagewidth = screen->size().width();
    m_pageheight = screen->size().height();

    m_isPaused = true;
    m_gameOver = false;
    m_finish = false;
    m_theend = false;
    m_omission = false;
    m_playerspoint = false;
    m_enemyspoint = false;
    m_pauseMenu = false;
    m_omissionFrames = 0;
    bodyr = m_pagewidth/13.5;

    for (int i = 0; i < 6; i++) {
        WormBody *wormBody = new WormBody((m_pagewidth/3.375)+i*(m_pagewidth/10.8), m_pageheight/1.92);
        m_wormBody.append(wormBody);
    }

    for (int i = 0; i < 6; i++) {
        WormBody *wormBody = new WormBody((m_pagewidth/1.5)-i*(m_pagewidth/10.8), m_pageheight/4.8);
        m_enemyBody.append(wormBody);
    }

    m_updater.setInterval(50);
    connect(&m_updater, SIGNAL(timeout()), this, SLOT(update()));
    m_updater.start();
    lowPx = m_pagewidth/20;
    highPx = m_pagewidth/1.0188;
    lowPy = m_pageheight/48;
    highPy = m_pageheight/1.318;
    distCounter = 0;
    m_playerwon = 0;
    m_enemywon = 0;
    m_playerapples = 0;
    m_enemyapples = 0;
    newFoodPos();
}

QQmlListProperty<WormBody> DuelEngine::wormBody()
{
    return QQmlListProperty<WormBody>(this, m_wormBody);
}

QQmlListProperty<WormBody> DuelEngine::enemyBody()
{
    return QQmlListProperty<WormBody>(this, m_enemyBody);
}

void DuelEngine::setTouch(float tX, float tY)
{
    // takes touch coords from QML
    m_tX = tX;
    m_tY = tY;
}

void DuelEngine::update()
{
    if(!m_isPaused) {

        float cur_len, len, newdx, newdy;

        // resets worms body movement counter to zero
        if (distCounter == 5) {
            distCounter = 0;
            m_newDest.clear();
            m_enemyDest.clear();
        }

        if (m_omissionFrames >= 20) {
            m_omission = false;
            m_omissionFrames = 0;
        }

        // stores worms head pos at the beginning of every update
        cur_headx = m_wormBody.at(0)->px();
        cur_heady = m_wormBody.at(0)->py();

        // calcs distance from touch to head
        len = qSqrt((cur_headx-m_tX)*(cur_headx-m_tX)+(cur_heady-m_tY)*(cur_heady-m_tY));

        // sets the speed of worm
        cur_len = len/(bodyr/4.444);
        newdx = (m_tX-cur_headx)/cur_len;
        newdy = (m_tY-cur_heady)/cur_len;

        WormBody *wormBodyh = new WormBody(cur_headx+newdx, cur_heady+newdy);
        m_wormBody.replace(0, wormBodyh);

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

        emit wormBodyChanged();

        for (int i = 1; i < m_enemyBody.count(); i++) {
            float bx, by, disthb;
            bx = m_enemyBody.at(i-1)->px();
            by = m_enemyBody.at(i-1)->py();
            disthb = qSqrt((((cur_headx+bodyr)-(bx+bodyr))*((cur_headx+bodyr)-(bx+bodyr)))+(((cur_heady+bodyr)-(by+bodyr))*((cur_heady+bodyr)-(by+bodyr))));
            if (disthb <= bodyr*1.5) {
                if (!m_enemyspoint && !m_playerspoint) {
                    m_isPaused = true;
                    m_enemywon += 1;
                    emit enemywonChanged();
                    m_enemyspoint = true;
                    emit enemyspointChanged();
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
            m_playerapples += 1;
            emit playerapplesChanged();
            if (m_playerapples == 10) {
                m_isPaused = true;
                m_playerwon += 1;
                emit playerwonChanged();
                m_playerspoint = true;
                emit playerspointChanged();
            }
            newFoodPos();

        }

        // checks hits to the wall
        if ((cur_headx+bodyr) <= m_pagewidth/20 || (cur_heady+bodyr) <= m_pageheight/48 || (cur_headx+bodyr) >= m_pagewidth/1.04 || (cur_heady+bodyr) >= m_pageheight/1.3) {
            if (!m_enemyspoint && !m_playerspoint) {
                m_isPaused = true;
                m_enemyspoint = true;
                emit enemyspointChanged();
                m_enemywon += 1;
                emit enemywonChanged();
            }
        }

        // enemy movement
        cur_enemhx = m_enemyBody.at(0)->px();
        cur_enemhy = m_enemyBody.at(0)->py();

        float enlen, cur_elen, enewdx, enewdy, edestx, edesty, neckposx, neckposy, anglebody, anglehead;

        neckposx = m_enemyBody.at(1)->px();
        neckposy = m_enemyBody.at(1)->py();

        edestx = m_foodx;
        edesty = m_foody;

        anglebody = qRadiansToDegrees(qAtan2(neckposy-cur_enemhy, neckposx-cur_enemhx));
        anglehead = qRadiansToDegrees(qAtan2(cur_enemhy-edesty, cur_enemhx-edestx));

        for (int i = 0; i < m_wormBody.count(); i++) {              // set the new direction when sees an obstacle
            if (qSqrt((((cur_enemhx+bodyr)-(m_wormBody.at(i)->px()+bodyr))*((cur_enemhx+bodyr)-(m_wormBody.at(i)->px()+bodyr)))+((cur_enemhy+bodyr)-(m_wormBody.at(i)->py()+bodyr))*((cur_enemhy+bodyr)-(m_wormBody.at(i)->py()+bodyr))) < bodyr*2.5) {
                m_omission = true;
                m_omissionFrames = 0;
                if (anglebody <= -90) {
                    if (m_wormBody.at(i)->px() + bodyr > cur_enemhx + bodyr) {
                        m_omissionx = -bodyr*3;
                        m_omissiony = +bodyr*3;
                    } else {
                        m_omissionx = +bodyr*3;
                        m_omissiony = -bodyr*3;
                    }
                } else if (anglebody <= 0) {
                    if (m_wormBody.at(i)->py() + bodyr > cur_enemhy + bodyr) {
                        m_omissionx = -bodyr*3;
                        m_omissiony = -bodyr*3;
                    } else {
                        m_omissionx = +bodyr*3;
                        m_omissiony = +bodyr*3;
                    }
                } else if (anglebody <= 90) {
                    if (m_wormBody.at(i)->px() + bodyr > cur_enemhx + bodyr) {
                        m_omissionx = -bodyr*3;
                        m_omissiony = +bodyr*3;
                    } else {
                        m_omissionx = +bodyr*3;
                        m_omissiony = -bodyr*3;
                    }
                } else {
                    if (m_wormBody.at(i)->py() + bodyr > cur_enemhy + bodyr) {
                        m_omissionx = -bodyr*3;
                        m_omissiony = -bodyr*3;
                    } else {
                        m_omissionx = +bodyr*3;
                        m_omissiony = +bodyr*3;
                    }
                }
            }
        }

        if (cur_enemhx+bodyr <= bodyr && m_omissionx < 0 && m_omissiony < 0) {
            m_omissionx = 0;
            m_omissiony = -bodyr*3;
        }
        if (cur_enemhx+bodyr <= bodyr && m_omissionx < 0 && m_omissiony >= 0) {
            m_omissionx = 0;
            m_omissiony = bodyr*3;
        }
        if (cur_enemhx+bodyr >= highPx-bodyr && m_omissionx >= 0 && m_omissiony < 0) {
            m_omissionx = 0;
            m_omissiony = -bodyr*3;
        }
        if (cur_enemhx+bodyr >= highPx-bodyr && m_omissionx >= 0 && m_omissiony >= 0) {
            m_omissionx = 0;
            m_omissiony = bodyr*3;
        }
        if (cur_enemhy+bodyr <= bodyr && m_omissionx < 0 && m_omissiony < 0) {
            m_omissionx = -bodyr*3;
            m_omissiony = 0;
        }
        if (cur_enemhy+bodyr <= bodyr && m_omissionx >= 0 && m_omissiony < 0) {
            m_omissionx = bodyr*3;
            m_omissiony = 0;
        }
        if (cur_enemhy+bodyr >= highPy-bodyr && m_omissionx < 0 && m_omissiony >= 0) {
            m_omissionx = -bodyr*3;
            m_omissiony = 0;
        }
        if (cur_enemhy+bodyr >= highPy-bodyr && m_omissionx >= 0 && m_omissiony >= 0) {
            m_omissionx = bodyr*3;
            m_omissiony = 0;
        }

        if (anglebody - anglehead > bodyr/2 || anglebody - anglehead < -bodyr/2 ) {
            float sidex, sidey;
            sidex = qSqrt((cur_enemhx - neckposx)*(cur_enemhx - neckposx));
            sidey = qSqrt((cur_enemhy - neckposy)*(cur_enemhy - neckposy));

            if (anglebody >= 0) {                                                   // go up    this is clumsy way to make turns slower
                if (anglehead < anglebody && anglehead > (anglebody - 180)) { // turn left
                    if (anglebody <= 90) {                              // when going left
                        float locx, locy;
                        if (sidex >= sidey) {
                            locx = cur_enemhx - bodyr*1.5;
                            locy = cur_enemhy + (sidey/sidex)*bodyr*1.5;
                        } else {
                            locx = cur_enemhx - (sidex/sidey)*bodyr*1.5;
                            locy = cur_enemhy + bodyr*1.5;
                        }
                        if (anglebody == 0) {
                            edestx = cur_enemhx - bodyr*1.5;
                            edesty = cur_enemhy + bodyr*0.75;
                        } else {
                            edestx = locx - locy/4;
                            edesty = locy + locx/4;
                        }
                    } else {                                            // when going right
                        float locx, locy;
                        if (sidex >= sidey) {
                            locx = cur_enemhx + bodyr*1.5;
                            locy = cur_enemhy - (sidey/sidex)*bodyr*1.5;
                        } else {
                            locx = cur_enemhx + (sidex/sidey)*bodyr*1.5;
                            locy = cur_enemhy - bodyr*1.5;
                        }
                        if (anglebody == 180) {
                            edestx = cur_enemhx + bodyr*1.5;
                            edesty = cur_enemhy - bodyr*0.75;
                        } else {
                            edestx = locx - locy/4;
                            edesty = locy - locx/4;
                        }
                    }

                } else {                                                      // turn right
                    if (anglebody <= 90) {                              // when going left
                        float locx, locy;
                        if (sidex >= sidey) {
                            locx = cur_enemhx - bodyr*1.5;
                            locy = cur_enemhy - (sidey/sidex)*bodyr*1.5;
                        } else {
                            locx = cur_enemhx - (sidex/sidey)*bodyr*1.5;
                            locy = cur_enemhy - bodyr*1.5;
                        }
                        if (anglebody == 0) {
                            edestx = cur_enemhx - bodyr*1.5;
                            edesty = cur_enemhy - bodyr*0.75;
                        } else {
                            edestx = locx + locy/4;
                            edesty = locy - locx/4;
                        }
                    } else {                                            // when going right
                        float locx, locy;
                        if (sidex >= sidey) {
                            locx = cur_enemhx + bodyr*1.5;
                            locy = cur_enemhy + (sidey/sidex)*bodyr*1.5;
                        } else {
                            locx = cur_enemhx + (sidex/sidey)*bodyr*1.5;
                            locy = cur_enemhy + bodyr*1.5;
                        }
                        if (anglebody == 180) {
                            edestx = cur_enemhx + bodyr*1.5;
                            edesty = cur_enemhy + bodyr*0.75;
                        } else {
                            edestx = locx + locy/4;
                            edesty = locy + locx/4;
                        }
                    }

                }
            } else {                                                                // go down
                if (anglehead > anglebody && anglehead < (anglebody + 180)) { // turn right
                    if (anglebody >= -90) {                              // when going left
                        float locx, locy;
                        if (sidex >= sidey) {
                            locx = cur_enemhx - bodyr*1.5;
                            locy = cur_enemhy + (sidey/sidex)*bodyr*1.5;
                        } else {
                            locx = cur_enemhx - (sidex/sidey)*bodyr*1.5;
                            locy = cur_enemhy + bodyr*1.5;
                        }
                        edestx = locx - locy/4;
                        edesty = locy - locx/4;
                    } else {                                             // when going right
                        float locx, locy;
                        if (sidex >= sidey) {
                            locx = cur_enemhx + bodyr*1.5;
                            locy = cur_enemhy + (sidey/sidex)*bodyr*1.5;
                        } else {
                            locx = cur_enemhx + (sidex/sidey)*bodyr*1.5;
                            locy = cur_enemhy + bodyr*1.5;
                        }
                        edestx = locx - locy/4;
                        edesty = locy + locx/4;
                    }
                } else {                                                      // turn left
                    if (anglebody >= -90) {                              // when going left
                        float locx, locy;
                        if (sidex >= sidey) {
                            locx = cur_enemhx - bodyr*1.5;
                            locy = cur_enemhy + (sidey/sidex)*bodyr*1.5;
                        } else {
                            locx = cur_enemhx - (sidex/sidey)*bodyr*1.5;
                            locy = cur_enemhy + bodyr*1.5;
                        }
                        edestx = locx + locy/4;
                        edesty = locy + locx/4;
                    } else {                                             // when going right
                        float locx, locy;
                        if (sidex >= sidey) {
                            locx = cur_enemhx - bodyr*1.5;
                            locy = cur_enemhy + (sidey/sidex)*bodyr*1.5;
                        } else {
                            locx = cur_enemhx - (sidex/sidey)*bodyr*1.5;
                            locy = cur_enemhy + bodyr*1.5;
                        }
                        edestx = locx + locy/4;
                        edesty = locy - locx/4;
                    }
                }
            }

            if (edestx < 0 || edesty < 0) {
                edestx = m_foodx;
                edesty = m_foody;
            }

        }

        if (m_omission) {
            edestx = cur_enemhx + m_omissionx;
            edesty = cur_enemhy + m_omissiony;
            m_omissionFrames += 1;
        }

        enlen = qSqrt((cur_enemhx-edestx)*(cur_enemhx-edestx)+(cur_enemhy-edesty)*(cur_enemhy-edesty));

        // sets the speed of worm
        cur_elen = enlen/(bodyr/4.444);
        enewdx = (edestx-cur_enemhx)/cur_elen;
        enewdy = (edesty-cur_enemhy)/cur_elen;

        WormBody *wormBody = new WormBody(cur_enemhx+enewdx, cur_enemhy+enewdy);
        m_enemyBody.replace(0, wormBody);


        for (int i = 1; i < m_enemyBody.count(); i++) {

            // stores new movement coords in every sixth update
            if (distCounter == 0) {
                float edestx, edesty, ecurx, ecury, enewx, enewy;
                edestx = m_enemyBody.at(i-1)->px();
                edesty = m_enemyBody.at(i-1)->py();
                ecurx = m_enemyBody.at(i)->px();
                ecury = m_enemyBody.at(i)->py();
                enewx = edestx-ecurx;
                enewy = edesty-ecury;
                WormBody *ewormBody = new WormBody(enewx, enewy);
                m_enemyDest.append(ewormBody);
            }

            // this makes the actual movement
            float endestx, endesty, encurx, encury;
            endestx = m_enemyDest.at(i-1)->px();
            endesty = m_enemyDest.at(i-1)->py();
            encurx = m_enemyBody.at(i)->px();
            encury = m_enemyBody.at(i)->py();

            WormBody *enwormBody = new WormBody(encurx+(endestx/5), encury+(endesty/5));
            m_enemyBody.replace(i, enwormBody);
        }

        emit enemyBodyChanged();

        for (int i = 1; i < m_wormBody.count(); i++) {
            float bx, by, disthb;
            bx = m_wormBody.at(i-1)->px();
            by = m_wormBody.at(i-1)->py();
            disthb = qSqrt((((cur_enemhx+bodyr)-(bx+bodyr))*((cur_enemhx+bodyr)-(bx+bodyr)))+(((cur_enemhy+bodyr)-(by+bodyr))*((cur_enemhy+bodyr)-(by+bodyr))));
            if (disthb <= bodyr*1.5) {
                m_isPaused = true;
                if (!m_playerspoint && !m_enemyspoint) {
                    m_playerwon += 1;
                    emit playerwonChanged();
                    m_playerspoint = true;
                    emit playerspointChanged();
                }
            }
        }

        // picks apples and grows with new body part
        foodDist = qSqrt((((cur_enemhx+bodyr)-(m_foodx+bodyr/2))*((cur_enemhx+bodyr)-(m_foodx+bodyr/2)))+(((cur_enemhy+bodyr)-(m_foody+bodyr/2))*((cur_enemhy+bodyr)-(m_foody+bodyr/2))));

        if (foodDist <= bodyr*1.5) {
            float curTx, curTy;
            curTx = m_enemyBody.at((m_enemyBody.count()-1))->px();
            curTy = m_enemyBody.at((m_enemyBody.count()-1))->py();

            WormBody *wormBody = new WormBody(curTx, curTy);
            WormBody *wormDest = new WormBody(0, 0);
            m_enemyBody.append(wormBody);
            m_enemyDest.append(wormDest);
            m_enemyapples += 1;
            emit enemyapplesChanged();
            if (m_enemyapples == 10) {
                m_isPaused = true;
                m_enemywon += 1;
                emit enemywonChanged();
                m_enemyspoint = true;
                emit enemyspointChanged();
            }
            newFoodPos();
        }

        if ((cur_enemhx+bodyr) <= bodyr/2 || (cur_enemhy+bodyr) <= bodyr/2 || (cur_enemhx+bodyr) >= highPx || (cur_enemhy+bodyr) >= highPy) {
            if (!m_playerspoint && !m_enemyspoint) {
                m_isPaused = true;
                m_playerwon += 1;
                emit playerwonChanged();
                m_playerspoint = true;
                emit playerspointChanged();
            }
        }

        if (m_playerwon == 10) {
            m_playerspoint = false;
            emit playerspointChanged();
            m_theend = true;
            emit theendChanged();
        }

        if (m_enemywon == 10) {
            m_enemyspoint = false;
            emit enemyspointChanged();
            m_gameOver = true;
            emit gameOverChanged();
        }

        distCounter += 1;
    }
}

// sets the new game
void DuelEngine::newGame()
{
    m_gameOver = false;
    m_finish = false;
    m_theend = false;
    m_omission = false;
    m_playerspoint = false;
    m_enemyspoint = false;
    m_omissionFrames = 0;
    m_playerapples = 0;
    m_enemyapples = 0;

    emit gameOverChanged();
    emit finishChanged();
    emit theendChanged();
    emit playerapplesChanged();
    emit enemyapplesChanged();
    emit playerspointChanged();
    emit enemyspointChanged();

    m_newDest.clear();
    m_enemyDest.clear();
    m_wormBody.clear();
    m_enemyBody.clear();

    for (int i = 0; i < 6; i++) {
        WormBody *wormBody = new WormBody((m_pagewidth/3.375)+i*(m_pagewidth/10.8), m_pageheight/1.92);
        m_wormBody.append(wormBody);
    }

    for (int i = 0; i < 6; i++) {
        WormBody *wormBody = new WormBody((m_pagewidth/1.5)-i*(m_pagewidth/10.8), m_pageheight/4.8);
        m_enemyBody.append(wormBody);
    }

    emit wormBodyChanged();
    emit enemyBodyChanged();

    distCounter = 0;
    newFoodPos();
}

void DuelEngine::resetGame()
{
    m_gameOver = false;
    m_finish = false;
    m_theend = false;
    m_omission = false;
    m_playerspoint = false;
    m_enemyspoint = false;
    m_omissionFrames = 0;
    m_playerwon = 0;
    m_enemywon = 0;
    m_playerapples = 0;
    m_enemyapples = 0;

    emit playerapplesChanged();
    emit playerwonChanged();
    emit enemyapplesChanged();
    emit enemywonChanged();
    emit gameOverChanged();
    emit finishChanged();
    emit theendChanged();
    emit playerspointChanged();
    emit enemyspointChanged();

    m_newDest.clear();
    m_enemyDest.clear();
    m_wormBody.clear();
    m_enemyBody.clear();

    for (int i = 0; i < 6; i++) {
        WormBody *wormBody = new WormBody((m_pagewidth/3.375)+i*(m_pagewidth/10.8), m_pageheight/1.92);
        m_wormBody.append(wormBody);
    }

    for (int i = 0; i < 6; i++) {
        WormBody *wormBody = new WormBody((m_pagewidth/1.5)-i*(m_pagewidth/10.8), m_pageheight/4.8);
        m_enemyBody.append(wormBody);
    }

    emit wormBodyChanged();
    emit enemyBodyChanged();

    distCounter = 0;
    newFoodPos();
}

// calculates with random gen new pos of apples
void DuelEngine::newFoodPos()
{
    do {
        m_foodx = randPosx(lowPx, highPx-bodyr);
        m_foody = randPosy(lowPy, highPy-bodyr);
        foodDist = qSqrt(((cur_headx-m_foodx)*(cur_headx-m_foodx))+((cur_heady-m_foody)*(cur_heady-m_foody)));
    } while (foodDist <= bodyr*3);

    emit foodxChanged();
    emit foodyChanged();
}

int DuelEngine::randPosx(int lowPx, int highPx)
{
    return qrand() %  ((highPx + 1) - lowPx) + lowPx;
}

int DuelEngine::randPosy(int lowPy, int highPy)
{
    return qrand() %  ((highPy + 1) - lowPy) + lowPy;
}

//setters and getters

bool DuelEngine::isPaused() const
{
    return m_isPaused;
}

float DuelEngine::foodx() const
{
    return m_foodx;
}

float DuelEngine::foody() const
{
    return m_foody;
}

bool DuelEngine::gameOver() const
{
    return m_gameOver;
}


bool DuelEngine::finish() const
{
    return m_finish;
}

bool DuelEngine::theend() const
{
    return m_theend;
}

int DuelEngine::playerwon() const
{
    return m_playerwon;
}

int DuelEngine::enemywon() const
{
    return m_enemywon;
}

int DuelEngine::playerapples() const
{
    return m_playerapples;
}

int DuelEngine::enemyapples() const
{
    return m_enemyapples;
}

bool DuelEngine::playerspoint() const
{
    return m_playerspoint;
}

bool DuelEngine::enemyspoint() const
{
    return m_enemyspoint;
}

bool DuelEngine::pauseMenu() const
{
    return m_pauseMenu;
}

void DuelEngine::setFoodx(float foodx)
{
    if (m_foodx == foodx)
        return;

    m_foodx = foodx;
    emit foodxChanged();
}

void DuelEngine::setFoody(float foody)
{
    if (m_foody == foody)
        return;

    m_foody = foody;
    emit foodyChanged();
}

void DuelEngine::setgameOver(bool gameOver)
{
    if (m_gameOver == gameOver)
        return;

    m_gameOver = gameOver;
    emit gameOverChanged();
}

void DuelEngine::setfinish(bool finish)
{
    if (m_finish == finish)
        return;

    m_finish = finish;
    emit finishChanged();
}

void DuelEngine::settheend(bool theend)
{
    if (m_theend == theend)
        return;

    m_theend = theend;
    emit theendChanged();
}

void DuelEngine::setplayerwon(int playerwon)
{
    if (m_playerwon == playerwon)
        return;

    m_playerwon = playerwon;
    emit playerwonChanged();
}

void DuelEngine::setenemywon(int enemywon)
{
    if (m_enemywon == enemywon)
        return;

    m_enemywon = enemywon;
    emit enemywonChanged();
}

void DuelEngine::setplayerapples(int playerapples)
{
    if (m_playerapples == playerapples)
        return;

    m_playerapples = playerapples;
    emit playerapplesChanged();
}

void DuelEngine::setenemyapples(int enemyapples)
{
    if (m_enemyapples == enemyapples)
        return;

    m_enemyapples = enemyapples;
    emit enemyapplesChanged();
}

void DuelEngine::setplayerspoint(bool playerspoint)
{
    if (m_playerspoint == playerspoint)
        return;

    m_playerspoint = playerspoint;
    emit playerspointChanged();
}

void DuelEngine::setnnemyspoint(bool enemyspoint)
{
    if (m_enemyspoint == enemyspoint)
        return;

    m_enemyspoint = enemyspoint;
    emit enemyspointChanged();
}

void DuelEngine::setpauseMenu(bool pauseMenu)
{
    if (m_pauseMenu == pauseMenu)
        return;

    m_pauseMenu = pauseMenu;
    emit pauseMenuChanged();
}

void DuelEngine::setisPaused(bool isPaused)
{
    if (m_isPaused == isPaused)
        return;

    m_isPaused = isPaused;
    emit isPausedChanged();
}

