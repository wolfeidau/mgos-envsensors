restore:
	for f in $$(ls -1 config/*); do echo $$f; mos put $$f; done

build:
	mos build --local --verbose
	mos flash
	mos console
.PHONY: build