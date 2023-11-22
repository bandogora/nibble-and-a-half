CC ?= gcc
DIR ?= .

CFLAGS ?= -O3 -fstack-protector-strong -Wall -Wextra -Wformat -Werror=format-security \
	-D_FORTIFY_SOURCE=2 -fPIC
LDFLAGS ?= -Wl,-z,relro -Wl,-z,now -Wl,--as-needed -pie -L$(DIR) -lc

test_objects = main.o base64.o Timer.o testbase64.o

nibble-and-a-half: base64.h base64.o
	$(CC) $(CFLAGS) base64.o -o $(DIR)/nibble-and-a-half $(LDFLAGS)
	chmod +x $(DIR)/nibble-and-a-half

test: $(test_objects)
	$(CC) $(CFLAGS) $(test_objects) -o $(DIR)/test -Wl,-z,stack-size=138412032 $(LDFLAGS)
	chmod +x $(DIR)/test

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

libnaah64.so: base64.h base64.o
	$(CC) -shared $^ -o $@

libnaah64.a: base64.o
	ar cr $@ $+

.PHONY: clean install

install: nibble-and-a-half
	cp $(DIR)/libnaah64.a ~/.local/lib/
	cp $(DIR)/libnaah64.so ~/.local/lib/

clean:
	rm -rf $(DIR)/nibble-and-a-half $(DIR)/libnaah64.a $(DIR)/libnaah64.so $(DIR)/test $(test_objects)
