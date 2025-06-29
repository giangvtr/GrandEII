# Projet C — Grand EII

## Objectif

Développer un logiciel permettant de contrôler des lumières infrarouges via un Arduino.

---

## 📋 TODO

- [ ] Modifier la norme pour les fichiers JSON : un fichier par groupe de lumières

---

## SETUP POUR PC
### 0. Dépendances:
 - Le programme ne marche que sur un système Linux.
 - Afin d'exectuer GRANDEII_PC, vous aurez besoin de `gtk3` avec `GLib 2.80`.
 - Afin d'avoir les informations sur le coverage, vous aurez besoin de `gcov`
 - Afin d'avoir les informations sur les fuites de mémoires, vous aurez besoin de `valgrind`

### 1. Les branches principales

| Branche         | Description                                               |
|----------------|-----------------------------------------------------------|
| `communication`| Communication entre le logiciel et l’Arduino             |
| `view`         | Interface graphique utilisateur (GUI)                    |
| `model`        | Structures de données et base de données                 |
| `unit-tests`   | Tests unitaires, principalement dans `src/unit_tests/`   |

---

### 2. Cloner une branche

#### a) Première fois

```bash
# Remplacez <branch-name> et <repository-url> par les valeurs appropriées
git clone --single-branch --branch <branch-name> <repository-url>
```

#### b) Si vous avez déjà un dépôt local

```bash
# Toujours tirer les dernières modifications avant de commencer à travailler
git pull origin <branch-name>
```

📌 **Dans CLion** :

- Cliquez sur l’icône Git dans la barre du haut
- Sélectionnez "Update Project" (Ctrl+T)
- Choisissez la branche désirée, puis "Checkout"

---

### 3. Charger les cibles CMake dans CLion

- Naviguez vers `GRANDEII_PC/src`
- Clic droit sur `CMakeLists.txt` > *Reload CMake Project*
- Les cibles apparaîtront automatiquement dans l’interface via l’icône d’exécution

---

### 4. Compiler et exécuter les tests

📌 **Dans CLion** :

- Sélectionnez la cible dans la liste déroulante en haut à droite (ex: `UNIT_TESTS` ou `MAIN_CONSOLE`)
- Cliquez sur le triangle vert ▶️ pour compiler et exécuter
- Vous pouvez également configurer les options de build dans `Run > Edit Configurations`

---

📌 **Dans le Terminal** :
```bash
cd GRAND_EII/src
mkdir build
cd build

# Compilation de base
cmake ..

#Pour activer le mode de DEBUG ainsi de cover:
# Rendez-vous ligne 10 de CMakeLists.txt mettez :
#   option(COVER "Generate coverage data" "ON")
# Mettez :
#   option(COVER "Generate coverage data" "OFF") Si vous ne voulez pas de cela


# Compilation
make

# Exécution
./UNIT_TESTS #Pour les unit tests
./GRANDEII_PC #Pour le programme principal
# Nettoyage (si nécessaire)
./clean_all
```

---

### 5. Générer un rapport de couverture de tests (pour les testeurs)
**Dans CLion** :
- Clic droit sur la cible `UNIT_TESTS` > *Run with Coverage*
- Visualisez les lignes couvertes comme dans le rapport HTML

```bash
# Capturer la couverture (nécessite lcov)
lcov --capture --directory . --output-file coverage.info

# Générer un rapport HTML
genhtml coverage.info --output-directory out

# Ouvrir le rapport (Linux)
xdg-open out/index.html
```

---

### 6. Commit de vos modifications
📌 **Dans CLion** :
- Cliquez sur l’icône Git (colonne de gauche, 2e onglet)
- Cochez les fichiers à inclure
- Rédigez un message de commit
- Cliquez sur "Commit" ou "Commit and Push"

📌 **Dans Terminal** :

#### a) Vérifiez les changements

```bash
git status
git diff
```

#### b) Ajoutez vos fichiers

```bash
# Ajouter un fichier spécifique
git add <file>

# Ajouter tous les fichiers dans le répertoire courant
git add .

# Ajouter uniquement les fichiers modifiés
git add -u
```

#### c) Vérifiez ce qui est mis en scène

```bash
git diff --staged
```

#### d) Faites un commit descriptif

```bash
git commit -m "<message>"
git push origin <branch-name>
# ex : git push origin model
```

💡 *Conseil* : privilégiez des commits petits et ciblés (commits atomiques).

---
## SETUP POUR ARDUINO

Téléchargez Arduino IDE et lancer le programme GRANDEII_ARDUINO.ino