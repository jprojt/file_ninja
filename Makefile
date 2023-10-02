.PHONY: release
release:
	mkdir -p build/release && \
	cd build/release && \
	cmake ../.. && \
	cmake --build . --config Release --target capstone

.PHONY: run_release
run_release: release
	cd build/release && \
	./capstone

.PHONY: debug
debug:
	mkdir -p build/debug && \
	cd build/debug && \
	cmake -DCMAKE_BUILD_TYPE=debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../.. && \
	cmake --build . --config Debug


.PHONY: clean
clean:
	rm -rf build


.PHONY: run_debug
run_debug: debug
	cd build/debug && \
	./capstone


.PHONY: test
test:
	mkdir -p build/test
	cd build/test && \
	cmake ../.. && \
	cmake --build . --config Debug --target test

.PHONY: run_tests
run_tests: test
	cd build/test && \
	./test

