# Le Gateau 🎂 electronique
![Arduino Compatible](https://img.shields.io/badge/Arduino-Compatible-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Status](https://img.shields.io/badge/Statut-100%25%20Fonctionnel-success?style=for-the-badge)
![License](https://img.shields.io/badge/Licence-MIT-blue?style=for-the-badge)

Bienvenue dans le futur de la fête ! Ce projet transforme une carte microcontrôleur du type ESP32  en un **gâteau d'anniversaire interactif**. 

Soufflez sur le capteur de flamme pour éteindre la bougie virtuelle, déclencher un superbe gâteau en pixel-art sur l'écran OLED et lancer l'incontournable mélodie *"Joyeux Anniversaire"*.

---

## Fonctionnalités

* **Détection en temps réel :** Surveillance active d'une vraie flamme (allumette ou briquet) via un capteur infrarouge.
* **Affichage Dynamique :** Écran OLED qui passe d'une invite de commande à une animation festive de gâteau avec flamme clignotante.
* **Rétro-Musique :** Un buzzer piézo-électrique joue la mélodie complète de "Happy Birthday" dès que la bougie est "soufflée".
* **Mémoire d'État :** Le système est intelligent ; il ne joue la musique qu'une seule fois pour éviter de saturer vos oreilles !

---

## Matériel Requis & Branchements

### Composants
* **1x** Microcontrôleur (compatible Arduino/ESP32)
* **1x** Écran OLED 1.3 inch, SH1106 (128x64, communication I2C)
* **1x** Capteur de flamme infrarouge
* **1x** Buzzer passif
* Câbles de prototypage (Jumpers)

### Schéma de câblage
Voici la configuration des broches définie par défaut dans le code :

| Composant | Broche Composant | Broche Carte (Exemple ESP32) |
| :--- | :--- | :--- |
| **Écran OLED** | VCC / GND | 3.3V / GND |
| | SDA / SCL | GPIO 21/22 |
| **Buzzer** | + (Signal) | **GPIO 26** |
| | - (GND) | GND |
| **Capteur Flamme** | DO (Digital Out) | **GPIO 14** |
| | VCC / GND | 5V ou 3.3V / GND |

---

## 💻 Installation et Configuration

Pour faire fonctionner ce projet chez vous, suivez ces étapes simples :

### 1. Préparation de l'IDE Arduino
Assurez-vous d'avoir téléchargé l'**IDE Arduino**. Si vous utilisez une carte ESP32, n'oubliez pas d'ajouter le gestionnaire de carte correspondant dans vos préférences.

### 2. Installation des Bibliothèques
Ouvrez le gestionnaire de bibliothèques de l'IDE (`Outils > Gérer les bibliothèques`) et installez les outils suivants :
1.  **Wire** (généralement incluse par défaut).
2.  **Adafruit GFX Library** (le moteur graphique pour dessiner).
3.  **Adafruit SH110X** (le pilote spécifique pour notre écran OLED).

### 3. Téléversement
1. Clonez ce dépôt ou copiez le code du fichier `Hbd_Project`.
2. Ouvrez le fichier `Hbd_Project.ino` dans votre IDE.
3. Connectez votre carte en USB, sélectionnez le bon port de communication et cliquez sur **Téléverser**.

---

## Architecture du Code

Le code a été pensé pour être le plus lisible et modulaire possible. Il se divise en 3 grandes étapes logiques :

```text
[ SETUP ] Initialisation de l'OLED, du Buzzer et du Capteur de flamme.
    |
[ LOOP  ] Écoute constante du capteur (digitalRead).
    |
    +---> ÉTAPE 1 : Flamme détectée -> "Fais un voeu..." (afficherMessageVoeu)
    |
    +---> ÉTAPE 2 : Flamme éteinte  -> Dessin du gâteau + Musique (afficherGateau + jouerMusique)
    |
    +---> ÉTAPE 3 : Mode Post-Vœu  -> La bougie OLED clignote à l'infini (animerFlammeOLED)

```

### Zoom sur les fonctions personnalisées :

* `afficherMessageVoeu()` : Nettoie l'écran et écrit les instructions textuelles de départ.
* `afficherGateau()` : Utilise des primitives graphiques (`fillRect`, `drawLine`) pour concevoir le gâteau pixel par pixel directement dans la mémoire de l'écran.
* `animerFlammeOLED()` : Alterne entre un cercle blanc et un cercle noir toutes les quelques millisecondes pour simuler le vacillement de la bougie.
* `jouerMusique()` : Parcourt un tableau de fréquences (Hertz) à un rythme mathématiquement calculé pour cadencer les noires et les croches de la mélodie.

---


Développé avec le ❤️ pour rendre les anniversaires un peu plus *geek*. Si ce projet vous a plu, n'hésitez pas à laisser une ⭐️ sur ce dépôt !

```

```
