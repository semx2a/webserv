# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/18 15:25:37 by seozcan           #+#    #+#              #
#    Updated: 2023/09/18 17:35:10 by seozcan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::PATHS::

PROJ	:=	

SDIR	:=	$(addprefix $(PROJ), src/)
ODIR	:=	$(addprefix $(PROJ), obj/)

CONF	:=	$(addprefix $(SDIR), conf/)
CORE	:=	$(addprefix $(SDIR), core/)
REQ		:=	$(addprefix $(SDIR), request/)
REP		:=	$(addprefix $(SDIR), response/)
UTILS	:=	$(addprefix $(SDIR), utils/)

# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::TARGET::

NAME	:=	webserv

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SOURCES::

SRC		:=	$(addprefix $(CONF),		\
				Config.cpp				\
				SpecConfig.cpp			\
				CommonConfig.cpp		\
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

MAIN	:=	$(addprefix $(SDIR), main.cpp)
MAIN_OBJ := $(patsubst $(SDIR)%.cpp,$(ODIR)%.o,$(MAIN))
MAIN_DEP := $(patsubst $(SDIR)%.cpp,$(ODIR)%.d,$(MAIN))

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::COMPILERS::

CPP		:=	c++

WFLAGS	:=	-Wall -Wextra -Werror -std=c++98

WCONV	:=	-Wconversion

DFLAGS	:=	-MMD

GFLAG	:=	-g3

SANFLAG	:=	-fsanitize=address

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

# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::PARAMS::

INCLUDE_FLAGS	= $(addprefix -I,	\
						$(CORE)			\
						$(CONF)			\
						$(REQ)			\
						$(REP)			\
						$(UTILS))

DEP				= $(OBJ:.o=.d)

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MANDATORY::

all: header h2 message $(NAME)

$(ODIR):
	@mkdir -p $(dir $@)
	@echo "$(HIGREEN)objs folder:[OK]$(NO_COLOR)" | $(INDENT)

$(OBJ): | $(ODIR)

$(ODIR)%.o: $(SDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CPP) $(WFLAGS) $(INCLUDE_FLAGS) -c $< -o $@ 
	@echo "$(HIGREEN)compilation:[OK]$(NO_COLOR)" | $(INDENT)

-include ${DEP}

$(NAME): $(OBJ)	$(MAIN_OBJ)
	@$(CPP) $(WFLAGS) $(DFLAGS) $(OBJ) $(MAIN_OBJ) -o $(NAME)
	@echo "$(HIGREEN)mandatory exe:[OK]$(NO_COLOR)" | $(INDENT)

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

fclean:		header clean
	@rm -f $(NAME)
	@echo "$(HIORANGE)$(NAME) bin:[RM]$(NO_COLOR)" | $(INDENT)

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
	@echo ""

h2:
	@echo "\n$(BHIPURPLE):::::::::::::::::::::::::::::::::::::::::::::::::::WEBSERV::\n$(NO_COLOR)"

message:
	@make -q $(NAME) && echo "$(BHIGREEN)All files are already up to date$(NO_COLOR)" || true

re:		header fclean 
	@make all 


# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::DEBUG::

TDIR	:=	$(addprefix $(PROJ), test/)

TSDIR	:= $(addprefix $(TDIR), src/)
TSRC	:= $(addprefix $(TDIR), parsing_tester.cpp)

TODIR	:= $(addprefix $(TDIR), obj/)
TOBJ	:= $(patsubst $(TDIR)%.cpp,$(TODIR)%.o,$(TSRC))
#TOBJ 	+= $(OBJ)

TDEP	:= $(patsubst $(TSDIR)%.cpp,$(TODIR)%.d,$(TSRC))

$(TOBJ): | $(TODIR)

$(TODIR):
	@mkdir -p $(dir $@)
	@echo "$(HIGREEN)objs folder:[OK]$(NO_COLOR)" | $(INDENT)

$(TODIR)%.o: $(TSDIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CPP) $(WFLAGS) $(INCLUDE_FLAGS) $(DFLAGS) $(TOBJ) -c $< -o $@ 
	@echo "$(HIGREEN)compilation:[OK]$(NO_COLOR)" | $(INDENT)

debug_parsing: $(OBJ) $(TOBJ)
	$(info TOBJ is $(TOBJ))
	@$(CPP) $(WFLAGS) $(OBJ) $(TOBJ) -o $(NAME)
	@echo "$(HIGREEN)debug parsing:[OK]$(NO_COLOR)" | $(INDENT)

clean_debug_parsing:
	@rm -rf $(TODIR) $(ODIR) 
	@echo "$(HIORANGE)objs folder:[RM]$(NO_COLOR)" | $(INDENT)

fclean_debug_parsing: clean_debug_parsing
	@rm -f $(NAME)
	@echo "$(HIORANGE)$(NAME) bin:[RM]$(NO_COLOR)" | $(INDENT)

re_debug_parsing: fclean_debug_parsing debug_parsing

.PHONY:	all bonus clean fclean re push update o_dir h1 h2 header 
