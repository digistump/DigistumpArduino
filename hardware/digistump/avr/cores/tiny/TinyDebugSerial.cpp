/*==============================================================================

  TinyDebugSerial.cpp - Tiny write-only software serial.

  Copyright 2010 Rowdy Dog Software.

  This file is part of Arduino-Tiny.

  Arduino-Tiny is free software: you can redistribute it and/or modify it 
  under the terms of the GNU Lesser General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  Arduino-Tiny is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License 
  along with Arduino-Tiny.  If not, see <http://www.gnu.org/licenses/>.

==============================================================================*/

#include "core_build_options.h"
#include "TinyDebugSerial.h"

static TinyDebugSerialWriter stub;

void TinyDebugSerial::useStub( void )
{
  _writer = &stub;
  _writer->init();
}

TinyDebugSerial::TinyDebugSerial( void )
{
  useStub();
}

#if defined( DEFAULT_TO_TINY_DEBUG_SERIAL ) && DEFAULT_TO_TINY_DEBUG_SERIAL
TinyDebugSerial Serial;
#endif
