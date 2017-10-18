#include <stdio.h>#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "tree.h"
#include "list.h"
#include "utils.h"

typedef struct shelf {
  char* name;
  int amount;
} shelf_t;

typedef struct item {
  char* name;
  char* desc;
  int price;
  list_t* shelves;
} item_t;

struct action {
  enum { NOTHING, EDIT } type;
  union {
    struct { item_t* old; item_t* new; };
  };
};

shelf_t* create_shelf(char* name, int amount) {
  shelf_t* shelf = calloc(1, sizeof(shelf_t));

  if (shelf != NULL) {
    shelf->name = name;
    shelf->amount = amount;
  }

  return shelf;
}

item_t* create_item(char* name, char* desc, int price, list_t* shelves) {
  item_t* item = calloc(1, sizeof(item_t));

  if (item != NULL) {
    item->name = name;
    item->desc = desc;
    item->price = price;
    item->shelves = shelves;
  }

  return item;
}

char ask_question_menu(char* question) {
  char c;
  bool done = false;
  
  do {
    c = toupper(ask_question_char(question));
    
    switch (c) {
    case 'L':
    case 'T':
    case 'R':
    case 'G':
    case 'H':
    case 'A':
      done = true;
      break;
    default:
      puts("Ogiltig inmatning!");
      break;
    }
  } while (!done);

  return c;
}

void print_menu_next() {
  puts("\n[V]isa nästa sida");
  puts("[A]vbryt");
}

item_t* get_item_on_shelf(tree_t* tree, char* shelf_name) {
  int t_size = tree_size(tree);
  char** t_keys = tree_keys(tree);

  for (int i = 0; i < t_size; ++i) {
    char* node_name = t_keys[i];
    item_t* item = tree_get(tree, node_name);
    list_t* shelves = item->shelves;
    int length = list_length(shelves);
    
    for (int j = 0; j < length; ++j) {
      shelf_t* shelf_item = *(list_get(shelves, j));
      char* other_shelf_name = shelf_item->name;
      
      if (strcmp(shelf_name, other_shelf_name) == 0) {
        return item;
      }
    }
  }

  return NULL;
}

void add_shelf_to_item2(tree_t* tree, char* item_name) {
  char* shelf_name = NULL;
  item_t* item = NULL;
  bool available_shelf = false;
  bool shelf_exists = false;

  while (!available_shelf) {
    // loop as long as user enters an already taken shelf
    shelf_name = ask_question_shelf("Hyllplats: ");
    item = get_item_on_shelf(tree, shelf_name);
    
    if (item == NULL) {
      available_shelf = true;
    } else if (strcmp(item->name, item_name) == 0) {
      available_shelf = true;
      shelf_exists = true;
    } else {
      free(shelf_name);
      puts("Hyllan är upptagen!\n");
    }
  }

  if (item == NULL) {
    item = tree_get(tree, item_name);
  }

  int amount = ask_question_int("Antal: ");

  if (shelf_exists) {
    // the shelf already exists, find it and increase its 'amount' value
    list_t* shelves = item->shelves;
    int length = list_length(shelves);

    shelf_t* shelf = NULL;

    for (int i = 0; i < length; ++i) {
      shelf = *(list_get(shelves, i));
      
      if (strcmp(shelf->name, shelf_name) == 0) {
        break;
      }
    }

    // 'shelf' should always be non-null at this point;
    shelf->amount += amount;
  } else {
    // create a new shelf and add it to the list of shelves for our item
    shelf_t* shelf = create_shelf(strdup(shelf_name), amount);

    if (shelf != NULL) {
      list_append(item->shelves, shelf);
    }
  }

  free(shelf_name);

  return;
}

