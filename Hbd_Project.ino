// ==========================================
// PROJET : Gâteau d'anniversaire interactif
// ==========================================

// --- LES BIBLIOTHÈQUES (Nos boîtes à outils) ---
// Les "#include" permettent d'ajouter des outils créés par d'autres personnes.
// Au lieu de réinventer la roue pour allumer un pixel, on utilise ces outils !
#include <Wire.h>             // Permet à la carte de communiquer avec l'écran.
#include <Adafruit_GFX.h>     // Outils pour dessiner (lignes, rectangles, cercles).
#include <Adafruit_SH110X.h>  // Outils spécifiques pour faire fonctionner notre écran OLED.

// --- CONFIGURATION DE L'ÉCRAN OLED ---
// On définit (avec #define) des mots qui remplaceront des nombres partout dans le code.
#define SCREEN_WIDTH 128      // Largeur de l'écran en pixels
#define SCREEN_HEIGHT 64      // Hauteur de l'écran en pixels
#define OLED_RESET -1         // Broche de réinitialisation (non utilisée ici, donc -1)
#define SCREEN_ADDRESS 0x3C   // "L'adresse postale" de l'écran pour lui envoyer des infos (Parfois 0x3D)

// On crée l'objet "display" (notre écran virtuel dans le code) en lui donnant nos paramètres.
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- CONFIGURATION DES BROCHES (Où sont branchés nos composants ?) ---
// Le mot "const" veut dire que ce numéro ne changera jamais pendant le programme.
const int BUZZER_PIN = 26;       // Le buzzer (haut-parleur) est sur la broche 26 pour esp32, Pin 6 pour Arduino Uno/Nano
const int FLAME_SENSOR_PIN = 14; // Le capteur de flamme est sur la broche 14 pour esp32, Pin 7 pour Arduino Uno/Nano

// --- DÉFINITION DES NOTES DE MUSIQUE ---
// Chaque note de musique correspond à une fréquence en Hertz (la vitesse de vibration).
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784

// On stocke la mélodie (Joyeux Anniversaire) dans un "tableau". 
// C'est comme une liste de courses, lue de gauche à droite.
int melodie[] = {
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_C5, NOTE_B4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_D5, NOTE_C5,
  NOTE_G4, NOTE_G4, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_F5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_C5
};

// Un autre tableau pour la durée de chaque note. 
// (4 = noire, 8 = croche, 2 = blanche...)
int dureeNotes[] = {
  8, 8, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 2
};

// --- VARIABLE D'ÉTAT (La mémoire du programme) ---
// Un "bool" (booléen) est une variable qui ne peut retenir que deux choses : VRAI (true) ou FAUX (false).
// Au début, la bougie n'est pas soufflée, donc c'est faux (false).
bool bougieSoufflee = false; 


// ==========================================
// FONCTION SETUP : LA PRÉPARATION (Lu 1 seule fois au démarrage)
// ==========================================
void setup() {
  // Ouvre un canal de communication avec l'ordinateur (pour lire des messages d'erreur si besoin)
  Serial.begin(115200);

  // On explique à la carte comment utiliser ses broches :
  pinMode(BUZZER_PIN, OUTPUT);       // OUTPUT (Sortie) : La carte envoie du son au buzzer
  pinMode(FLAME_SENSOR_PIN, INPUT);  // INPUT (Entrée) : La carte écoute/lit le capteur de flamme

  // On tente d'allumer l'écran. Si ça rate (! veut dire "NON"), on bloque tout.
  if(!display.begin(SCREEN_ADDRESS, true)) {
    Serial.println(F("Échec OLED")); // Message envoyé à l'ordinateur
    for(;;); // Boucle infinie : le programme s'arrête ici si l'écran ne marche pas.
  }
  
  // On nettoie l'écran (on efface tout) puis on affiche (display) ce nettoyage.
  display.clearDisplay();
  display.display();
}


// ==========================================
// FONCTION LOOP : L'ACTION PRINCIPALE (Lu en boucle à l'infini)
// ==========================================
void loop() {
  // 1. On va demander au capteur s'il voit du feu.
  // digitalRead lit l'état de la broche : LOW (0, il y a du feu) ou HIGH (1, pas de feu).
  int etatFlamme = digitalRead(FLAME_SENSOR_PIN);

  // ÉTAPE 1 : Si le capteur voit du feu (LOW) ET (&&) que la bougie n'a pas encore été soufflée
  // Le "!" avant bougieSoufflee veut dire "PAS". Donc "si bougieSoufflee est faux".
  if (etatFlamme == LOW && !bougieSoufflee) {
    afficherMessageVoeu(); // On appelle notre fonction qui affiche le texte (voir plus bas)
  }
  
  // ÉTAPE 2 : Si le capteur NE voit PLUS de feu (HIGH) ET qu'on vient juste de la souffler
  else if (etatFlamme == HIGH && !bougieSoufflee) {
    bougieSoufflee = true; // On change la mémoire : OUI, elle a été soufflée ! 
                           // Comme ça, on ne relancera pas la musique en boucle.
    
    afficherGateau(); // On dessine le gâteau
    jouerMusique();   // On joue "Joyeux anniversaire"
  }

  // ÉTAPE 3 : Que se passe-t-il après l'événement ?
  // Si la bougie a bien été soufflée, on fait clignoter un point blanc (flamme virtuelle).
  if (bougieSoufflee) {
    animerFlammeOLED();
  }
}


