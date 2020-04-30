CC=g++


MUSIC_INC_ROOT = -I../music-lib-cpp-linux-v1.6.0/include -I../music-lib-cpp-linux-v1.6.0/include/curl 
MUSIC_LIB_ROOT = -L../music-lib-cpp-linux-v1.6.0/lib -lmusic-client-v1.6.0 -lcurl -lIce -lIceUtil -lm


CFLAGS= -w -c -g -O2 

OBJECTS = CIMISSMain.o


Target = CIMISSDown.e


.SUFFIXES: .cpp
.cpp.o : 
	$(CC) $(CFLAGS) $(MUSIC_INC_ROOT) $(MUSIC_LIB_ROOT)  -lstdc++ $*.cpp -o $*.o
  
CIMISSDown:$(OBJECTS)
	$(CC) $(MUSIC_INC_ROOT) $(OBJECTS) $(MUSIC_LIB_ROOT) -lstdc++ -O0 -g -o $(Target)
	rm -f $(OBJECTS)
  
clean:
	rm -f $(OBJECTS) *.e  
	rm -f $(OBJECTS) *.o
