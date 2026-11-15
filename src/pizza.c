#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <emscripten.h>

/* 
 * BEGINNERS GUIDE:
 * - 'const char*' is a pointer to a string of characters that cannot be changed.
 * - 'static const char* name[]' is an array of strings stored in the program's memory.
 * - 'rand()' generates a random number; 'srand()' seeds it so we get different results each time.
 * - 'EMSCRIPTEN_KEEPALIVE' tells the compiler not to remove this function even if it's not 
 *   used inside the C code, because it will be called from JavaScript.
 */

// --- Pizza Data ---

static const char* meats[] = {
    "Pepperoni", "Sausage", "Fennel Sausage", "Spicy Sausage", "Chicken", "BBQ Chicken",
    "Chorizo", "Chicken Andouille", "Salami", "Tofu", "Bacon", "Canadian Bacon",
    "Proscuitto", "Italian Sausage", "Ground Beef", "Anchovies", "Turkey", "Ham",
    "Venison", "Lamb", "Duck", "Soylent Green", "Carne Asada", "Soppressata Picante",
    "Coppa", "Pancetta", "Bresola", "Lox", "Guanciale", "Chili", "Beef Jerky",
    "Pastrami", "Kielbasa", "Scallops", "Filet Mignon"
};

static const char* nonMeats[] = {
    "White Onions", "Red Onions", "Sauteed Onions", "Green Peppers", "Red Peppers",
    "Banana Peppers", "Ghost Peppers", "Habanero Peppers", "Jalapeno Peppers",
    "Stuffed Peppers", "Spinach", "Tomatoes", "Pineapple", "Pear Slices", "Apple Slices",
    "Mushrooms", "Arugula", "Basil", "Fennel", "Rosemary", "Cilantro", "Avocado",
    "Guacamole", "Salsa", "Swiss Chard", "Kale", "Sun Dried Tomatoes", "Walnuts",
    "Artichoke", "Asparagus", "Caramelized Onions", "Mango", "Garlic", "Olives",
    "Cauliflower", "Polenta", "Fried Egg", "Zucchini", "Hummus"
};

static const char* cheeses[] = {
    "American Cheese", "Swiss Cheese", "Goat Cheese", "Mozzarella Cheese", "Parmesean Cheese",
    "Velveeta Cheese", "Gouda Cheese", "Muenster Cheese", "Applewood Cheese", "Asiago Cheese",
    "Bleu Cheese", "Boursin Cheese", "Brie Cheese", "Cheddar Cheese", "Chevre Cheese",
    "Havarti Cheese", "Jack Cheese", "Pepper Jack Cheese", "Gruyere Cheese", "Limberger Cheese",
    "Manchego Cheese", "Marscapone Cheese", "Pecorino Cheese", "Provolone Cheese", "Queso Cheese",
    "Roquefort Cheese", "Romano Cheese", "Ricotta Cheese", "Smoked Gouda"
};

static const char* sauces[] = {
    "Red Sauce", "Marinara", "BBQ Sauce", "No Sauce", "Hot Sauce"
};

static const char* crusts[] = {
    "White Crust", "Whole Wheat Crust", "Flatbread Crust", "Stuffed Crust"
};

// --- Name Generator Data ---

