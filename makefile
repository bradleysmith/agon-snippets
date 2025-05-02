# ----------------------------
# Makefile Options
# ----------------------------

NAME = KEYB_TEST
DESCRIPTION = "Keyboard test"
COMPRESSED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell agondev-config --makefile)
