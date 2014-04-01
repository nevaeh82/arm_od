#ifndef ITAB_H
#define ITAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "../MapInterface/MapController.h"

class ITab : public QWidget
{
public:
    virtual ~ITab() {};
    virtual QLabel *get_indicator()                             = 0;
    virtual QPushButton *get_push_button()                      = 0;
    virtual void set_map_controller(MapController *map_contr)   = 0;

    virtual void set_components()                               = 0;
};

#endif // ITAB_H