static const char* adj_dark[] = {"dark","morbid", "scary", "spooky", "gothic", "deviant", "creepy", "sadistic", "black", "dangerous", "dejected", "haunted", "morose", "tragic", "shattered", "broken", "sad", "melancholy", "somber", "dark", "gloomy", "homicidal", "murderous", "shady", "misty", "dusky", "ghostly", "shadowy", "demented", "cursed", "insane", "possessed", "grotesque", "obsessed"};
static const char* adj_color[] = {"blue", "green", "purple", "grey", "scarlet", "NeonGreen", "NeonBlue", "NeonPink", "HotPink", "pink", "black", "red", "maroon", "silver", "golden", "yellow", "orange", "mustard", "plum", "violet", "cerulean", "brown", "lavender", "violet", "magenta", "chestnut", "rosy", "copper", "crimson", "teal", "indigo", "navy", "azure", "periwinkle", "brassy", "verdigris", "veridian", "tan", "raspberry", "beige", "sandy", "ElectricBlue", "white", "champagne", "coral", "cyan"};
static const char* adj_whimsical[] = {"whimsical", "silly", "drunken", "goofy", "funny", "weird", "strange", "odd", "playful", "clever", "boastful", "breakdancing", "hilarious", "conceited", "happy", "comical", "curious", "peculiar", "quaint", "quirky", "fancy", "wayward", "fickle", "yawning", "sleepy", "cockeyed", "dizzy", "dancing", "absurd", "laughing", "hairy", "smiling", "perplexed", "baffled", "cockamamie", "vulgar", "hoodwinked", "brainwashed"};
static const char* adj_shiny[] = {"sapphire", "opal", "silver", "gold", "platinum", "ruby", "emerald", "topaz", "diamond", "amethyst", "turquoise", "starlit", "moonlit", "bronze", "metal", "jade", "amber", "garnet", "obsidian", "onyx", "pearl", "copper", "sunlit", "brass", "brassy", "metallic"};
static const char* adj_noisy[] = {"untuned", "loud", "soft", "shrieking", "melodious", "musical", "operatic", "symphonic", "dancing", "lyrical", "harmonic", "orchestral", "noisy", "dissonant", "rhythmic", "hissing", "singing", "crooning", "shouting", "screaming", "wailing", "crying", "howling", "yelling", "hollering", "caterwauling", "bawling", "bellowing", "roaring", "squealing", "beeping", "knocking", "tapping", "rapping", "humming", "scatting", "whispered", "whispering", "rasping", "buzzing", "whirring", "whistling", "whistled"};
static const char* adj_apocalyptic[] = {"nuclear", "apocalyptic", "desolate", "atomic", "zombie", "collapsed", "grim", "fallen", "collapsed", "cannibalistic", "radioactive", "toxic", "poisonous", "venomous", "disastrous", "grimy", "dirty", "undead", "bloodshot", "rusty", "glowing", "decaying", "rotten", "deadly", "plagued", "decimated", "rotting", "putrid", "decayed", "deserted", "acidic"};
static const char* adj_insulting[] = {"stupid", "idiotic", "fat", "ugly", "hideous", "grotesque", "dull", "dumb", "lazy", "sluggish", "brainless", "slow", "gullible", "obtuse", "dense", "dim", "dazed", "ridiculous", "witless", "daft", "crazy", "vapid", "inane", "mundane", "hollow", "vacuous", "boring", "insipid", "tedious", "monotonous", "weird", "bizarre", "backward", "moronic", "ignorant", "scatterbrained", "forgetful", "careless", "lethargic", "insolent", "indolent", "loitering", "gross", "disgusting", "bland", "horrid", "unseemly", "revolting", "homely", "deformed", "disfigured", "offensive", "cowardly", "weak", "villainous", "fearful", "monstrous", "unattractive", "unpleasant", "nasty", "beastly", "snide", "horrible", "syncophantic", "unhelpful", "bootlicking"};
static const char* adj_praise[] = {"beautiful", "intelligent", "smart", "genius", "ingenious", "gorgeous", "pretty", "witty", "angelic", "handsome", "graceful", "talented", "exquisite", "enchanting", "fascinating", "interesting", "divine", "alluring", "ravishing", "wonderful", "magnificient", "marvelous", "dazzling", "cute", "charming", "attractive", "nifty", "delightful", "superior", "amiable", "gentle", "heroic", "courageous", "valiant", "brave", "noble", "daring", "fearless", "gallant", "adventurous", "cool", "enthusiastic", "fierce", "awesome", "radical", "tubular", "fearsome", "majestic", "grand", "stunning"};
static const char* adj_scientific[] = {"scientific", "technical", "digital", "programming", "calculating", "formulating", "cyberpunk", "mechanical", "technological", "innovative", "brainy", "chemical", "quantum", "astro", "space", "theoretical", "atomic", "electronic", "gaseous", "investigative", "solar", "extinct", "galactic"};