void add_shelf_to_item(tree_t* tree, char* item_name) {
  char* shelf_name = ask_question_shelf("Hyllplats: ");

  int t_size = tree_size(tree);
  char** t_keys = tree_keys(tree); // varför har vi kvar denna funktion?
  
  bool done = false;
  bool add_shelf = true;

  while (!done) {

    bool check_remaining_nodes = true;

    for (int i = 0; i < t_size; ++i) {
      char* other_name = t_keys[i];
      item_t* other = tree_get(tree, other_name);
      list_t* other_shelves = other->shelves;
      int other_length = list_length(other_shelves);
    
      for (int j = 0; j < other_length; ++j) {
        shelf_t* other_shelf = *(list_get(other_shelves, j));
        char* other_shelf_name = other_shelf->name;
      
        if (strcmp(other_shelf_name, shelf_name) == 0) {
          add_shelf = false;
          puts("other_shelf_name == shelf_name");

          if (strcmp(other_name, item_name) == 0) {
            puts("Varan finns redan på hyllplatsen!");
            int amount = ask_question_int("Antal: ");
            other_shelf->amount += amount;
            done = true;
            break;
          } else {
            check_remaining_nodes = false;
            puts("Ogiltig inmatning!");
            break;
          }
        }
      }

      if (done || !check_remaining_nodes) break;
    }

    if (add_shelf) {
      int amount = ask_question_int("Antal: ");

      shelf_t* shelf = create_shelf(shelf_name, amount);
      item_t* item = tree_get(tree, item_name);
      list_append(item->shelves, shelf);

      done = true;
    }
  }
}

void add_new_item(tree_t* tree, char* name) {
  char* desc = ask_question_string("Beskrivning: ");
  int price = ask_question_int("Pris: ");

  char* shelf_name = ask_question_shelf("Hylla: ");

  while (get_item_on_shelf(tree, shelf_name) != NULL) {
    puts("Hyllan är upptagen!\n");
    free(shelf_name);
    shelf_name = ask_question_shelf("Hylla: ");
  }
  
  int amount = ask_question_int("Antal: ");

  shelf_t* shelf = create_shelf(shelf_name, amount);
  list_t* shelves = list_new();
  list_append(shelves, shelf);

  item_t* item = create_item(name, desc, price, shelves);
  tree_insert(tree, name, item);

  return;
}

void add_item(tree_t* tree) {
  puts("\nLÄGG TILL EN VARA");
  char* name = ask_question_string("Varunamn: ");
  
  if (tree_has_key(tree, name)) {
    add_shelf_to_item2(tree, name);
  } else {
    add_new_item(tree, name);
  }

  return;
}

void remove_item() {
  return;
}

void undo_edit(struct action* undo) {
  item_t* old = undo->old;
  item_t* new = undo->new;

  char* unused_name = new->name;
  char* unused_desc = new->desc;
  list_t* unused_shelves = new->shelves;

  new->name = old->name;
  new->desc = old->desc;
  new->price = old->price;
  new->shelves = old->shelves;

  free(unused_name);
  free(unused_desc);
  free(unused_shelves);
  free(undo->old);
}

void undo_action(struct action* undo) {
  switch (undo->type) {
  case EDIT:
    undo_edit(undo);
    break;
  case NOTHING:
  default:
    break;
  }

  undo->type = NOTHING;

  return;
}

void print_item(item_t* item) {
  printf("\nNamn: %s\n", item->name);
  printf("Beskrivning: %s\n", item->desc);
  printf("Pris: %d kr\n", item->price);
  puts("Lagerhyllor:");

  list_t* shelves = item->shelves;
  int len = list_length(shelves);

  for (int i = 0; i < len; ++i) {
    shelf_t** shelf = list_get(shelves, i);
    printf("\t%s:\t%d st\n", (*shelf)->name, (*shelf)->amount);
  }
}

int list_page(char** keys, int len, int index) {
  int i = 0;

  for (; i < 20 && index < len;) {
    printf("%d %s\n", i + 1, keys[index]);
    ++i;
    ++index;
  }

  return i;
}

item_t* select_item(tree_t* tree) {
  int size = tree_size(tree);
  char** keys = tree_keys(tree);
  int index = 0;
  bool done = false;

  if (keys == NULL) {
    puts("\nDet finns inga varor!");
    free(keys);
    return NULL;
  }

  puts("\nVARUKATALOG");

  while (!done) {
    int page_size = list_page(keys, size, index);

    print_menu_next();

    char* str = ask_question_string("Skriv in en bokstav eller ett index: ");

    int num = atoi(str);

    switch (toupper(*str)) {
    case 'A':
      done = true;
      break;
    case 'V':
      break;
    default:
      if (num < 1 || num > page_size) {
        puts("Ogiltig inmatning!");
      } else {
        item_t* item = tree_get(tree, keys[num - 1]);
        return item;
      }

      break;
    }

    index += 20;
    
    if (index >= size) {
      done = true;
    }
  }
  
  return NULL;
}

void print_menu_edit() {
  puts("\nREDIGERA VARA");
  puts("[B]eskrivning");
  puts("[P]ris");
  puts("[L]agerhylla");
  //puts("An[t]al");
  puts("[A]vbryt");
}

