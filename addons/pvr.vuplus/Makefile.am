addondir = $(prefix)/addons/pvr.vuplus

addon_DATA = \
	addon.xml \
	XBMC_vuplus.pvr 

XBMC_vuplus.pvr:
	make -C ../../src

install-data-hook: XBMC_vuplus.pvr
	-cp -Rf $(srcdir)/resources $(DESTDIR)$(addondir)
	chmod -R +w $(DESTDIR)$(addondir)/resources

uninstall-hook:
	rm -Rf $(DESTDIR)$(addondir)/resources

EXTRA_DIST = \
	resources \
	addon.xml 

