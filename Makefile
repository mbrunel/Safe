# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/15 01:02:41 by mbrunel           #+#    #+#              #
#    Updated: 2020/02/16 07:39:14 by mbrunel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=ps-managerv2

GIT=$(D_LIBFT)/.git

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

CC=gcc
CFLAGS=-Wall -Wextra #-Werror
DFLAGS=-MP -MMD -MF $(D_DEP)/$*.d -MT $@
IFLAGS=-I$(D_INC) -I$(D_LIBFT)/inc `pkg-config --cflags gtk+-2.0`
LDFLAGS= $(LIBFT) `pkg-config --libs gtk+-2.0`

C_RED=\033[31m
C_GREEN=\033[32m
C_CYAN=\033[36m
C_NONE=\033[0m
SUCCESS_MSG=SUCCESS

SRC=main.c

OBJ:=$(patsubst %.c, $(D_OBJ)/%.o, $(SRC))
DEP:=$(patsubst %.c, $(D_DEP)/%.d, $(SRC))

$(NAME) : $(GIT) $(OBJ)
	@$(MAKE) -C $(D_LIBFT)
	@$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)
	@printf "\n%s\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n\n" $@

all : $(NAME)

clean :
	@rm -rf $(BUILD)
	@printf "\nrm %s\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $(BUILD)

fclean : clean
	@rm -rf $(NAME)
	@printf "\nrm %s\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $(NAME)
	@$(MAKE) -C $(D_LIBFT) fclean
	@printf "\nrm %s\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $(LIBFT)

re : fclean all

$(GIT) :
	@git submodule init
	@git submodule update --remote

$(BUILD) :
	@mkdir -p $@ $(DIRS)
	@printf "\n%s\t\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $@

$(D_OBJ)/%.o : $(D_SRC)/%.c $(INC) Makefile | $(BUILD)
	@$(CC) $(CFLAGS) $(IFLAGS) $(DFLAGS) -c $< -o $@
	@printf "\n%s\t\t$(C_GREEN)[$(SUCCESS_MSG)]$(C_NONE)\n" $<

-include $(DEP)

.PHONY: all clean fclean re
