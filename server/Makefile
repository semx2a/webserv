# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/18 15:25:37 by seozcan           #+#    #+#              #
#    Updated: 2023/09/20 18:34:52 by seozcan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::COLORS::

NO_COLOR		=	\033[m

BLACK			=	\033[0;30m
RED				=	\033[0;31m
GREEN			=	\033[0;32m
ORANGE 		 	=	\033[0;33m
BLUE			=	\033[0;34m
PURPLE			=	\033[0;35m
CYAN			=	\033[0;36m
WHITE			= 	\033[0;37m

# High Intensty
HIGREEN			=	\033[0;92m
HIORANGE	 	=	\033[0;93m
HIPURPLE		=	\033[0;95m

# Bold
BORANGE 		=	\033[1;33m
BBLUE			=	\033[1;34m
BPURPLE			=	\033[1;35m
BCYAN			=	\033[1;36m

# Bold High Intensity
BHIBLACK		=	\033[1;30m
BHIRED			=	\033[1;91m
BHIGREEN		=	\033[1;92m
BHIORANGE		=	\033[1;33m
BHIYELLOW	 	=	\033[1;93m
BHIBLUE		 	=	\033[1;94m
BHIPURPLE		=	\033[1;35m
BHIPINK			=	\033[1;95m
BHICYAN			=	\033[1;96m
BHIWHITE		=	\033[1;37m

# Italic
ICYAN			=	\033[3;36m

# Identation
INDENT			=	awk -F ':' '{ printf "%-61s %s\n", $$1 ":", $$2 }'


# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::TARGET::

NAME	:=	webserv
TNAME	:=	testexe

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::PATHS::

SDIR	:=	src/
TDIR	:=	test/src/
ODIR	:=	obj/
TODIR	:=	test/obj/

CONF	:=	$(addprefix $(SDIR), conf/)
CORE	:=	$(addprefix $(SDIR), core/)
REQ		:=	$(addprefix $(SDIR), request/)
REP		:=	$(addprefix $(SDIR), response/)
UTILS	:=	$(addprefix $(SDIR), utils/)

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SOURCES::

SRC		:=	$(addprefix $(CONF),		\
				Config.cpp				\
				ServerContext.cpp		\
				Parser.cpp)				\
			$(addprefix $(CORE),		\
				ClientData.cpp			\
				Epoll.cpp 				\
				Server.cpp 				\
				Socket.cpp) 			\
			$(addprefix $(REQ), 		\
				Request.cpp)			\
			$(addprefix $(REP),			\
				Response.cpp)			\
			$(addprefix $(UTILS),		\
				print.cpp)

OBJ 	:= $(patsubst $(SDIR)%.cpp,$(ODIR)%.o,$(SRC))
DEP		:= $(patsubst $(SDIR)%.cpp,$(ODIR)%.d,$(SRC))

MAIN	:= $(addprefix $(SDIR), main.cpp)
MOBJ	:= $(patsubst $(SDIR)%.cpp,$(ODIR)%.o,$(MAIN))
MDEP	:= $(patsubst $(SDIR)%.cpp,$(ODIR)%.d,$(MAIN))

TSRC	:=	$(addprefix $(TDIR), parsing_tester.cpp)
TOBJ	:=	$(patsubst $(TDIR)%.cpp,$(TODIR)%.o,$(TSRC))
TDEP	:=	$(patsubst $(TDIR)%.cpp,$(TODIR)%.d,$(TSRC))

INC		:= $(addprefix -I,	\
				  $(CORE)			\
				  $(CONF)			\
				  $(REQ)			\
				  $(REP)			\
				  $(UTILS))

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::COMPILERS::

CPP		:=	c++

WFLAGS	:=	-Wall -Wextra -Werror -std=c++98

WCONV	:=	-Wconversion

DFLAGS	:=	-MMD

GFLAG	:=	-g3

SANFLAG	:=	-fsanitize=address

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MANDATORY::

all: header h2 message $(NAME)

$(ODIR):
	@mkdir -p $(dir $@)
	@echo "$(HIGREEN)objs folder:[OK]$(NO_COLOR)" | $(INDENT)

$(OBJ): | $(ODIR)

$(ODIR)%.o: $(SDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CPP) $(WFLAGS) $(DFLAGS) $(GFLAG) $(INC) -c $< -o $@ 
	@echo "$(HIGREEN)compilation:[OK]$(NO_COLOR)" | $(INDENT)

-include ${DEP}
-include ${MDEP}

$(NAME): $(OBJ)	$(MOBJ)
	@$(CPP) $(WFLAGS) $(DFLAGS) $(GFLAG) $(OBJ) $(MOBJ) -o $(NAME)
	@echo "$(HIGREEN)mandatory exe:[OK]$(NO_COLOR)" | $(INDENT)

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::TESTS::

test: header h2 message $(TNAME)

$(TODIR):
	@mkdir -p $(dir $@)
	@echo "$(HIGREEN)objs folder:[OK]$(NO_COLOR)" | $(INDENT)

$(TOBJ): | $(TODIR)

$(TODIR)%.o: $(TDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CPP) $(WFLAGS) $(DFLAGS) $(GFLAG) $(INC) -c $< -o $@ 
	@echo "$(HIGREEN)compilation:[OK]$(NO_COLOR)" | $(INDENT)

-include ${DEP}
-include ${TDEP}

$(TNAME): $(OBJ) $(TOBJ)
	@$(CPP) $(WFLAGS) $(DFLAGS) $(GFLAG) $(SANFLAG) $(OBJ) $(TOBJ) -o $(TNAME)
	@echo "$(HIGREEN)test exe:[OK]$(NO_COLOR)" | $(INDENT)

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::RULES::

update:		header fclean
	@git pull

push:		header fclean
	@echo "$(HIGREEN)"
	@git add .
	@git commit --quiet
	@git push --quiet
	@echo "$(HIGREEN)git push:[OK]$(NO_COLOR)" | $(INDENT)

clean:		header
	@rm -rf $(ODIR)
	@echo "$(HIORANGE)objs folder:[RM]$(NO_COLOR)" | $(INDENT)

fclean: header clean
	@rm -f $(NAME)
	@echo "$(HIORANGE)$(NAME) bin:[RM]$(NO_COLOR)" | $(INDENT)

re: header fclean all

cleantest:	header
	@rm -rf $(TODIR) $(ODIR)
	@echo "$(HIORANGE)objs folder:[RM]$(NO_COLOR)" | $(INDENT)

fcleantest: cleantest
	@rm -f $(TNAME)
	@echo "$(HIORANGE)$(TNAME) bin:[RM]$(NO_COLOR)" | $(INDENT)

retest:	fcleantest test

header:
	@echo " ______________________________________________________"
	@echo "|\     $(BHIRED)__________    ________    ___   ___    _______ $(NO_COLOR) \ "
	@echo "| \   $(BHIORANGE)/\         \  /\   __  \  /\  \ /\  \  /\  ____\ $(NO_COLOR) \ "
	@echo "|  \  $(BHIGREEN)\ \  \ _/\  \ \ \   __  \ \ \  \ /_ /_ \ \  _\_/_ $(NO_COLOR) \ "
	@echo "|   \  $(BHIBLUE)\ \__\_/ \__\ \ \__\-\__\ \ \__\  \__\ \ \______\ $(NO_COLOR) \ "
	@echo "|    \  $(BHIPURPLE)\/__/  \/__/  \/__/ /__/  \/__/ \/__/  \/______/ $(NO_COLOR)  \ "
	@echo "|     \_____________________________________________________\ "
	@echo "|     /$(BLACK)////////////////////////////////////////////////////$(NO_COLOR) / "
	@echo "|    /$(BLACK)                                                   /$(NO_COLOR) / "
	@echo "|   /$(BLACK)                   S E O Z C A N                   /$(NO_COLOR) / "
	@echo "|  /$(BLACK)                   N F A U C O N N                 /$(NO_COLOR) / "
	@echo "| /$(BLACK)___________________________________________________/$(NO_COLOR) / "
	@echo "|/____________________________________________________\/ "
	@echo "  \                \ \                 \ \            \ "
	@echo "   \                \ \                 \ \            \ "
	@echo "    \                \ \                 \ \            \ "
	@echo "     \________________\_\__             __\_\____________\ "
	@echo "                           🮡  _________/                    "

h2:
	@echo "\n$(BHIPURPLE):::::::::::::::::::::::::::::::::::::::::::::::::::WEBSERV::\n$(NO_COLOR)"

message:
	@make -q $(NAME) && echo "$(BHIGREEN)All files are already up to date$(NO_COLOR)" || true

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::DEBUG::
#
#TDIR	:=	$(addprefix $(PROJ), test/)
#
#TSDIR	:= $(addprefix $(TDIR), src/)
#TSRC	:= $(addprefix $(TDIR), parsing_tester.cpp)
#TSRC	+= $(SRC)
#
#TODIR	:= $(addprefix $(TDIR), obj/)
#TOBJ	:= ${subst ${TSDIR}, ${TODIR}, ${TSRC:%.cpp=%.o}}
#TOBJ	+= $(OBJ)
#
##TOBJ	:= $(patsubst $(TDIR)%.cpp,$(TODIR)%.o,$(TSRC))
##TOBJ 	+= $(OBJ)
#
##TDEP	:= $(tsubst $(TSDIR)%.cpp,$(TODIR)%.d,$(TSRC))
#
#$(TOBJ): | $(TODIR)
#
#$(TODIR):
#	mkdir -p $(dir $@)
#	echo "$(HIGREEN)objs folder:[OK]$(NO_COLOR)" | $(INDENT)
#
#$(TODIR)%.o: $(TSDIR)%.cpp
#	mkdir -p $(dir $@)
#	$(CPP) $(WFLAGS) $(INC) $(DFLAGS) $(TOBJ) -c $< -o $@ 
#	@echo "$(HIGREEN)compilation:[OK]$(NO_COLOR)" | $(INDENT)
#
#-include ${TDEP}
#-include ${DEP}
#
#debug_parsing: $(OBJ) $(TOBJ)
#	$(info TOBJ is $(TOBJ))
#	$(CPP) $(WFLAGS) $(OBJ) $(TOBJ) -o $(NAME)
#	@echo "$(HIGREEN)debug parsing:[OK]$(NO_COLOR)" | $(INDENT)
#
#clean_debug_parsing:
#	@rm -rf $(TODIR) $(ODIR) 
#	@echo "$(HIORANGE)objs folder:[RM]$(NO_COLOR)" | $(INDENT)
#
#fclean_debug_parsing: clean_debug_parsing
#	@rm -f $(NAME)
#	@echo "$(HIORANGE)$(NAME) bin:[RM]$(NO_COLOR)" | $(INDENT)
#
#re_debug_parsing: fclean_debug_parsing debug_parsing

.PHONY:	all bonus clean fclean re push update o_dir h1 h2 header 
