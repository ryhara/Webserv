NAME = webserv
SRC = main.cpp

CONFIG = ConfigParse.cpp

SERVER = Server.cpp HTTPRequestParse.cpp  HTTPRequest.cpp HTTPResponse.cpp Kqueue.cpp

UTILS = ft_memset.cpp error.cpp

CGI = CGI.cpp

SRCDIR = ./srcs
CONFIGDIR = ./srcs/config
SERVERDIR = ./srcs/server
UTILSDIR = ./srcs/utils
CGIDIR = ./srcs/cgi

SRCS = $(addprefix $(CONFIGDIR)/, $(CONFIG)) $(addprefix $(SERVERDIR)/, $(SERVER)) $(addprefix $(UTILSDIR)/, $(UTILS)) $(addprefix $(CGIDIR)/, $(CGI))

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o) $(CONFIG:.cpp=.o) $(SERVER:.cpp=.o) $(UTILS:.cpp=.o) $(CGI:.cpp=.o))
DEPENDS = $(OBJS:.o=.d)
CXX = c++
RM = rm -rf
CXXFLAGS = -Wall -Wextra -Werror -std=c++98  -MMD -MP -g
INC = -I$(CONFIGDIR) -I$(SERVERDIR) -I$(SRCDIR) -I$(UTILSDIR) -I$(CGIDIR)

all : $(OBJDIR) $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(CONFIGDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

$(OBJDIR)/%.o: $(SERVERDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

$(OBJDIR)/%.o: $(UTILSDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

$(OBJDIR)/%.o: $(CGIDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

clean :
	$(RM) $(OBJDIR)

fclean : clean
	$(RM) $(NAME)
	$(RM) a.out

re : fclean all

test : all
	./$(NAME) ./config/test.conf

func_test :
	g++ -o a.out ./tests/*_test.cpp $(SRCS) $(INC) -pthread -lgtest_main -lgtest -std=c++14
	@./a.out

use_cfunc :
	nm -u $(NAME) | grep "^_[a-z]"
	@echo "使用していない関数も含まれるので注意"

debug : CXXFLAGS += -D DEBUG -fsanitize=address
debug : re

leak :
	while true; do leaks -q $(NAME); sleep 1; done

.PHONY : all clean fclean re test debug func_test use_cfunc leak

# g++ テストファイル 実装したファイル -pthread -lgtest_main -lgtest -std=c++14 -I(インクルードのパス)