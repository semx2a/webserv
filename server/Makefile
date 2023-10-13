# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/18 15:25:37 by seozcan           #+#    #+#              #
#    Updated: 2023/10/13 22:36:21 by seozcan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::COLORS::

RESET			=	\033[m

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
PROJ	:= 	./
SDIR	:=	$(addprefix $(PROJ), src/)
TDIR	:=	$(addprefix $(PROJ), test/src/)
ODIR	:=	$(addprefix $(PROJ), obj/)
TODIR	:=	$(addprefix $(PROJ), test/obj/)

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
				Epoll.cpp 				\
				Engine.cpp 				\
				Socket.cpp) 			\
			$(addprefix $(REQ), 		\
				Buffer.cpp				\
				Request.cpp)			\
			$(addprefix $(REP),			\
			   	AFile.cpp				\
			  	MimeTypes.cpp			\
				StatusCodes.cpp			\
				ResponseContext.cpp		\
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

DEPFLAG		:=	-MMD

DEBUGFLAG	:=	-g3 #-fsanitize=address

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MANDATORY::

all: header h2 message $(NAME)

$(ODIR):
	@mkdir -p $(dir $@)
	@echo "$(HIGREEN)objs folder:[OK]$(RESET)" | $(INDENT)

$(OBJ): | $(ODIR)

$(ODIR)%.o: $(SDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(DEPFLAG) $(DEBUGFLAG) $(INC) -c $< -o $@ 
	@echo "$(HIGREEN)compilation:[OK]$(RESET)" | $(INDENT)

-include ${DEP}
-include ${MDEP}

$(NAME): $(OBJ)	$(MOBJ)
	@$(CXX) $(CXXFLAGS) $(DEPFLAG) $(DEBUGFLAG) $(OBJ) $(MOBJ) -o $(NAME)
	@echo "$(HIGREEN)mandatory exe:[OK]$(RESET)" | $(INDENT)

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::RULES::

update:		header fclean
	@git pull

push:		header fclean
	@echo "$(HIGREEN)"
	@git add .
	@git commit --quiet
	@git push --quiet
	@echo "$(HIGREEN)git push:[OK]$(RESET)" | $(INDENT)

clean:		header
	@rm -rf $(ODIR)
	@echo "$(HIORANGE)objs folder:[RM]$(RESET)" | $(INDENT)

fclean: header clean
	@rm -f $(NAME)
	@echo "$(HIORANGE)$(NAME) bin:[RM]$(RESET)" | $(INDENT)

re: header fclean all

cleantest:	header
	@rm -rf $(TODIR) $(ODIR)
	@echo "$(HIORANGE)objs folder:[RM]$(RESET)" | $(INDENT)

fcleantest: cleantest
	@rm -f $(TNAME)
	@echo "$(HIORANGE)$(TNAME) bin:[RM]$(RESET)" | $(INDENT)

retest:	fcleantest test

header:
	@echo " ______________________________________________________"
	@echo "|\     $(BHIRED)__________    ________    ___   ___    _______ $(RESET) \ "
	@echo "| \   $(BHIORANGE)/\         \  /\   __  \  /\  \ /\  \  /\  ____\ $(RESET) \ "
	@echo "|  \  $(BHIGREEN)\ \  \ _/\  \ \ \   __  \ \ \  \ /_ /_ \ \  _\_/_ $(RESET) \ "
	@echo "|   \  $(BHIBLUE)\ \__\_/ \__\ \ \__\-\__\ \ \__\  \__\ \ \______\ $(RESET) \ "
	@echo "|    \  $(BHIPURPLE)\/__/  \/__/  \/__/ /__/  \/__/ \/__/  \/______/ $(RESET)  \ "
	@echo "|     \_____________________________________________________\ "
	@echo "|     /$(BLACK)////////////////////////////////////////////////////$(RESET) / "
	@echo "|    /$(BLACK)                                                   /$(RESET) / "
	@echo "|   /$(BLACK)                   S E O Z C A N                   /$(RESET) / "
	@echo "|  /$(BLACK)                   N F A U C O N N                 /$(RESET) / "
	@echo "| /$(BLACK)___________________________________________________/$(RESET) / "
	@echo "|/____________________________________________________\/ "
	@echo "  \                \ \                 \ \            \ "
	@echo "   \                \ \                 \ \            \ "
	@echo "    \                \ \                 \ \            \ "
	@echo "     \________________\_\__             __\_\____________\ "
	@echo "                           🮡  _________/                    "

h2:
	@echo "\n$(BHIPURPLE):::::::::::::::::::::::::::::::::::::::::::::::::::WEBSERV::\n$(RESET)"

message:
	@make -q $(NAME) && echo "$(BHIGREEN)All files are already up to date$(RESET)" || true

.PHONY:	all bonus clean fclean re push update h1 h2 header 
