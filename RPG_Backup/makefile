OBJS = ./src/enemy.o ./src/player.o ./src/battle.o ./src/Safe_Input.o\
		 ./src/skill.o ./src/item.o ./src/FileFunctions.o ./src/Main.o\
		 ./src/areas.o ./src/party.o ./src/game_hub.o ./src/shop.o\
		 ./src/inn.o ./src/Inventory.o ./src/home.o 
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
DS = ./src/
DH = ./headers/

Main: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o Main

enemy.o: $(DS)enemy.cpp $(DH)enemy.h $(DH)skill.h $(DH)FileFunctions.h
	$(CC) $(CFLAGS) $(DS)enemy.cpp -o $(DS)enemy.o

player.o: $(DS)player.cpp $(DH)player.h $(DH)enemy.h $(DH)skill.h $(DH)item.h\
			 $(DH)Safe_Input.h
	$(CC) $(CFLAGS) $(DS)player.cpp -o $(DS)player.o

battle.o: $(DS)battle.cpp $(DH)battle.h $(DH)enemy.h $(DH)player.h $(DH)item.h\
			 $(DH)Safe_Input.h $(DH)party.h
	$(CC) $(CFLAGS) $(DS)battle.cpp -o $(DS)battle.o

Safe_Input.o: $(DS)Safe_Input.cpp $(DH)Safe_Input.h
	$(CC) $(CFLAGS) $(DS)Safe_Input.cpp -o $(DS)Safe_Input.o

skill.o: $(DS)skill.cpp $(DH)skill.h $(DH)player.h $(DH)enemy.h
	$(CC) $(CFLAGS) $(DS)skill.cpp -o $(DS)skill.o

item.o: $(DS)item.cpp $(DH)item.h $(DH)player.h $(DH)enemy.h $(DH)Safe_Input.h
	$(CC) $(CFLAGS) $(DS)item.cpp -o $(DS)item.o

FileFunctions.o: $(DS)FileFunctions.cpp $(DH)FileFunctions.h
	$(CC) $(CFLAGS) $(DS)FileFunctions.cpp -o $(DS)FileFunctions.o

areas.o: $(DS)areas.cpp $(DH)areas.h $(DH)Safe_Input.h $(DH)enemy.h\
			$(DH)player.h $(DH)item.h $(DH)battle.h $(DH)FileFunctions.h\
			$(DH)party.h
	$(CC) $(CFLAGS) $(DS)areas.cpp -o $(DS)areas.o 

party.o: $(DS)party.cpp $(DH)party.h $(DH)player.h $(DH)skill.h $(DH)item.h\
			$(DH)Safe_Input.h
	$(CC) $(CFLAGS) $(DS)party.cpp -o $(DS)party.o

game_hub.o: $(DS)game_hub.cpp $(DH)game_hub.h $(DH)party.h $(DH)areas.h\
				$(DH)Safe_Input.h $(DH)player.h $(DH)shop.h $(DH)inn.h $(DH)home.h
	$(CC) $(CFLAGS) $(DS)game_hub.cpp -o $(DS)game_hub.o

shop.o: $(DS)shop.cpp $(DH)shop.h $(DH)Safe_Input.h $(DH)party.h $(DH)item.h\
		  $(DH)FileFunctions.h 
	$(CC) $(CFLAGS) $(DS)shop.cpp -o $(DS)shop.o

inn.o: $(DS)inn.cpp $(DH)inn.h $(DH)party.h $(DH)player.h $(DH)Safe_Input.h
	$(CC) $(CFLAGS) $(DS)inn.cpp -o $(DS)inn.o

Inventory.o: $(DS)Inventory.cpp $(DH)Inventory.h $(DH)item.h $(DH)Safe_Input.h
	$(CC) $(CFLAGS) $(DS)Inventory.cpp -o $(DS)Inventory.o

home.o: $(DS)home.cpp $(DH)home.h $(DH)player.h $(DH)Inventory.h $(DH)party.h\
		  $(DH)Safe_Input.h $(DH)item.h
	$(CC) $(CFLAGS) $(DS)home.cpp -o $(DS)home.o

Main.o: $(DS)Main.cpp $(DH)battle.h $(DH)enemy.h $(DH)player.h $(DH)item.h\
		  $(DH)areas.h  $(DH)party.h  $(DH)game_hub.h
	$(CC) $(CFLAGS) $(DS)Main.cpp -o $(DS)Main.o


clean:
	rm $(OBJS)

tar:
	tar -zcvf ../Sandbox.tar.gz ../Sandbox/

