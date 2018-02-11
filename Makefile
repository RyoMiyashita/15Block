#
# Makefile
#

#最終目的のファイル
TARGET = block.out

# ソースファイル(*.c)の一覧
SRCS = block.c

# オブジェクトファイル(*.o)の一覧
OBJS = ${SRCS:.c=.o}

# ヘッダーファイルの一覧(今は何もなし)
HEADERS =

# コンパイラ・リンカのしの指定
CC = gcc
CCFLAGS = -Wall
LD = gcc
LDFLAGS =
LIBS = -Wall -framework OpenGL -framework GLUT -framework glpng -lm

# OBJSからTARGETを作る方法
$(TARGET) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LIBS)

# *.cから*.oを作る方法
.c.o :
	$(CC) $(CCFLAGS) -c $<

# *.oはHEADERSとMakefileに依存(これらが書き換わった時にも*.oを再構築)
$(OBJS) : $(HEADERS) Makefile

# make cleanした時に実行されるコマンド
clean :
	rm -f $(TARGET) $(OBJS) core *-
