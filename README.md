# Ohen
Ohen est un jeu en réseau

## Règles
Dans une partie d'Ohen, tout les joueurs possèdent de la vie et du Ohen. 
Quand la vie d'un joueur tombe à zéro, il meurt et ne peut plus jouer.
Quand un joueur atteint le maximum d'Ohen, ou est le dernier à être en vie, il gagne.
Chaque joueur choisit une action, que le serveur va lui faire faire toute les secondes.
Il peut "générer" du Ohen, "attaquer" un joueur, ou se "d�fendre".
Quand un joueur en attaque un autre, il dépense du Ohen proportionnellement à son attaque.
Quand un joueur se défends, il arrète de générer du Ohen et génère de la vie à la place, tout en réduisant les dégâts qu'il se prends.
Il peut également à tout moment, acheter une amélioration contre de l'Ohen, augmentant ses statistiques telles que la régénération d'Ohen, l'attaque, ou la défense.
Ces améliorations augmentent de prix à chaque fois afin d'éviter qu'un joueur d'augmente trop vite une statistique, forçant les joueurs à équilibrer leurs améliorations afin de rester dans la course et permettant une stratégie de rotation des attaques entre les joueurs, visant à ralentir la progression du joueur en tête.

## Implémentations

- [C](ohen-c/README.md)
- [JS](ohen-js/README.md)