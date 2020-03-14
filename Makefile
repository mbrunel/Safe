# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/15 01:02:41 by mbrunel           #+#    #+#              #
#    Updated: 2020/03/14 06:52:00 by mbrunel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=Safe

OS := $(shell uname)

OPENSSL=$(HOME)/.brew/Cellar/openssl@1.1
SDL2=$(HOME)/.brew/Cellar/sdl2

ifeq ($(OS), Linux)
OPENSSL=/usr/bin/sdl2-config
SDL2=/usr/bin/openssl
endif

BUILD=.build
D_SRC=src
D_INC=inc

D_OBJ=$(BUILD)/obj
D_DEP=$(BUILD)/dep
D_SUB=
DIRS:=$(D_DEP) $(addprefix $(D_DEP)/, $(D_SUB))\
	$(D_OBJ) $(addprefix $(D_OBJ)/, $(D_SUB))

D_LIB=lib
D_NUKLEAR=$(D_LIB)/nuklear

CC=gcc
DFLAGS=-MP -MMD -MF $(D_DEP)/$*.d -MT $@


CFLAGS=-Wall -Wextra -g3 -Ofast -DOSX=1
IFLAGS=-I$(D_INC) -I$(D_NUKLEAR) `sdl2-config --cflags` -I/$(HOME)/.brew/opt/openssl@1.1/include -I$(HOME)/.brew/include
LDFLAGS=-lm -DNKCD=NKC_SDL -framework OpenGL -framework Cocoa -framework IOKit `sdl2-config --libs` -L/$(HOME)/.brew/opt/openssl@1.1/lib -lcrypto

ifeq ($(OS), Linux)
CFLAGS=-Ofast -g3 -Wall -Wextra -DLINUX=1
IFLAGS=-I$(D_NUKLEAR) -I$(D_INC) `pkg-config openssl --cflags` `pkg-config sdl2 --cflags` -I/usr/include/libdrm
LDFLAGS=-lm -DNKCD=NKC_SDL `pkg-config openssl --libs` `pkg-config sdl2 --libs` -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lGL
endif

C_RED=\033[31m
C_GREEN=\033[32m
C_CYAN=\033[36m
C_NONE=\033[0m
CROSS=✘#\xE2\x9C\x95
CHECK=✔#\xE2\x9C\x94
BUILD_MSG=$(C_GREEN)$(CHECK)$(C_NONE)
REMOVE_MSG=$(C_RED)$(CROSS)$(C_NONE)

SRC=main.c\
	styles.c\
	back_end.c\
	utils.c\
	front_end.c\
	aes.c

OBJ:=$(patsubst %.c, $(D_OBJ)/%.o, $(SRC))
DEP:=$(patsubst %.c, $(D_DEP)/%.d, $(SRC))

all : $(SDL2) $(OPENSSL)
	@$(MAKE) -j $(NAME)

$(NAME) : $(SDL2) $(OPENSSL) $(OBJ)
	@$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)
	@printf "$(BUILD_MSG) %s\n" $@

clean :
	@rm -rf $(BUILD)
	@printf "$(REMOVE_MSG) rm %s\n" $(BUILD)

fclean : clean
	@rm -rf $(NAME)
	@printf "$(REMOVE_MSG) rm %s\n" $(NAME)

re : fclean all

$(OPENSSL) :
	@printf "\n$(C_CYAN)Install openssl...$(C_NONE)\n"
	brew install openssl

$(SDL2) :
	@printf "\n$(C_CYAN)Install sdl2...$(C_NONE)\n"
	brew install sdl2

$(BUILD) :
	@mkdir -p $@ $(DIRS)
	@printf "$(BUILD_MS-DLINUX=1G) %s\n" $@

$(D_OBJ)/%.o : $(D_SRC)/%.c | $(BUILD)
	@$(CC) $(CFLAGS) $(IFLAGS) $(DFLAGS) -c $< -o $@
	@printf "$(BUILD_MSG) %s\n" $<

-include $(DEP)

.PHONY: all clean fclean re
