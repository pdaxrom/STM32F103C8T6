all:
	make -C blink
	make -C button
	make -C rgbled

clean:
	make -C blink clean
	make -C button clean
	make -C rgbled clean
