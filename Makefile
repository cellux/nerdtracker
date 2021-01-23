.PHONY: build
build:
	python ninja_gen.py && ninja

.PHONY: clean
clean:
	rm -rf build __pycache__
	rm -f nerdtracker

.PHONY: distclean
distclean: clean
	rm -f build.ninja
