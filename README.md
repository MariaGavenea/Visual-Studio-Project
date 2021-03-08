# Visual-Studio-Project

Relevante in acest proiect sunt 2 jocuri. Primul, intitulat Tema1, reprezinta un joc cu un arcas care tinteste baloane de o anumita culoare (pentru acumulare de puncte) si care trebuie sa evite shuriken-uri ce vin constant spre el.

Cel de-al doilea joc (Tema3) este un endless runner in care un arici trebuie sa sara de pe o platforma de lemn pe alta, colectand conuri pentru a acumula puncte, fara sa cada in rau si sa evite bolovanii aflati pe unele platforme.

Pentru realizarea acestor doua jocuri am folosit OpengGL. Mi-am creat propriile matrici de modelare, meshe si propria camera. Am invatat sa lucrez cu shadere, sa aplic texturi si modele de iluminare.

# Arcasul (Joc 2D)

**Controlul jucatorului**:
  - pentru miscarea arcului in sus, respectiv in jos, se vor folosi tastele W, respectiv S
  - cu click stanga este aruncata sageata (cu cat este tinut apasat mai mult butonul cu atat viteza sagetii va fi mai mare, pana la o limita prestabilita)
  
**Descriere joc**:

- in partea stanga a ecranului se afla arcasul
- din partea dreapta a ecranului vin shuriken-uri pe 5 linii
- in jumatatea dreapta a ecranului apar 5 baloane (3 galbene si 2 rosii intercalate) care se ridica fiecare pe cate o coloana
- la impactul sagetii cu un balon sau un shuriken, punctajele sunt urmatoarele:
    - shuriken: 10 puncte
		- balon rosu: 30 puncte
		- balon galben: -50 puncte
		- scorul este afisat la consola de fiecare data cand se castiga/pierd puncte
- initial, jucatorul are 3 vieti
- atunci cand jucatorul este lovit de un shuriken, pierde o viata (se afiseaza la consola numarul de vieti ramase)
- jocul se incheie cand jucatorul ramane fara vieti

# Endless runner (Joc 3D)

**Controlul jucatorului**:
 - deplasarea inainte
    - W - mareste viteza de deplasare pana la o viteza limita
    - S - scade viteza de deplasare pana la o viteza limita
 - A - deplasare la stanga
 - D - deplasare la dreapta

 - space - saritura

**Controlul camerei**:

- tasta C - comutare intre Third Person Camera si First Person Camera
  
**Descriere joc**:

Initial, jucatorul nu are nicio viata si combustibilul este plin. Scopul jocului este acela ca jucatorul sa acumuleze cat mai multe colectabile (conuri) fara sa ajunga intr-una din situatiile urmatoare (in care pierde jocul):
- cade de pe platforma
- ramane fara combustibil si nu are vieti care sa il salveze
- loveste un obstacol (bolovan) si nu are vieti care sa il salveze
- ajunge pe o platforma rosie

**Platforme**
- exista 3 coloane cu platforme de diferite culori
- in functie de culoare, platforma ii ofera jucatorului o anumita abilitate sau il incurca
- tipuri de platforme:
   - efect in timp:
        - verde (creste combustibilul)
        - galbena (scade combustibilul mai repede decat ar scadea in mod normal)
        - oranj (blocheaza jucatorul la o viteza apropiata de cea maxima)
   - efect instant:
        - rosie (pierde jocul)
        - roz - adauga vieti pana la 3 (daca jucatorul are 3 vieti aceste placi nu au niciun efect)
        - gri - scad vieti pana la 0 (daca jucatorul are 0 vieti aceste placi nu au niciun efect)
        
 **Elemente decorative**
 
 Jucatorul nu interactioneaza cu aceste obiecte. De o parte si de alta a raului se afla brazi (unii au in varf lumini) si stalpi cu lumini de tip spot. Luminile au diferite culori.