void print_menu_shelf() {
  puts("\nREDIGERA HYLLA");
  puts("[N]amn");
  puts("An[t]al");
  puts("[A]vsluta");
}

void edit_shelf(tree_t* tree, shelf_t* shelf) {
  print_menu_shelf();

  char c = toupper(ask_question_char("Skriv in en bokstav: "));
  bool done = false;

  while (!done) {
    switch (c) {
    case 'N':
      printf("Nuvarande namn: %s\n", shelf->name);
      char* name = ask_question_shelf("Nytt namn: ");

      if (get_item_on_shelf(tree, name) != NULL) {
        puts("Upptagen hylla");
      } else {
        shelf->name = name;
        done = true;
      }
      break;
    case 'T':
      printf("Nuvarande antal: %d\n", shelf->amount);
      int amount = ask_question_int("Nytt antal: ");
      shelf->amount = amount;
      done = true;
      break;
    case 'A':
      done = true;
      break;
    default:
      break;
    }
  }

  return;
}

item_t* copy_item(item_t* original) {
  if (original == NULL) return NULL;

  char* name = strdup(original->name);
  char* desc = strdup(original->desc);
  
  list_t* shelves = list_new();
  int length = list_length(original->shelves);

  for (int i = 0; i < length; ++i) {
    shelf_t* source = *(list_get(original->shelves, i));
    char* name = strdup(source->name);
    int amount = source->amount;

    shelf_t* copy = create_shelf(name, amount);

    list_append(shelves, copy);
  }

  item_t* item = create_item(name, desc, original->price, shelves);
  return item;
}

void edit_item(tree_t* tree, struct action* undo) {
  item_t* item = select_item(tree);

  if (item == NULL) return;

  item_t* old = copy_item(item);
  undo->old = old;
  undo->new = item;
  undo->type = EDIT;

  print_menu_edit();

  char c = toupper(ask_question_char("Skriv in en bokstav: "));

  switch (c) {
  case 'B':
    printf("Nuvarande besrivning: %s\n", item->desc);
    char* str = ask_question_string("Ny besrivning: ");
    item->desc = str;
    break;

  case 'P':
    printf("Nuvarande pris: %d\n", item->price);
    int price = ask_question_int("Nytt pris: ");
    item->price = price;
    break;

  case 'L':
    printf("Nuvarande hyllor:\n");
    list_t* shelves = item->shelves;
    int len = list_length(shelves);

    for (int i = 0; i < len; ++i) {
      shelf_t* shelf = *list_get(shelves, i);
      printf("%d) %s:\t%d st\n", i + 1, shelf->name, shelf->amount);
    }

    int index = ask_question_int("Skriv in ett index: ") - 1;

    if (index < 0 || index >= len) {
      puts("Ogiltigt index!");
      return;
    }

    edit_shelf(tree, *(list_get(shelves, index)));
    break;

  default:
    puts("Ogiltig inmatning!");
    break;
  }

  return;
}

void list_db(tree_t* tree) {
  item_t* item = select_item(tree);

  if (item != NULL) {
    print_item(item);
  }
}

void print_main_menu() {
  puts("\nHUVUDMENY");
  puts("[L]ägg till en vara");
  puts("[T]a bort en vara");
  puts("[R]edigera en vara");
  puts("Ån[g]ra senaste ändringen");
  puts("Lista [h]ela varukatalogen");
  puts("[A]vsluta");
}


void event_loop(tree_t* tree) {
  char c;
  bool quit = false;

  struct action undo = { .type = NOTHING };

  do {
    print_main_menu();
    c = ask_question_menu("Skriv in en bokstav: ");

    switch (c) {
    case 'L':
      add_item(tree);
      break;
    case 'T':
      remove_item();
      break;
    case 'R':
      edit_item(tree, &undo);
      break;
    case 'G':
      undo_action(&undo);
      break;
    case 'H':
      list_db(tree);
      break;
    case 'A':
      quit = true;
      break;
    default:
      puts("Ogiltig inmatning!");
      break;
    }
  } while (!quit);

  return;
}

int main(int argc, char* argv[]) {
  tree_t* tree = tree_new();
  puts("Välkommen till lagerhanteraren 1.0");
  puts("----------------------------------");
  event_loop(tree);

  tree_delete(tree, NULL);
  return 0;
}
