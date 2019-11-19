all:
	make logic

#ExampleTddTest:
#	make clean &&
#	make -i -f ExampleTddTest.mk &&


clean:
	rm -rf objs lib *_tests
#	rm -rf objs/src objs/tests *_tests

logic:
	make clean
#	make gcov
	make -i -f logic.mk
#	python -m gcovr --html-details -o out/logic/out.html

