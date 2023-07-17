import re

def removeAll (x, liste) :
    for i in liste :
        if (i == x) :
            liste.remove(x)

def coupureDonnees (liste) :
    l = []
    m = []
    for i in liste:
        try :
            int(i)
        except ValueError :
            if (i != "[%clk" and len(i) > 0 and i[-1] != ']'):
                l.append(i)
    for  i in l :
        m.append(i)
    return m[:12]

def elageur (fichier) :
    fichier = open(fichier, "r")
    file = fichier.read()
    fichier.close()

    file = re.split("\n", file)
    rep  = []

    for texte in file :
        if(len(texte) != 0 and texte[0] != '[') :
            t = re.split("[. {}]", texte)
            t = coupureDonnees(t)
            rep.append(t)
    return rep

def supprimerDoublons(l):
    unique = []
    for i in l :
        if i not in unique :
            unique.append(i)
    return unique
        

bdd0 = elageur("./master_games.txt")
bdd1 = elageur("./master_games(1).txt")
bdd2 = elageur("./master_games(2).txt")
bdd3 = elageur("./master_games(3).txt")

bdd = bdd0 + bdd1 + bdd2 + bdd3

enfin = supprimerDoublons(bdd)
for i in enfin :
  print(i)
    