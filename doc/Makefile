TAG     := $(shell git describe | sed 's/-.*//')
DATE    := $(shell git log -1 --format=%ai $(TAG) | sed 's/-/ /g' | awk '{printf "\\\\newdate{tagDate}{%s}{%s}{%s}\n", $$3, $$2, $$1}')
VERSION := $(shell git describe | sed 's/^[vV]//; s/-.*//')

all: midRoot.tex mr.tex header.tex
	printf "%s\n" $(VERSION) > version.tex
	printf "%s\n" $(DATE)    > date.tex
	latex midRoot
	bibtex midRoot
	latex midRoot
	latex midRoot
	dvipdf midRoot
mr.tex: ../src/midRoot.org smallU.ps smallR.ps smallR2.ps smallR3.ps
	bash ../scripts/org2nw ../src/midRoot.org | noweave -n -index -autodefs c > mr.tex
smallU.ps:
	neato -Tps -o smallU.ps  small.dot	
smallR.ps:
	dot -Tps -o smallR.ps smallR.dot				
smallR2.ps:
	dot -Tps -o smallR2.ps smallR2.dot
smallR3.ps:
	dot -Tps -o smallR3.ps smallR3.dot
clean:
	rm -f mr.tex midRoot.pdf *.ps
