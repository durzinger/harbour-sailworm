#include "wormbody.h"
#include "wormengine.h"

WormBody::WormBody(QObject *parent) :
    QObject(parent)
{
}

WormBody::WormBody(int px, int py, WormEngine *parent) :
    QObject(parent), m_px(px), m_py(py), m_wormEngine(parent)
{

}

int WormBody::px() const
{
    return m_px;
}

int WormBody::py() const
{
    return m_py;
}

inline void WormBody::setPx(int arg)
{
    m_px = arg;
    emit pxChanged();
}

inline void WormBody::setPy(int arg)
{
    m_py = arg;
    emit pyChanged();
}

