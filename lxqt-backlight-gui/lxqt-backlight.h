/*
    Copyright (C) 2016  P.L. Lucas <selairi@gmail.com>
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef __LXQT_BACKLIGHT_H__
#define __LXQT_BACKLIGHT_H__

#include <QDialog>
#include <QTextStream>
#include "ui_lxqtbacklight.h"
#include "../liblxqtbacklight/lxqtbacklight.h"

class LXQtBacklightDialog: public QDialog
{
Q_OBJECT

public:
    LXQtBacklightDialog(QWidget *parent =0);

public slots:
    void backlightSliderChanged(int value);
    void backlightChanged(int value);

private:    
    Ui::LXQtBacklightDialog *ui;
    LXQtBacklight *backlight;
};


#endif

