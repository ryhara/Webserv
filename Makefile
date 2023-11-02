NAME = webserv
SRC = main.cpp \










SRCDIR = srcs
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(SRC:%.cpp=%.o))
CXX = c++
RM = rm -rf
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
INC = -I./includes

all : $(OBJDIR) $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

clean :
	$(RM) $(OBJDIR)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re