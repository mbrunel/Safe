# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/15 01:02:41 by mbrunel           #+#    #+#              #
#    Updated: 2020/02/24 20:10:05 by mbrunel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=ps-managerv2
AES=aes

GIT=$(D_LIBFT)/.git $(D_AES)/.git

BUILD=.build
D_SRC=src
D_INC=inc

D_OBJ=$(BUILD)/obj
D_DEP=$(BUILD)/dep
D_SUB=
DIRS:=$(D_DEP) $(addprefix $(D_DEP)/, $(D_SUB))\
	$(D_OBJ) $(addprefix $(D_OBJ)/, $(D_SUB))

D_LIB=lib
D_LIBFT=$(D_LIB)/libft
LIBFT=$(D_LIBFT)/libft.a
D_NUKLEAR=$(D_LIB)/nuklear
D_AES=$(D_LIB)/tiny-AES-c
L_AES=aes.a
AES=$(D_AES)/$(L_AES)

CC=gcc
CFLAGS=-Wall -Wextra #-Werror
DFLAGS=-MP -MMD -MF $(D_DEP)/$*.d -MT $@
IFLAGS=-I$(D_INC) -I$(D_LIBFT)/inc -I$(D_NUKLEAR) `sdl2-config --cflags` -I$(D_AES)
LDFLAGS= $(LIBFT) -Ofast -lm -DNKCD=NKC_SDL -framework OpenGL -framework Cocoa -framework IOKit `sdl2-config --libs` $(AES)

C_RED=\033[31m
C_GREEN=\033[32m
C_CYAN=\033[36m
C_NONE=\033[0m
SUCCESS_MSG=SUCCESS

SRC=main.c\
	styles.c\
	data_b.c\
	calls_utils.c

OBJ:=$(patsubst %.c, $(D_OBJ)/%.o, $(SRC))
DEP:=$(patsubst %.c, $(D_DEP)/%.d, $(SRC))

$(NAME) : $(GIT) $(OBJ)
	@$(MAKE) -C $(D_LIBFT)
	@$(MAKE) -C $(D_AES) $(L_AES)
	@$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)
	@printf "\n%s\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n\n" $@

all : $(NAME)

linux : $(GIT)
	@$(MAKE) -C $(D_LIBFT)
	@$(MAKE) -C $(D_AES)
	gcc $(CFLAGS) $(addprefix $(D_SRC)/, $(SRC)) -O2 -Wall $(AES) -I$(D_AES) $(LIBFT) -lm -DNKCD=NKC_SDL  -D_REENTRANT -I/usr/local/include/SDL2 -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL2 -I/usr/include/libdrm -lGL -o $(NAME) -Ilib/nuklear_linux -Iinc  -Ilib/libft/inc

clean :
	@rm -rf $(BUILD)
	@printf "\nrm %s\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $(BUILD)

fclean : clean
	@rm -rf $(NAME)
	@printf "\nrm %s\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $(NAME)
	@$(MAKE) -C $(D_LIBFT) fclean
	@printf "\nrm %s\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $(LIBFT)
	@$(MAKE) -C $(D_AES) clean
	@printf "\nrm %s\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $(AES)

re : fclean all

$(GIT) :
	@git submodule init
	@git submodule update --remote

$(BUILD) :
	@mkdir -p $@ $(DIRS)
	@printf "\n%s\t\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $@

$(D_OBJ)/%.o : $(D_SRC)/%.c | $(BUILD)
	@$(CC) $(CFLAGS) $(IFLAGS) $(DFLAGS) -c $< -o $@
	@printf "\n%s\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $<

-include $(DEP)

.PHONY: all clean fclean re
