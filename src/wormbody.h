#ifndef WORMBODY
#define WORMBODY

#include <QObject>
//#include "wormengine.h"
class WormEngine;

class WormBody : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int px READ px WRITE setPx NOTIFY pxChanged)
    Q_PROPERTY(int py READ py WRITE setPy NOTIFY pyChanged)

public:

    explicit WormBody(QObject *parent = 0);
    WormBody(int px, int py, WormEngine *parent = 0);

signals:

    void pxChanged();
    void pyChanged();

    void setPx(int arg);
    void setPy(int arg);

public slots:

    int px() const;
    int py() const;

private:

    int m_px;
    int m_py;

    WormEngine *m_wormEngine;

};

#endif // WORMBODY

