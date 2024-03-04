NAME = webserv
SRC = main.cpp

CONFIG = ConfigParse.cpp Config.cpp Location.cpp ServerConfig.cpp

SERVER = Server.cpp HTTPRequestParse.cpp  HTTPRequest.cpp HTTPResponse.cpp Client.cpp \
	HTTPResponseGET.cpp HTTPResponsePOST.cpp  HTTPResponseDELETE.cpp HTTPResponseHandle.cpp

UTILS = ft_memset.cpp error.cpp nonBlockingFd.cpp ft_to_string.cpp isHex.cpp ft_stoi.cpp

CGI = CGI.cpp

SRCDIR = ./srcs
CONFIGDIR = ./srcs/config
SERVERDIR = ./srcs/server
UTILSDIR = ./srcs/utils
CGIDIR = ./srcs/cgi
EXCEPTIONDIR = ./srcs/exception

SRCS = $(addprefix $(CONFIGDIR)/, $(CONFIG)) $(addprefix $(SERVERDIR)/, $(SERVER)) $(addprefix $(UTILSDIR)/, $(UTILS)) $(addprefix $(CGIDIR)/, $(CGI))

OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o) $(CONFIG:.cpp=.o) $(SERVER:.cpp=.o) $(UTILS:.cpp=.o) $(CGI:.cpp=.o))
DEPENDS = $(OBJS:.o=.d)
CXX = c++
RM = rm -rf
CXXFLAGS = -Wall -Wextra -Werror -std=c++98  -MMD -MP -g
INC = -I$(CONFIGDIR) -I$(SERVERDIR) -I$(SRCDIR) -I$(UTILSDIR) -I$(CGIDIR) -I$(EXCEPTIONDIR)

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

re : fclean all

allclean : fclean post_clean log_clean
	$(RM) a.out
	$(RM) hoge.dummy
	$(RM) ./tests/python/__pycache__

post_clean :
	$(RM) ./uploads/post_*

log_clean :
	$(RM) ./uploads/log*

test : all
	./$(NAME) ./config/default.conf

func_test :
	g++ -o a.out ./tests/*_test.cpp $(SRCS) $(INC) -pthread -lgtest_main -lgtest -std=c++14
	@./a.out

invalid_test :
	@chmod +x ./tests/invalid_conf_test.sh
	./tests/invalid_conf_test.sh ./config/invalid

python_test :
	@chmod +x ./tests/python_test.sh
	./tests/python_test.sh ./tests/python/

use_cfunc :
	./tests/check_cfunctions.sh $(NAME)

dummy :
	base64 -i /dev/urandom | head -c 10000 > hoge.dummy

debug : CXXFLAGS += -D DEBUG -fsanitize=address
debug : re
	./$(NAME)

leak :
	while true; do leaks -q $(NAME); sleep 1; done

.PHONY : all clean fclean re test debug func_test use_cfunc leak post_clean invalid_test dummy allclean python_test log_clean

# g++ テストファイル 実装したファイル -pthread -lgtest_main -lgtest -std=c++14 -I(インクルードのパス)