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

#include "lxqt-backlight.h"
#include <QDebug>

LXQtBacklightDialog::LXQtBacklightDialog(QWidget *parent):QDialog(parent)
{
    ui = new Ui::LXQtBacklightDialog();
    ui->setupUi(this);
    
    backlight = new LXQtBacklight(this);
        
    if( backlight->isBacklightAvailable() ) {
        ui->backlightSlider->setMaximum( backlight->getMaxBacklight() );
        ui->backlightSlider->setValue( backlight->getBacklight() );
        connect(ui->backlightSlider, SIGNAL(valueChanged(int)),
            this, SLOT(backlightSliderChanged(int)) );
        connect(backlight, SIGNAL(backlightChanged(int)),
            this, SLOT(backlightChanged(int)) );
    }
    else
        ui->backlightSlider->setEnabled(false);
}

void LXQtBacklightDialog::backlightSliderChanged(int value)
{
    backlight->setBacklight(value);
}

void LXQtBacklightDialog::backlightChanged(int value)
{
    disconnect(ui->backlightSlider, SIGNAL(valueChanged(int)),
        this, SLOT(backlightSliderChanged(int)) );
    ui->backlightSlider->setValue( value );
    connect(ui->backlightSlider, SIGNAL(valueChanged(int)),
        this, SLOT(backlightSliderChanged(int)) );
}
