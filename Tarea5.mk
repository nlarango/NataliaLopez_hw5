masas = Grafica.jpg
erase=rm -r -f *.dat a.out *.png *.aux *.log

Resultados_hw5.pdf : Resultados_hw5.tex $(masa)
	pdflatex $<

$(masas): *.dat Plots.py
	python Plots.py

*.dat: CurvaRotacion.x RadialVelocities.dat
	./CurvaRotacion.x

CurvaRotacion.x: CurvaRotacion.c
	gcc -lm CurvaRotacion.c

clean:
	$(erase) *.pdf *.wav
