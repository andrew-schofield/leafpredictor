# Copyright (C) 2008 Andrew Schofield <the.uncle.fungus@gmail.com>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

bin_PROGRAMS = leafpredictor
leafpredictor_SOURCES = aboutDialog.cpp eigenSystem.cpp main.cpp mainDialog.cpp
INCLUDES = -I$(top_srcdir)/src/include
AM_CXXFLAGS = -Wall -Wno-strict-aliasing
AM_CPPFLAGS = \
	-DDATADIR=\"$(datadir)\"