// ==========================================
// LES FONCTIONS PERSONNALISÉES (Nos mini-programmes)
// ==========================================
// Plutôt que de mettre tout le code complexe dans le "loop", on le range 
// dans des blocs nommés "void ...". Ça rend la lecture beaucoup plus claire !

// --- Fonction pour afficher le texte de départ ---
void afficherMessageVoeu() {
  display.clearDisplay();             // 1. On efface l'écran
  display.setTextSize(1);             // 2. On choisit la taille du texte
  display.setTextColor(SH110X_WHITE); // 3. On choisit la couleur (Blanc sur fond noir)
  
  display.setCursor(25, 20);          // 4. On place le curseur (X horizontal, Y vertical)
  display.println(F("Fais un voeu,"));// 5. On écrit la première phrase
  
  display.setCursor(0, 35);           // 6. On déplace le curseur plus bas
  display.println(F("et souffle ta bougie!")); // 7. On écrit la suite
  
  display.display();                  // 8. IMPORTANT : on envoie tout ça à l'écran pour l'afficher !
}

// --- Fonction pour dessiner un beau gâteau d'anniversaire ---
void afficherGateau() {
  display.clearDisplay();
  
  // Le message de félicitations
  display.setTextSize(1);             
  display.setTextColor(SH110X_WHITE); 
  display.setCursor(25, 2);             
  display.println(F("Happy Birthday!"));
  
  display.setCursor(20, 18);
  display.println(F("Enjoy"));
  display.setCursor(72, 18);
  display.println(F("Your Day!"));
  
  // On dessine avec des coordonnées mathématiques (X, Y, Largeur, Hauteur)
  // Base du gâteau (Un rectangle blanc plein, avec une ligne noire pour faire la déco)
  display.fillRect(34, 45, 60, 15, SH110X_WHITE);
  display.drawLine(34, 52, 94, 52, SH110X_BLACK);

  // Étage supérieur (Un peu plus petit)
  display.fillRect(44, 30, 40, 15, SH110X_WHITE);
  display.drawLine(44, 37, 84, 37, SH110X_BLACK);

  // La bougie posée sur le gâteau (Un petit rectangle)
  display.fillRect(62, 18, 4, 12, SH110X_WHITE);

  display.display(); // On affiche le tout !
}

// --- Fonction pour simuler une petite flamme qui clignote ---
void animerFlammeOLED() {
  // On dessine un cercle plein (fillCircle) en X=64, Y=13, de rayon 3.
  display.fillCircle(64, 13, 3, SH110X_WHITE);
  display.display();
  delay(500); // Le programme fait une pause de 500 millisecondes (0.5 seconde)

  // On dessine un cercle noir par-dessus pour "effacer" la flamme
  display.fillCircle(64, 13, 3, SH110X_BLACK);
  display.display();
  delay(300); // Pause de 300 millisecondes
}

// --- Fonction pour jouer la mélodie au buzzer ---
void jouerMusique() {
  // On calcule combien de notes il y a dans la liste (astuce mathématique)
  int nombreDeNotes = sizeof(melodie) / sizeof(melodie[0]);
  
  // La boucle "for" va se répéter pour chaque note de la musique.
  // Elle commence à i=0, et s'arrête quand on a joué toutes les notes.
  for (int i = 0; i < nombreDeNotes; i++) {
    
    // On calcule combien de temps doit durer la note
    int duree = 1000 / dureeNotes[i];
    
    // "tone" fait sonner le buzzer (Sur quelle broche ? Quelle note ? Combien de temps ?)
    tone(BUZZER_PIN, melodie[i], duree);
    
    // Pour bien séparer les notes, on ajoute une petite pause (1.30 fois la durée de la note)
    int pauseEntreNotes = duree * 1.50;
    delay(pauseEntreNotes);
    
    // "noTone" coupe le son avant de passer à la note suivante
    noTone(BUZZER_PIN);
  }
}