static const char* noun_animals[] = {"flamingo", "hedgehog", "owl", "elephant", "pussycat", "alligator", "dachsund", "poodle", "beagle", "crocodile", "kangaroo", "wallaby", "woodpecker", "eagle", "falcon", "canary", "parrot", "parakeet", "hamster", "gerbil", "squirrel", "rat", "dove", "toucan", "raccoon", "vulture", "peacock", "goldfish", "rook", "koala", "skunk", "goat", "rooster", "fox", "porcupine", "llama", "grasshopper", "gorilla", "monkey", "seahorse", "wombat", "wolf", "giraffe", "badger", "lion", "mouse", "beetle", "cricket", "nightingale", "hawk", "trout", "squid", "octopus", "sloth", "snail", "locust", "baboon", "lemur", "meerkat", "oyster", "frog", "toad", "jellyfish", "butterfly", "caterpillar", "tiger", "hyena", "zebra", "snail", "pig", "weasel", "donkey", "penguin", "crane", "buzzard", "vulture", "rhino", "hippopotamus", "dolphin", "sparrow", "beaver", "moose", "minnow", "otter", "bat", "mongoose", "swan", "firefly", "platypus"};
static const char* noun_profession[] = {"doctor", "lawyer", "ninja", "writer", "samurai", "surgeon", "clerk", "artist", "actor", "engineer", "mechanic", "comedian", "fireman", "nurse", "RockStar", "musician", "carpenter", "plumber", "cashier", "electrician", "waiter", "president", "governor", "senator", "scientist", "programmer", "singer", "dancer", "director", "mayor", "merchant", "detective", "investigator", "navigator", "pilot", "priest", "cowboy", "stagehand", "soldier", "ambassador", "pirate", "miner", "police"};
static const char* noun_fantasy[] = {"centaur", "wizard", "gnome", "orc", "troll", "sword", "fairy", "pegasus", "halfling", "elf", "changeling", "ghost", "knight", "squire", "magician", "witch", "warlock", "unicorn", "dragon", "wyvern", "princess", "prince", "king", "queen", "jester", "tower", "castle", "kraken", "seamonster", "mermaid", "psychic", "seer", "oracle"};
static const char* noun_music[] = {"violin", "flute", "bagpipe", "guitar", "symphony", "orchestra", "piano", "trombone", "tuba", "opera", "drums", "harpsichord", "harp", "harmonica", "accordion", "tenor", "soprano", "baritone", "cello", "viola", "piccolo", "ukelele", "woodwind", "saxophone", "bugle", "trumpet", "sousaphone", "cornet", "stradivarius", "marimbas", "bells", "timpani", "bongos", "clarinet", "recorder", "oboe", "conductor", "singer"};
static const char* noun_horror[] = {"murderer", "chainsaw", "knife", "sword", "murder", "devil", "killer", "psycho", "ghost", "monster", "godzilla", "werewolf", "vampire", "demon", "graveyard", "zombie", "mummy", "curse", "death", "grave", "tomb", "beast", "nightmare", "frankenstein", "specter", "poltergeist", "wraith", "corpse", "scream", "massacre", "cannibal", "skull", "bones", "undertaker", "zombie", "creature", "mask", "psychopath", "fiend", "satanist", "moon", "fullMoon"};
static const char* noun_gross[] = {"slime", "bug", "roach", "fluid", "pus", "booger", "spit", "boil", "blister", "orifice", "secretion", "mucus", "phlegm", "centipede", "beetle", "fart", "snot", "crevice", "flatulence", "juice", "mold", "mildew", "germs", "discharge", "toilet", "udder", "odor", "substance", "fluid", "moisture", "garbage", "trash", "bug"};
static const char* noun_everyday[] = {"mirror", "knife", "fork", "spork", "spoon", "tupperware", "minivan", "suburb", "lamp", "desk", "stereo", "television", "TV", "book", "car", "truck", "soda", "door", "video", "game", "computer", "calender", "tree", "plant", "flower", "chimney", "attic", "kitchen", "garden", "school", "wallet", "bottle"};
static const char* noun_jewelry[] = {"earrings", "ring", "necklace", "pendant", "choker", "brooch", "bracelet", "cameo", "charm", "bauble", "trinket", "jewelry", "anklet", "bangle", "locket", "finery", "crown", "tiara", "blingBling", "chain", "rosary", "jewel", "gemstone", "beads", "armband", "pin", "costume", "ornament", "treasure"};
static const char* noun_places[] = {"swamp", "graveyard", "cemetery", "park", "building", "house", "river", "ocean", "sea", "field", "forest", "woods", "neighborhood", "city", "town", "suburb", "country", "meadow", "cliffs", "lake", "stream", "creek", "school", "college", "university", "library", "bakery", "shop", "store", "theater", "garden", "canyon", "highway", "restaurant", "cafe", "diner", "street", "road", "freeway", "alley"};
static const char* noun_scifi[] = {"robot", "alien", "raygun", "spaceship", "UFO", "rocket", "phaser", "astronaut", "spaceman", "planet", "star", "galaxy", "computer", "future", "timeMachine", "wormHole", "timeTraveler", "scientist", "invention", "martian", "pluto", "jupiter", "saturn", "mars", "quasar", "blackHole", "warpDrive", "laser", "orbit", "gears", "molecule", "electron", "neutrino", "proton", "experiment", "photon", "apparatus", "universe", "gravity", "darkMatter", "constellation", "circuit", "asteroid"};

