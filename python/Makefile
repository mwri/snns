.PHONY: tarball

SNNSDIR="SNNSv4.2"
TOCOPY = setup.py ChangeLog README Makefile examples snns

tarball :
	rm -rf dist
	mkdir -p dist/$(SNNSDIR)/python
	cp -a $(TOCOPY) dist/$(SNNSDIR)/python/
	rm -rf `find dist -name "*.pyc" -o -name "CVS"`
	tar -C dist -cvzf dist/PySNNS-`date +"%G%m%d"`.tar.gz $(SNNSDIR)
	rm -rf dist/$(SNNSDIR)
