# Makefile for cuda Sieve of Eratosthenes lab, 2020
#
INCLUDE_DIR = -I/usr/local/cuda/include -I/usr/local/include/qtvis  -I/usr/include/x86_64-linux-gnu/qt5/QtOpenGL -I/usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtOpenGL -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -isystem /usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++

LIBS= -L/usr/local/cuda-10.2/targets/x86_64-linux/lib/stubs  -L/usr/local/cuda-10.2/targets/x86_64-linux/lib  /usr/local/lib/libqtvis.a /usr/local/lib/libqtviscuda.a /usr/lib/x86_64-linux-gnu/libQt5OpenGL.so.5.9.5 /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.9.5 /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.9.5 /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.9.5 /usr/lib/x86_64-linux-gnu/libGLX.so /usr/lib/x86_64-linux-gnu/libOpenGL.so -lpthread /usr/lib/x86_64-linux-gnu/libomp.so -lpthread -lcudadevrt -lcudart_static -lrt -lpthread -ldl

DEFINEFLAG = -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_WIDGETS_LIB

CC=gcc
NVCC=nvcc

CFLAGS = -g -fPIC -Wall

TEST1=cudaSieve
# TEST2=testgrid
#
# MAINPROG=main
# CUDAPROG=userBufferKernel

all: $(TEST1)

$(TEST1): $(TEST1).cu
	$(NVCC) -g -o $(TEST1) $(TEST1).cu

# $(TEST2): $(TEST2).cu
# 	$(NVCC) -g -o $(TEST2) $(TEST2).cu
#
# $(MAINPROG): $(MAINPROG).o $(CUDAPROG).o
# 	$(CC) $(CFLAGS) $(DEFINEFLAG) -o $(CUDAPROG) $(CUDAPROG).o $(MAINPROG).o $(INCLUDE_DIR) $(LIB_DIR) $(LIBS)
#
#
# $(CUDAPROG).o: $(CUDAPROG).cu $(CUDAPROG).h
# 	    $(NVCC) -c $(INCLUDE_DIR) $(CUDAPROG).cu
#
# $(MAINPROG).o: $(MAINPROG).cpp
# 	$(CC) $(CFLAGS) $(DEFINEFLAG) -fPIC -c $(INCLUDE_DIR) $(MAINPROG).cpp

clean:
	$(RM) *.o $(TEST1)
