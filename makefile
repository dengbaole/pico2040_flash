

BUILD_DIR = build
PICO_DIR = ./pico

.PHONY: clean default pico rebuild assets

default: pico

assets_oled:
	@echo Create Assets
	./utils_bin/p2a-win-amd64.exe -in ./moudle/ui/oled -out ./moudle/oled_image
	

pico:clean
	cmake . -G Ninja -B$(BUILD_DIR) -S.
	ninja -C $(BUILD_DIR)

clean:
		@echo "Cleaning up build directory..."
	@if exist $(BUILD_DIR) ( \
		rmdir /s /q $(BUILD_DIR) \
	) else ( \
		echo Build directory does not exist: $(BUILD_DIR) \
	)

rebuild: clean pico

format:
	./utils_bin/astyle.exe --project="./utils_bin/.astylerc" -r **.c,**.h --ignore-exclude-errors --exclude=_build --exclude=utils_bin --exclude=dist --exclude=utils -v -Q
