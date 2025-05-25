# 🎮 Project V - Jeu en C++/Qt

Bienvenue dans **Project V**, un jeu 2D développé en C++ utilisant **Qt** pour la partie graphique. Incarnez **Butcher** dans un univers inspiré de la série *The Boys* : combattez vos ennemis, débloquez des pouvoirs, et tentez de vaincre les redoutables boss !

---

## 🛠️ Compilation et Lancement

### ✅ Prérequis

Assurez-vous d'avoir les éléments suivants installés sur votre système **Linux** :

- Qt 5 ou 6 (recommandé : Qt 6)
- qmake
- make
- g++ ou clang++
- (Optionnel) Qt Creator pour une compilation plus simple via une interface graphique

### 📦 Installation

1. **Télécharger l'archive ZIP du projet** et l'extraire :

```bash
unzip ProjectV.zip
cd ProjectV
```

2. **Compiler avec qmake** :

```bash
qmake6 -project
qmake6
````
-Dans le fichier Project.pro, rajoutez à la fin : 
```
QT += core widgets multimedia
```

```bash
make
```
Le jeu doit compiler sans problème.

3. **Lancer le jeu** :

```bash
./ProjectV
```

---

## 🎮 Règles du Jeu

### Objectif

Incarnez **Butcher**, un justicier solitaire, et survivez à des vagues d'ennemis tout en affrontant des boss.

### Contrôles

- **ZQSD** : déplacer le personnage
- **Souris** : viser
- **Clic gauche** : tirer
- **Pavé numérique** : Sélection d'arme : 1 Couteau - 2 Pistolet - 3 Fusil à pompe.

### Gameplay

- Vous commencez sans arme.
- Vous débloquez progressivement des armes  :
  - **Pistolet** (attaque à mi-distance)
  - **Fusil à pompe** (attaque à courte distance)
- L’univers est vu du dessus, avec un style **pixel art**.
- Le jeu s’arrête lorsque vous mourrez : un écran de mort affiche votre score, avec possibilité de le sauvegarder.

### Boss

Trois boss apparaissent au fil du jeu :
- **Zombie** : attaque au corp à corp, dégat faible.
- **Vampire** : attaque à distance, dégat moyen.
- **Reaper** : dangereux et imprévisible

---

## 💾 Fichiers Importants

- `main.cpp` : point d’entrée du jeu
- `GameManager.cpp/h` : logique centrale du jeu
- `Player.cpp/h` : gestion du joueur
- `HUD.cpp/h` : affichage du score, barre de vie, minuteur
- `src/` : ressources graphiques et sons
- `map.json` : carte du jeu exportée depuis **Tiled**

---

## 📄 Licence

Ce projet est à but pédagogique et n’est pas destiné à une distribution commerciale.
La carte du jeu provient de 'bluecarrot16' sur reddit.

