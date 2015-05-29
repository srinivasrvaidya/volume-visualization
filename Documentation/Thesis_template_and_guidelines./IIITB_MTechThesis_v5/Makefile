SRC=thesis

all: ${SRC}.tex
	pdflatex ${SRC}.tex
	bibtex ${SRC}
	pdflatex ${SRC}.tex
	pdflatex ${SRC}.tex

clean:
	rm *.aux ${SRC}.bbl ${SRC}.blg ${SRC}.lof ${SRC}.log ${SRC}.lot ${SRC}.toc Chapters/*.aux Chapters/Appendix/*.aux

clobber: 
	make clean
	rm ${SRC}.pdf 
