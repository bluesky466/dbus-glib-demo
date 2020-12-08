CC = gcc

DBUS_CC_FLAGS = $(shell pkg-config --cflags glib-2.0 gthread-2.0 dbus-1 dbus-glib-1)
DBUS_LD_FLAGS = $(shell pkg-config --libs glib-2.0 gthread-2.0 dbus-1 dbus-glib-1)

DBUS_XML = demo.xml


TARGET_SERVER_BIN = server
DBUS_SERVER_GLUE = demo_server_glue.h
EXEC_SERVER_SRCS = demo_server.c
EXEC_SERVER_INCLUDE = demo.h \
			demo_server.h

TARGET_CLIENT_BIN = client
DBUS_CLIENT_GLUE = demo_client_glue.h
EXEC_CLIENT_SRCS = demo_client.c
EXEC_CLIENT_INCLUDE = demo.h

all: $(TARGET_SERVER_BIN) $(TARGET_CLIENT_BIN)

$(TARGET_SERVER_BIN) : $(EXEC_SERVER_SRCS) $(DBUS_SERVER_GLUE) $(EXEC_SERVER_INCLUDE)
	sudo gcc $(EXEC_SERVER_SRCS) $(DBUS_CC_FLAGS) $(DBUS_LD_FLAGS) -Iinclude -o $(TARGET_SERVER_BIN) -g

$(DBUS_SERVER_GLUE) : $(DBUS_XML)
	$(LIBTOOL) --mode=execute dbus-binding-tool --prefix=demo --mode=glib-server --output=$(DBUS_SERVER_GLUE) $(DBUS_XML)


$(TARGET_CLIENT_BIN) : $(EXEC_CLIENT_SRCS) $(DBUS_CLIENT_GLUE) $(EXEC_CLIENT_INCLUDE)
	sudo gcc $(EXEC_CLIENT_SRCS) $(DBUS_CC_FLAGS) $(DBUS_LD_FLAGS) -Iinclude -o $(TARGET_CLIENT_BIN) -g

$(DBUS_CLIENT_GLUE) : $(DBUS_XML)
	$(LIBTOOL) --mode=execute dbus-binding-tool --prefix=demo --mode=glib-client --output=$(DBUS_CLIENT_GLUE) $(DBUS_XML)

