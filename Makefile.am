DATE = $(shell date +%Y%m%d)

SUBDIRS = \
	addons \
	src

DIST_SUBDIRS = \
	$(SUBDIRS) 

EXTRA_DIST = \
	include

all:
	cp -r ChangeLog addons/pvr.vuplus/changelog.txt

dist-zip: all
	cp -r addons/pvr.vuplus .
	$(STRIP) -s -x pvr.vuplus/XBMC_vuplus.pvr
	zip -9 -r ./pvr.vuplus-$(VERSION)-$(build_cpu).zip pvr.vuplus --exclude \*.am \*.in \*Makefile \*.gitignore
	rm -Rf pvr.vuplus

dist-zip-daily: all
	cp -r addons/pvr.vuplus .
	cat pvr.vuplus/addon.xml | sed "s/$(VERSION)/$(VERSION).$(DATE)/g" > pvr.vuplus/addon.xml.new
	mv pvr.vuplus/addon.xml.new pvr.vuplus/addon.xml
	$(STRIP) -s -x pvr.vuplus/XBMC_vuplus.pvr
	zip -9 -r ./pvr.vuplus-$(VERSION).$(DATE)-$(build_cpu).zip pvr.vuplus --exclude \*.am \*.in \*Makefile \*.gitignore
	rm -Rf pvr.vuplus
