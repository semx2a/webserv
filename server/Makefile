# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/18 15:25:37 by seozcan           #+#    #+#              #
#    Updated: 2023/09/17 17:38:55 by seozcan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::PATHS::

PROJ	:=	

ODIR	:=	$(addprefix $(PROJ), .obj)

CONF	:=	$(addprefix $(PROJ), CONF)
CORE	:=	$(addprefix $(PROJ), CORE)
REQ		:=	$(addprefix $(PROJ), REQUEST)
REP		:=	$(addprefix $(PROJ), RESPONSE)
UTILS	:=	$(addprefix $(PROJ), UTILS)

# TDIR	:=	$(addprefix $(PROJ), templates))

# ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::TARGET::

NAME	:=	webserv

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::SOURCES::

SRCS	:=	$(addprefix $(CONF)/,		\
				Config.cpp				\
				SpecConfig.cpp			\
				CommonConfig.cpp		\
				Parser.cpp)				\
			$(addprefix $(CORE)/,		\
				ClientData.cpp			\
				Epoll.cpp 				\
				main.cpp 				\
				Server.cpp 				\
				Socket.cpp) 			\
			$(addprefix $(REQ)/, 		\
				Request.cpp)			\
			$(addprefix $(REP)/,		\
				Response.cpp)			\
			$(addprefix $(UTILS)/,		\
				display.cpp)

OBJS	:=	$(SRCS:%.cpp=$(ODIR)/%.o)

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

INCLUDE_FLAGS		= $(addprefix -I,	\
						$(CORE)			\
						$(CONF)			\
						$(REQ)			\
						$(REP)			\
						$(UTILS))

DEPS				= $(OBJS:.o=.d)

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MANDATORY::

all: header h2 message makeodir $(NAME)

$(ODIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CPP) $(WFLAGS) $(INCLUDE_FLAGS) -c $< -o $@ 
	@echo -e "$(HIGREEN)compilation:[OK]$(NO_COLOR)" | $(INDENT)

-include ${DEPS}

$(NAME): $(OBJS)	
	@$(CPP) $(WFLAGS) $(DFLAGS) $(OBJS) -o $(NAME)
	@echo -e "$(HIGREEN)mandatory exe:[OK]$(NO_COLOR)" | $(INDENT)

makeodir:
	@mkdir -p $(ODIR)
	@echo -e "$(HIGREEN)objs folder:[OK]$(NO_COLOR)" | $(INDENT)
	

# :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::RULES::

update:		header fclean
	@git pull

push:		header fclean
	@echo -e "$(HIGREEN)"
	@git add .
	@git commit --quiet
	@git push --quiet
	@echo -e "$(HIGREEN)git push:[OK]$(NO_COLOR)" | $(INDENT)

clean:		header
	@rm -rf $(ODIR)
	@echo -e "$(HIORANGE)objs folder:[RM]$(NO_COLOR)" | $(INDENT)

fclean:		header clean
	@rm -f $(NAME)
	@echo -e "$(HIORANGE)$(NAME) bin:[RM]$(NO_COLOR)" | $(INDENT)

header:
	@echo -e " ______________________________________________________"
	@echo -e "|\     $(BHIRED)__________    ________    ___   ___    _______ $(NO_COLOR) \ "
	@echo -e "| \   $(BHIORANGE)/\         \  /\   __  \  /\  \ /\  \  /\  ____\ $(NO_COLOR) \ "
	@echo -e "|  \  $(BHIGREEN)\ \  \ _/\  \ \ \   __  \ \ \  \ /_ /_ \ \  _\_/_ $(NO_COLOR) \ "
	@echo -e "|   \  $(BHIBLUE)\ \__\_/ \__\ \ \__\-\__\ \ \__\  \__\ \ \______\ $(NO_COLOR) \ "
	@echo -e "|    \  $(BHIPURPLE)\/__/  \/__/  \/__/ /__/  \/__/ \/__/  \/______/ $(NO_COLOR)  \ "
	@echo -e "|     \_____________________________________________________\ "
	@echo -e "|     /$(BLACK)////////////////////////////////////////////////////$(NO_COLOR) / "
	@echo -e "|    /$(BLACK)                                                   /$(NO_COLOR) / "
	@echo -e "|   /$(BLACK)                   S E O Z C A N                   /$(NO_COLOR) / "
	@echo -e "|  /$(BLACK)                   N F A U C O N N                 /$(NO_COLOR) / "
	@echo -e "| /$(BLACK)___________________________________________________/$(NO_COLOR) / "
	@echo -e "|/____________________________________________________\/ "
	@echo -e ""

h2:
	@echo -e "\n$(BHIPURPLE):::::::::::::::::::::::::::::::::::::::::::::::::::WEBSERV::\n$(NO_COLOR)"

message:
	@make -q $(NAME) && echo -e "$(BHIGREEN)All files are already up to date$(NO_COLOR)" || true

re:		header fclean 
	@make all 

-include $(DEPS)

.PHONY:	all bonus clean fclean re push update o_dir h1 h2 header 
