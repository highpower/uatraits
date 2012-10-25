nodejsmoduledir = $(libdir)/node/

nodejsmodule_DATA = uatraits.node

$(nodejsmodule_DATA):
	node-waf configure build

CLEANFILES = *.node

clean-local:
	rm -rf build/
