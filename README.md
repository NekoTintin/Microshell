# Microshell
Un Microshell pour 42 (exam rank 04)

## Description
Microshell est un shell minimaliste qui permet d'exécuter des commandes avec les fonctionnalités suivantes :
- **Exécution de commandes** avec leurs arguments
- **Séparateur `;`** pour exécuter plusieurs commandes séquentiellement
- **Pipes `|`** pour rediriger la sortie d'une commande vers l'entrée de la suivante
- **Built-in `cd`** pour changer de répertoire

## Fonctionnalités
### Commande cd
```bash
./microshell cd /path/to/directory ";" ls
```
- Change le répertoire courant
- Gestion des erreurs (nombre d'arguments incorrect, répertoire inexistant)

### Séparateur `;`
```bash
./microshell ls -la ";" pwd ";" echo "Hello World"
```
Exécute les commandes l'une après l'autre.

### Pipes `|`
```bash
./microshell /bin/ls "|" /usr/bin/grep "microshell" ";" /bin/echo "Done"
```
Redirige la sortie standard d'une commande vers l'entrée de la suivante.

## Limitations
- Pas de gestion du code retour
- Pas de restauration de STDIN
- Chemins absolus ou courants nécessaires pour les commandes externes (ex: `/bin/ls` au lieu de `ls` ou `./ls`)

## Compilation et tests
### Compilation
```bash
bash compile.sh
```
Compile avec les flags `-Wall -Werror -Wextra`.

### Exécution
```bash
./microshell /bin/ls ";" /bin/pwd
```

### Test avec Valgrind
```bash
valgrind --track-fds=all ./microshell /bin/echo "test"
```
Vérifie les fuites mémoire et les file descriptors non fermés.

## Exemples
```bash
# Commande simple
./microshell /bin/echo "Hello 42"

# Plusieurs commandes
./microshell /bin/ls ";" /bin/pwd ";" /bin/echo "End"

# Avec pipes
./microshell /bin/cat microshell.c "|" /usr/bin/grep "print_error"

# Avec cd
./microshell cd .. ";" /bin/pwd

# Combinaison pipes et point-virgule
./microshell /bin/ls "|" /usr/bin/grep ".c" ";" /bin/echo "Files found"
```
