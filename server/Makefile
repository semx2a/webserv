# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/18 15:25:37 by seozcan           #+#    #+#              #
#    Updated: 2023/10/05 18:32:37 by seozcan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::COLORS::

RESET		=	\033[m

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
				ServerContext.cpp		\
				LocationContext.cpp		\
				Parser.cpp				\
				AContext.cpp)			\
			$(addprefix $(CORE),		\
				Buffer.cpp				\
				Epoll.cpp 				\
				Engine.cpp 				\
				Socket.cpp) 			\
			$(addprefix $(REQ), 		\
				Request.cpp)			\
			$(addprefix $(REP),			\
			   	AFile.cpp				\
			  	MimeTypes.cpp			\
				StatusCodes.cpp			\
				Response.cpp			\
				CGI.cpp					\
				ARespComponent.cpp		\
				StatusLine.cpp			\
				Headers.cpp				\
				Body.cpp)				\
			$(addprefix $(UTILS),		\
				utl.cpp				\
				HttpStatus.cpp)

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

CXX			:=	c++

CXXFLAGS	:=	-Wall -Wextra -Werror -std=c++98

CONVFLAGS	:=	-Wconversion

DFLAGS		:=	-MMD

GFLAG		:=	-g3

SANFLAG		:=	-fsanitize=address

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MANDATORY::

all: header h2 message $(NAME)

$(ODIR):
	@mkdir -p $(dir $@)
	@echo -e "$(HIGREEN)objs folder:[OK]$(RESET)" | $(INDENT)

$(OBJ): | $(ODIR)

$(ODIR)%.o: $(SDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) $(GFLAG) $(SANFLAG) $(INC) -c $< -o $@ 
	@echo -e "$(HIGREEN)compilation:[OK]$(RESET)" | $(INDENT)

-include ${DEP}
-include ${MDEP}

$(NAME): $(OBJ)	$(MOBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) $(GFLAG) $(SANFLAG) $(OBJ) $(MOBJ) -o $(NAME)
	@echo -e "$(HIGREEN)mandatory exe:[OK]$(RESET)" | $(INDENT)

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::TESTS::

test: header h2 message $(TNAME)

$(TODIR):
	@mkdir -p $(dir $@)
	@echo -e "$(HIGREEN)objs folder:[OK]$(RESET)" | $(INDENT)

$(TOBJ): | $(TODIR)

$(TODIR)%.o: $(TDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) $(GFLAG) $(INC) -c $< -o $@ 
	@echo -e "$(HIGREEN)compilation:[OK]$(RESET)" | $(INDENT)

-include ${DEP}
-include ${TDEP}

$(TNAME): $(OBJ) $(TOBJ)
	@$(CXX) $(CXXFLAGS) $(DFLAGS) $(GFLAG) $(SANFLAG) $(OBJ) $(TOBJ) -o $(TNAME)
	@echo -e "$(HIGREEN)test exe:[OK]$(RESET)" | $(INDENT)

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::RULES::

update:		header fclean
	@git pull

push:		header fclean
	@echo -e "$(HIGREEN)"
	@git add .
	@git commit --quiet
	@git push --quiet
	@echo -e "$(HIGREEN)git push:[OK]$(RESET)" | $(INDENT)

clean:		header
	@rm -rf $(ODIR)
	@echo -e "$(HIORANGE)objs folder:[RM]$(RESET)" | $(INDENT)

fclean: header clean
	@rm -f $(NAME)
	@echo -e "$(HIORANGE)$(NAME) bin:[RM]$(RESET)" | $(INDENT)

re: header fclean all

cleantest:	header
	@rm -rf $(TODIR) $(ODIR)
	@echo -e "$(HIORANGE)objs folder:[RM]$(RESET)" | $(INDENT)

fcleantest: cleantest
	@rm -f $(TNAME)
	@echo -e "$(HIORANGE)$(TNAME) bin:[RM]$(RESET)" | $(INDENT)

retest:	fcleantest test

header:
	@echo -e " ______________________________________________________"
	@echo -e "|\     $(BHIRED)__________    ________    ___   ___    _______ $(RESET) \ "
	@echo -e "| \   $(BHIORANGE)/\         \  /\   __  \  /\  \ /\  \  /\  ____\ $(RESET) \ "
	@echo -e "|  \  $(BHIGREEN)\ \  \ _/\  \ \ \   __  \ \ \  \ /_ /_ \ \  _\_/_ $(RESET) \ "
	@echo -e "|   \  $(BHIBLUE)\ \__\_/ \__\ \ \__\-\__\ \ \__\  \__\ \ \______\ $(RESET) \ "
	@echo -e "|    \  $(BHIPURPLE)\/__/  \/__/  \/__/ /__/  \/__/ \/__/  \/______/ $(RESET)  \ "
	@echo -e "|     \_____________________________________________________\ "
	@echo -e "|     /$(BLACK)////////////////////////////////////////////////////$(RESET) / "
	@echo -e "|    /$(BLACK)                                                   /$(RESET) / "
	@echo -e "|   /$(BLACK)                   S E O Z C A N                   /$(RESET) / "
	@echo -e "|  /$(BLACK)                   N F A U C O N N                 /$(RESET) / "
	@echo -e "| /$(BLACK)___________________________________________________/$(RESET) / "
	@echo -e "|/____________________________________________________\/ "
	@echo -e "  \                \ \                 \ \            \ "
	@echo -e "   \                \ \                 \ \            \ "
	@echo -e "    \                \ \                 \ \            \ "
	@echo -e "     \________________\_\__             __\_\____________\ "
	@echo -e "                           🮡  _________/                    "

h2:
	@echo -e "\n$(BHIPURPLE):::::::::::::::::::::::::::::::::::::::::::::::::::WEBSERV::\n$(RESET)"

message:
	@make -q $(NAME) && echo -e "$(BHIGREEN)All files are already up to date$(RESET)" || true

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
#	echo -e "$(HIGREEN)objs folder:[OK]$(RESET)" | $(INDENT)
#
#$(TODIR)%.o: $(TSDIR)%.cpp
#	mkdir -p $(dir $@)
#	$(CXX) $(CXXFLAGS) $(INC) $(DFLAGS) $(TOBJ) -c $< -o $@ 
#	@echo -e "$(HIGREEN)compilation:[OK]$(RESET)" | $(INDENT)
#
#-include ${TDEP}
#-include ${DEP}
#
#debug_parsing: $(OBJ) $(TOBJ)
#	$(info TOBJ is $(TOBJ))
#	$(CXX) $(CXXFLAGS) $(OBJ) $(TOBJ) -o $(NAME)
#	@echo -e "$(HIGREEN)debug parsing:[OK]$(RESET)" | $(INDENT)
#
#clean_debug_parsing:
#	@rm -rf $(TODIR) $(ODIR) 
#	@echo -e "$(HIORANGE)objs folder:[RM]$(RESET)" | $(INDENT)
#
#fclean_debug_parsing: clean_debug_parsing
#	@rm -f $(NAME)
#	@echo -e "$(HIORANGE)$(NAME) bin:[RM]$(RESET)" | $(INDENT)
#
#re_debug_parsing: fclean_debug_parsing debug_parsing

.PHONY:	all bonus clean fclean re push update o_dir h1 h2 header 
