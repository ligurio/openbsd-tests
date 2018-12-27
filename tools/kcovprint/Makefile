.PHONY: clean
	 
libkcovprint.so.1.0.0: kcovprint.c 
	$(CC) -fPIC kcovprint.c -shared -Wl,-soname,libkcovprint.so.1 -o libkcovprint.so.1.0.0
		 
clean:
	$(RM) -f libkcovprint.so.1.0.0

