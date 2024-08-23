########################## Colors ##########################

COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_ORANGE=\033[0;33m
COLOUR_BLUE=\033[0;34m
COLOUR_CYAN=\033[0;36m
COLOUR_DARK_GRAY=\033[0;90m
COLOUR_END=\033[0m

GREEN_FLASH		=	\e[92;5;118m
RED_FLASH		=	\e[91;5;196m
ORANGE_FLASH	=	\e[93;5;226m
RESET_FLASH		=	\e[0m

########################## Variables ##########################

NAME		= webserv

OBJDIR = .objFiles

FILES		=	src/main\
				src/server/Config\
				src/server/Server\
				src/server/ServerManager\
				src/request/Request\
				src/response/CGI_GET_POST_requests\
				src/response/methodDELETE\
				src/response/methodGET_dir\
				src/response/methodGET_file\
				src/response/methodGET\
				src/response/methodPOST\
				src/response/ProcessRequest\
				src/response/requestRouting\
				src/response/Response\
				src/response/StatusCodes_ContentTypes\
				src/response/cookie\

SRC			= $(FILES:=.cpp)

OBJ			= $(addprefix $(OBJDIR)/, $(FILES:=.o))

HEADER		= include/response/ProcessRequest.hpp\
				include/response/Response.hpp\
				include/Config.hpp\
				include/Request.hpp\
				include/Server.hpp\
				include/ServerManager.hpp\
				include/webserv.hpp\
				include/StatusCodes_ContentTypes.hpp\

CC			= c++
FLAGS		= -std=c++98 -Wall -Wextra -Werror -g3
RM			= rm -rf

########################## Rules ##########################

all: $(NAME)

$(NAME): $(OBJ) $(HEADER) Makefile
	@echo $(BANNER)
	@echo "$(COLOUR_DARK_GRAY)Compiling files...$(COLOUR_END)"
	@$(CC) $(OBJ) $(OPTS) -o $(NAME)
	@echo "$(GREEN_FLASH)↪$(RESET_FLASH) Finished, $(COLOUR_BLUE)$(NAME)$(COLOUR_END) is ready, all files have been $(COLOUR_GREEN)compiled$(COLOUR_END).\n"

$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@echo "$(COLOUR_GREEN)Compiling $<...$(COLOUR_END)"
	@$(CC) $(FLAGS) $(OPTS) -c $< -o $@
	@sleep .033
	@printf "\033[1A"
	@printf "\033[K"

clean:
	@echo "$(COLOUR_DARK_GRAY)Cleaning files...$(COLOUR_END)"
	@$(RM) $(OBJDIR) $(OBJ)
	@echo "$(ORANGE_FLASH)↪$(RESET_FLASH) Finished, $(COLOUR_BLUE)code files$(COLOUR_END) have been $(COLOUR_ORANGE)cleaned$(COLOUR_END)."

clean_files:
	@echo "$(COLOUR_DARK_GRAY)Cleaning files in www/upload/file...$(COLOUR_END)"
	@$(RM) www/upload/file/*
	@echo "$(ORANGE_FLASH)↪$(RESET_FLASH) Finished, files in /www/upload/file have been $(COLOUR_ORANGE)cleaned$(COLOUR_END)."

fclean: clean
	@echo "$(COLOUR_DARK_GRAY)Cleaning binaries...$(COLOUR_END)"
	@$(RM) $(NAME)
	@echo "$(RED_FLASH)↪$(RESET_FLASH) Finished, $(COLOUR_BLUE)$(NAME) and binaries$(COLOUR_END) have been $(COLOUR_RED)deleted$(COLOUR_END)."

re: fclean all

.PHONY: all clean fclean re bonus norm

########################## Banner ########################## AINSI SHADOW
                                                                                                                                            

BANNER		= "$(COLOUR_CYAN)"   \
"*  ██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗ * \n"\
" * ██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║*  \n"\
"   ██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║  \n"\
"*  ██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝  *\n"\
"   ╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ *  \n"\
"  * ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝   * by Avallet, Toteixei & Rroussel\n"\
"$(COLOUR_END)" 

########################## Banner syntax ########################## 

# BANNER		= "$(COLOUR_CYAN)"   \
# " ██████╗██████╗ ██████╗     ██████╗  ██████╗         ██╗    ███████╗██╗  ██╗ ██████╗  ██╗\n"\
# "██╔════╝██╔══██╗██╔══██╗   ██╔═████╗██╔═████╗       ██╔╝    ██╔════╝╚██╗██╔╝██╔═████╗███║\n"\
# "██║     ██████╔╝██████╔╝   ██║██╔██║██║██╔██║      ██╔╝     █████╗   ╚███╔╝ ██║██╔██║╚██║\n"\
# "██║     ██╔═══╝ ██╔═══╝    ████╔╝██║████╔╝██║     ██╔╝      ██╔══╝   ██╔██╗ ████╔╝██║ ██║\n"\
# "╚██████╗██║     ██║███████╗╚██████╔╝╚██████╔╝    ██╔╝       ███████╗██╔╝ ██╗╚██████╔╝ ██║\n"\
# " ╚═════╝╚═╝     ╚═╝╚══════╝ ╚═════╝  ╚═════╝     ╚═╝        ╚══════╝╚═╝  ╚═╝ ╚═════╝  ╚═╝ rroussel\n"\
# "$(COLOUR_END)" 	

# linux:	--> SHIFT ALT KEY_UP
# mac:		--> ALT CMD KEY_UP
# web:		manytools.org, AINSI Shadow

#  ██████╗  ██████╗ 
# ██╔═████╗██╔═████╗
# ██║██╔██║██║██╔██║
# ████╔╝██║████╔╝██║
# ╚██████╔╝╚██████╔╝
#  ╚═════╝  ╚═════╝ 
#                   
#  ██████╗  ██╗     
# ██╔═████╗███║     
# ██║██╔██║╚██║     
# ████╔╝██║ ██║     
# ╚██████╔╝ ██║     
#  ╚═════╝  ╚═╝     
#                   
#  ██████╗ ██████╗  
# ██╔═████╗╚════██╗ 
# ██║██╔██║ █████╔╝ 
# ████╔╝██║██╔═══╝  
# ╚██████╔╝███████╗ 
#  ╚═════╝ ╚══════╝ 
#                   
#  ██████╗ ██████╗  
# ██╔═████╗╚════██╗ 
# ██║██╔██║ █████╔╝ 
# ████╔╝██║ ╚═══██╗ 
# ╚██████╔╝██████╔╝ 
#  ╚═════╝ ╚═════╝  
#                   
#  ██████╗ ██╗  ██╗ 
# ██╔═████╗██║  ██║ 
# ██║██╔██║███████║ 
# ████╔╝██║╚════██║ 
# ╚██████╔╝     ██║ 
#  ╚═════╝      ╚═╝ 
#                   
#  ██████╗ ███████╗ 
# ██╔═████╗██╔════╝ 
# ██║██╔██║███████╗ 
# ████╔╝██║╚════██║ 
# ╚██████╔╝███████║ 
#  ╚═════╝ ╚══════╝ 
#                   
#  ██████╗  ██████╗ 
# ██╔═████╗██╔════╝ 
# ██║██╔██║███████╗ 
# ████╔╝██║██╔═══██╗
# ╚██████╔╝╚██████╔╝
#  ╚═════╝  ╚═════╝ 
#                   
#  ██████╗ ███████╗ 
# ██╔═████╗╚════██║ 
# ██║██╔██║    ██╔╝ 
# ████╔╝██║   ██╔╝  
# ╚██████╔╝   ██║   
#  ╚═════╝    ╚═╝   
#                   
#  ██████╗  █████╗  
# ██╔═████╗██╔══██╗ 
# ██║██╔██║╚█████╔╝ 
# ████╔╝██║██╔══██╗ 
# ╚██████╔╝╚█████╔╝ 
#  ╚═════╝  ╚════╝  
#                   
#  ██████╗  █████╗  
# ██╔═████╗██╔══██╗ 
# ██║██╔██║╚██████║ 
# ████╔╝██║ ╚═══██║ 
# ╚██████╔╝ █████╔╝ 
#  ╚═════╝  ╚════╝               