// Helper to capitalize first letter
void capitalize(char* str) {
    if (str && str[0]) {
        str[0] = (str[0] >= 'a' && str[0] <= 'z') ? (str[0] - 32) : str[0];
    }
}

// --- Logic Functions ---

EMSCRIPTEN_KEEPALIVE
const char* get_random_pizza_name() {
    static char name_buffer[128];
    
    const char** adj_lists[] = {adj_dark, adj_color, adj_whimsical, adj_shiny, adj_noisy, adj_apocalyptic, adj_insulting, adj_praise, adj_scientific};
    const char** noun_lists[] = {noun_animals, noun_profession, noun_fantasy, noun_music, noun_horror, noun_gross, noun_everyday, noun_jewelry, noun_places, noun_scifi};
    
    int adj_idx = rand() % 9;
    int noun_idx = rand() % 10;
    
    const char** selected_adj = adj_lists[adj_idx];
    const char** selected_noun = noun_lists[noun_idx];
    
    // To capitalize, we need a local copy because the static lists are constants
    char temp_adj[64];
    char temp_noun[64];
    
    strcpy(temp_adj, selected_adj[rand() % 30]); // simplified length
    strcpy(temp_noun, selected_noun[rand() % 30]); // simplified length
    
    capitalize(temp_adj);
    capitalize(temp_noun);
    
    sprintf(name_buffer, "The %s %s", temp_adj, temp_noun);
    return name_buffer;
}

EMSCRIPTEN_KEEPALIVE
void get_random_pizza_ingredients(char* buffer, int buffer_size) {
    int num_meats = rand() % 4;
    int num_non_meats = rand() % 3;
    int num_cheeses = rand() % 2;
    
    int offset = 0;
    
    for (int i = 0; i < num_meats; i++) {
        const char* m = meats[rand() % 35];
        offset += snprintf(buffer + offset, buffer_size - offset, "%s,", m);
    }
    for (int i = 0; i < num_non_meats; i++) {
        const char* nm = nonMeats[rand() % 39];
        offset += snprintf(buffer + offset, buffer_size - offset, "%s,", nm);
    }
    for (int i = 0; i < num_cheeses; i++) {
        const char* c = cheeses[rand() % 29];
        offset += snprintf(buffer + offset, buffer_size - offset, "%s,", c);
    }
    
    const char* s = sauces[rand() % 5];
    offset += snprintf(buffer + offset, buffer_size - offset, "%s,", s);
    
    const char* cr = crusts[rand() % 4];
    offset += snprintf(buffer + offset, buffer_size - offset, "%s", cr);
}

EMSCRIPTEN_KEEPALIVE
float calculate_parallax(float scroll_y, int index) {
    // Equivalent to: Math.sin(current_Scroll_Y + (i % 5))
    // current_Scroll_Y = last_Scroll_Y / 1250
    float current_scroll_y = scroll_y / 1250.0f;
    return sinf(current_scroll_y + (index % 5));
}

// Simple test function for native execution
#ifndef __EMSCRIPTEN__
int main() {
    srand(42);
    printf("Test Name: %s\n", get_random_pizza_name());
    char ingredients[512];
    get_random_pizza_ingredients(ingredients, 512);
    printf("Test Ingredients: %s\n", ingredients);
    printf("Test Parallax: %f\n", calculate_parallax(1000.0f, 2));
    return 0;
}
#endif
