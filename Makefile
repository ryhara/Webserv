NAME = webserv
SRC = main.cpp
CONFIG = ConfigParse.cpp
SERVER = Server.cpp

SRCDIR = ./srcs
CONFIGDIR = ./srcs/config
SERVERDIR = ./srcs/server
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o) $(CONFIG:.cpp=.o) $(SERVER:.cpp=.o))
DEPENDS = $(OBJS:.o=.d)
CXX = c++
RM = rm -rf
CXXFLAGS = -MMD -MP -Wall -Wextra -Werror -std=c++98
DEBUGFLAGS = -g -fsanitize=address
INC = -I$(CONFIGDIR) -I$(SERVERDIR) -I$(SRCDIR)

all : $(OBJDIR) $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(if $(DEBUG), $(DEBUGFLAGS))

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(CONFIGDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(if $(DEBUG), $(DEBUGFLAGS)) -o $@ -c $< $(INC)

$(OBJDIR)/%.o: $(SERVERDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(if $(DEBUG), $(DEBUGFLAGS)) -o $@ -c $< $(INC)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(if $(DEBUG), $(DEBUGFLAGS)) -o $@ -c $< $(INC)

clean :
	$(RM) $(OBJDIR)

fclean : clean
	$(RM) $(NAME)

re : fclean all

test : all
	./$(NAME) ./test.conf

debug : fclean
	make DEBUG=1


.PHONY : all clean fclean re test debug