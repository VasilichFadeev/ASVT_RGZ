run:
	g++ -Wall -Wextra -Werror -std=c++20 -fPIC \
		main.cpp picture.cpp view.cpp \
		$(shell pkg-config --cflags Qt5Widgets Qt5Gui Qt5Core) \
		$(shell pkg-config --libs Qt5Widgets Qt5Gui Qt5Core) \
		-o app

	./app

clean:
	rm -rf ./app

clang-format:
	cd linters && clang-format -i ./../*.cpp ./../*.h
