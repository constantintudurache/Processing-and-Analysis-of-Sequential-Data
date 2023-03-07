# Processing-and-Analysis-of-Sequential-Data

Introducere
In ziua de azi, majoritatea problemelor ajung sa fie rezolvate cu tehnici de ınvatare automata.
Pentru ca aceste tehnici sa functioneze, este nevoie de cantitati semnificative de date. Aceste
date provin de obicei dintr-o varietate de surse, iar majoritatea sunt nesigure. Deoarece ın
procesul de ınvatare calitatea datelor determina direct calitatea rezultatelor obtinute, este
foarte important ca datele sa fie corecte si consistente. De aceea, o mare parte din timp,
analistii ıl petrec ıncercand sa corecteze si sa completeze aceste date.
Posibile probleme prezente ıntr-un set de date:
• valori de date ce nu au sens si nu ar trebui sa existe (outlier/valori except, ionale)
• lipsa datelor ın anumite intervale de timp, datorate unor defect, iuni temporare (gaps)
• oscilatia prea mare a valorilor (zgomot/noise)
• datele sunt neuniform distribuite ın timp (ıntr-o secunda avem 5 ınregistrari, ın urmatoarea
secunda 22 ınregistrari)


Mod de rulare
Pentru rezolvarea temei veti avea de urmarit o serie de comenzi pe care le veti primi ca
argumente din linie de comanda. Citirea si scrierea se vor face la stdin respectiv stdout.
Argumentele pe care programul vostru va trebui sa le implementeze sunt:
• e1 - eliminare de exceptii folosind statistici
• e2 - eliminare de zgomot prin filtru median
• e3 - eliminare de zgomot folosind filtrul bazat pe media aritmetica
• u - uniformizarea frecventei
• c - completarea datelor lipsa
• st< delta > - Se vor calcula statisticile. Intervalul
de timp δ va trebui extras din argument.
. / tema1 −−s t1000
Programul vostru trebuie sa poata primi mai multe argumente la un moment dat, iar acestea
vor fi executate ın ordinea ın care apar ın linia de comanda. Comenzile de prelucrare vor
avea forma:
. / tema1 −−command1 −−command2 −−command3
