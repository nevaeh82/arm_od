#include "nstation.h"

NStation::NStation(int id)
{
    m_id = id;

    m_mode = 0;
    m_azimuth = 0;
    m_radius = 100;
    m_width = 30;
    m_isCircle = false;
    m_isEnable = true;
    m_name = "Station";
}
