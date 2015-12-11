all:
	cd src; make build_tsdistance

install:
	cd src; python setup.py install

uninstall:
	cd ./src; python setup.py install --record log; cat log | xargs rm -rf

clean:
	cd src; make clean

