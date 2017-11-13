#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tree.h"
#include "list.h"
#include "utils.h"
#include "common.h"

typedef struct shelf{
  // char *letter;
  // int *number;
  char *name;
  int amount; //funkar detta?
}shelf_t;

typedef struct item {
  char *name;
  char *desc;
  int price;
  list_t *shelves;
}item_t;

struct action {
  enum{NOTHING, EDIT} type;
  union{
    struct{item_t *old; item_t *new;};
  };
};
/* TREE COPY COMPARE FREE*/

// Måste göra funktionerna

elem_t tree_copy(elem_t tree)
{
  item_t *from = (item_t*)tree.p;
  item_t *to = (item_t*)calloc(1, sizeof(elem_t));
  *to = *from;

  elem_t result;
  result.p = to;
  return result;

}

void free_k(elem_t key)
{
  free(key.p);
}

void free_e(elem_t elem)
{
  free(elem.p);
}

int tree_compare(elem_t a, elem_t b)
{
  if (strcmp((char*)a.p, (char*)b.p) >0)
    {
      return 1 ;
    }
  else if (strcmp((char*)a.p, (char*)b.p) < 0)
      {
      return -1;
    }
  else return 0; 
}

/* */
/* SHELF: COPY / FREE / COMPARE */
elem_t shelf_copy(elem_t shelf)
{
  shelf_t *from = (shelf_t*)shelf.p;
  shelf_t *to = (shelf_t*)calloc(1, sizeof(shelf_t));
  *to = *from;

  elem_t result = { .p = to};
  return result;
}

void shelf_free(elem_t shelf)
{
  free(shelf.p);
}

int shelf_compare(elem_t a, elem_t b) //size_t??? int???
{
  if (strcmp((char*)a.p, (char*)b.p) >0)
    {
      return 2 ;
    }
  else if (strcmp((char*)a.p, (char*)b.p) < 0)
    {
      return 1;
    }
  else return 0;
}


//list_t *list = list_new(shelf_copy, shelf_free, shelf_compare); queeeee????

/* END OF SHELF: COPY / FREE / COMPARE */




shelf_t * create_shelf(char *name, int amount)
{
  shelf_t *shelf = (shelf_t*)calloc(1, sizeof(shelf_t));

  if (shelf != NULL)
    {
      shelf->name = name;
      shelf->amount = amount;
    }
  return shelf;
}


item_t *create_item(char *name, char *desc, int price, list_t *shelves)
{
  item_t *item = (item_t*)calloc(1, sizeof(item_t));

  item->name = name;
  item->desc = desc;
  item->price = price;
  item->shelves = shelves;
  return item;
}

item_t *get_item_on_shelf(tree_t *tree, char* shelf_name) //elem_t?
{
  int t_size = tree_size(tree);
  tree_key_t *t_keys = tree_keys(tree);

  for (int i = 0; i < t_size; ++i)
    {
      tree_key_t node_name = t_keys[i];
      item_t *item = NULL;
      elem_t elem;
      tree_get(tree, node_name, &elem);
      item = (item_t*)elem.p;
      
      list_t *shelves = item->shelves;
      int length = list_length(shelves);

      for (int j = 0; j < length; ++j)
        {
          shelf_t *shelf_item = NULL;
          elem_t element;
          list_get(shelves, j, &element);
          shelf_item = (shelf_t*)element.p;
          
          char *other_shelf_name = shelf_item->name;

          if (strcmp(shelf_name, other_shelf_name)==0)
            {
              return item;

            }
        }
    }
  return NULL;
}

void add_shelf_to_item2(tree_t *tree, char *item_name)
{
  char *shelf_name = NULL;
  item_t *item = NULL;
  bool shelf_exists = false;
  bool available_shelf = false;
  elem_t elem;

  while (!available_shelf)
    {
      shelf_name = ask_question_shelf("Hyllplats: ");
      item = get_item_on_shelf(tree, shelf_name);

      if (item == NULL)
        {
          available_shelf = true;
        }
      else if (strcmp(item->name, item_name)== 0)
        {
          available_shelf = true;
          shelf_exists = true;
        }
      else
        {
          free(shelf_name);
          puts("Hyllan är upptagen\n");
        }
    }
  if (item == NULL)
    {
      item = (item_t*)elem.p;
      tree_key_t key;
      key.p = item_name;
      tree_get(tree, key, &elem); // HELP-- ska vara av typ tree_key_t
    }
  int amount = ask_question_int("Antal: ");

  if (shelf_exists)
    {
      list_t *shelves = item->shelves;
      int length = list_length(shelves);
      shelf_t *shelf = NULL;
      elem_t element;

      for (int i=0; i < length; ++i)
        {
          shelf = (shelf_t*)element.p;
          list_get(shelves, i, &element);

          if(strcmp(shelf->name, shelf_name)==0)
            {
              break;
            }
        }
      shelf->amount += amount;
    }
  else
    {
      shelf_t *shelf = create_shelf(strdup(shelf_name), amount); // behövs stdup????

      if (shelf != NULL)
        {
          elem_t element;
          element.p = shelf;
          list_append(item->shelves, element);
        }
    }
  free(shelf_name);
  return;

}

void add_shelf_to_item(tree_t *tree, char* item_name)// ska jag ha annan
{
  char *shelf_name = ask_question_shelf("Hyllplats: ");
  int t_size = tree_size(tree);
  tree_key_t *t_keys = tree_keys(tree);
  bool done = false;
  bool add_shelf = true;

  while (!done)
    {
      bool check_remaining_nodes = true;

      for (int i = 0; i < t_size; ++i)
        {
          tree_key_t other_name = t_keys[i];
          item_t *other = NULL;
          elem_t elem;
          other = (item_t*)elem.p;
          tree_get(tree, other_name, &elem);
          list_t *other_shelves = other->shelves;
          int other_length = list_length(other_shelves);

          for (int j = 0; j < other_length; ++j)
            {
              shelf_t *other_shelf = NULL;
              elem_t element;
              other_shelf = (shelf_t*)element.p;
              list_get(other_shelves, j, &element);
              char *other_shelf_name = other_shelf->name;

              if (strcmp(other_shelf_name, shelf_name) ==0)
                {
                  add_shelf = false;
                  puts("other_shelf_name == shelf_name");

                  if (strcmp(other_shelf_name, item_name) == 0)
                    {
                      puts("Varan finns redan på hyllplatsen!");
                      int amount = ask_question_int("Antal: ");
                      other_shelf->amount += amount;
                      done = true;
                      break;

                    }
                  else
                    {
                      check_remaining_nodes = false;
                      puts("Ogiltig inmatning!");
                      break;
                    }
                }

            }
          if (done || !check_remaining_nodes) break;
        }
      if (add_shelf)
        {
          int amount = ask_question_int("Antal: ");
          shelf_t *shelf = create_shelf(shelf_name, amount);
          elem_t elem_shelf;
          elem_shelf.p = shelf;
          item_t *item = NULL;
          elem_t elem;
          elem.p = item;
          tree_key_t key;
          key.p = item_name;

          tree_get(tree, key, &elem); // ska vara av typ tree_key_t
          list_append(item->shelves, elem_shelf);

          done = true;
        }
    }

}


void add_new_item(tree_t *tree, char *name)
{
  char *desc= ask_question_string("Beskrivning: ");
  int price = ask_question_int("Pris: ");

  char *shelf_name = ask_question_shelf("Hylla: ");
  while(get_item_on_shelf(tree, shelf_name) != NULL)
    {
      puts("Hyllan är upptagen! \n");
      free(shelf_name);
      shelf_name = ask_question_shelf("Hylla: ");
    }
  int amount = ask_question_int("Antal: ");
  shelf_t *shelf = create_shelf(shelf_name, amount);
  elem_t elem;
  elem.p  = shelf;
  list_t *shelves = list_new(shelf_copy, shelf_free, shelf_compare);
  list_append(shelves, elem);

  item_t *item = create_item(name, desc, price, shelves);
  tree_key_t key;
  key.p = name;
  elem_t element;
  element.p = item;
  tree_insert(tree, key, element);

  return;
}


void add_item(tree_t *tree)
{
  puts("\nLÄGG TILL EN VARA");
  char *name = ask_question_string("Varunamn");
  tree_key_t key;
  key.p = name;

  if (tree_has_key(tree, key))
    {
      add_shelf_to_item2(tree, name);
    }
  else
    {
      add_new_item(tree, name);
    }

  return;
}


void remove_item()
{
  return;
}

/*----------------- UNDO ------------------*/
void undo_edit(struct action* undo)
{
  item_t *old = undo->old;
  item_t *new = undo->new;

  char *unused_name = new->name;
  char *unused_desc = new->desc;
  list_t *unused_shelves = new->shelves;

  new->name = old->name;
  new->desc = old->desc;
  new->price = old->price;
  new->shelves = old->shelves;

  free(unused_name);
  free(unused_desc);
  free(unused_shelves);
  free(undo->old);
}

void undo_action(struct action *undo)
{
  switch (undo->type)
    {
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
/*--------------- UNDO ----------------*/

void print_menu_next()
{
  puts("\n [V]isa nästa sida");
  puts("[A]vbryt");
}


void print_item(item_t *item)
{
  printf("\nNamn: %s\n", item->name );
  printf("Beskrivning: %s \n", item->desc);
  printf("Pris: %d kr\n", item->price);
  puts("Lagerhyllor:");

  list_t *shelves = item->shelves;
  int length = list_length(shelves);

  for (int i = 0; i < length; ++i)
    {
      //shelf_t **shelf = NULL;
      elem_t elem;
      //elem.p = shelf;
      list_get(shelves, i, &elem);
      printf("\t%s: \t%d st\n", ((shelf_t*)elem.p)->name, ((shelf_t*)elem.p)->amount);
    }
}

int list_page(tree_key_t *keys, int length, int index)
{
  int i = 0;
  //char *key;
  //key = (char*)keys.p;
  for (; i < 20 && index < length;)
    {
      printf("%d %s\n", i +1, (char*)keys[index].p);
      ++i;
      ++index;
    }
  return i;
}

item_t *select_item(tree_t *tree)
{
  int size = tree_size(tree);
  tree_key_t *keys = tree_keys(tree);
  int index = 0;
  bool done = false;

  if (keys == NULL)
    {
      puts("\n Det finns inga varor!");
      free(keys);
      return NULL;
    }
  puts("\nVARUKATALOG");

  while (!done)
    {
      int page_size =list_page(keys, size, index);
      print_menu_next();
      char *str = ask_question_string("Skriv in en bokstav eller ett index: ");
      int number = atoi(str);

      switch(toupper(*str))
        {
        case 'A':
          done = true;
          break;
        case 'V':
          break;
        default:
          if (number < 1 || number > page_size)
            {
              puts("Ogiltig inmatning!");
            }
          else
            {
              item_t *item = NULL;
              elem_t elem;
              elem.p = item;
              tree_get(tree, keys[number -1], &elem);
              return (item_t*)elem.p;
            }
          break;
        }
      index += 20;
      if(index >= size)
        {
          done = true;
        }
    }
  return NULL;
}

void print_menu_edit ()
{
  puts("\n REDIGERA EN VARA:");
  puts("[B]eskrivning");
  puts("[P]ris");
  puts("[L]agerhylla");
  puts("[A]vbryt");
}

void print_menu_shelf()
{
  puts("\nREDIGERA HYLLA");
  puts("[N]amn");
  puts("An[t]al");
  puts("[A]vsluta");
}

void edit_shelf(tree_t *tree, shelf_t *shelf)
{
  print_menu_shelf();

  char c = toupper(ask_question_char("Skriv in en bokstav: "));
  bool done = false;

  while(!done)
    {
      switch(c)
        {
        case 'N':
          printf("Nuvarande namn: %s\n", shelf->name);
          char *name = ask_question_shelf("Nytt namn:");

          if (get_item_on_shelf(tree, name) != NULL)
            {
              puts("Hyllan är upptagen");
            }
          else
            {
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

item_t *copy_item(item_t *original)
{
  if (original == NULL) return NULL;

  char *name = strdup(original->name);
  char *desc = strdup(original->desc);

  list_t* shelves = list_new(shelf_copy, shelf_free, shelf_compare);
  int length = list_length(original->shelves);

  for (int i = 0; i < length; ++i)
    {
      shelf_t *source = NULL;
      elem_t elem;
      list_get(original->shelves, i, &elem);
      source = elem.p;
      char *name = strdup(source->name);
      int amount = source->amount;

      shelf_t *copy = create_shelf(name, amount);
      elem_t element;
      element.p = copy;
      list_append(shelves, element);
    }
  item_t *item = create_item(name, desc, original->price, shelves);
  return item;
}


void edit_item(tree_t *tree, struct action *undo)
{
  item_t *item = select_item(tree);
  if(item == NULL) return;

  item_t *old = copy_item(item);
  undo->old = old;
  undo->new = item;
  undo->type = EDIT;

  print_menu_edit();

  char c = toupper(ask_question_char("skriv in en bokstav: "));

  switch (c)
    {
    case 'B':
      printf("Nuvarande beskrivning: %s\n", item->desc);
      char *str = ask_question_string("Ny beskrivning: ");
      item->desc = str;
      break;

    case 'P':
      printf("Nuvarande pris: %d \n", item->price);
      int price = ask_question_int("Nytt pris: ");
      item->price = price;
      break;

    case 'L':
      printf("Nuvarande hyllor: \n");
      list_t *shelves = item->shelves;
      int length = list_length(shelves);

      for (int i = 0; i < length; ++i)
        {
          shelf_t *shelf = NULL;
          elem_t elem;
          shelf = elem.p;
          list_get(shelves, i, &elem);
          printf("%d) %s: \t%d st\n", i + 1, shelf->name, shelf->amount);
        }
      int index = ask_question_int("Skriv in ett index:") -1;

      if (index < 0 || index >= length)
        {
          puts("Ogiltigt index!");
          return;
        }
      elem_t element;
      shelf_t *shelf;
      shelf = element.p;
      list_get(shelves, index, &element);
      edit_shelf(tree, shelf); ///// Vad blir inputen istället list_get
      break;
    default:
      puts("Ogiltig inmatning!");
      break;
    }
  return;
}


void list_db(tree_t *tree)
{
  item_t *item = select_item(tree);
  if (item != NULL)
    {
      print_item(item);
    }
}


/* MENY / EVENTLOOP / MAIN */
char ask_question_menu(char *question)
{
  char c;
  bool done = false;
  do {
    c = toupper(ask_question_char(question));

    switch (c)
      {
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
  }
  while (!done);
  return c;
}




void print_main_menu()
{
  puts("\n HUVUDMENY \n");
  puts("[L]ägg till en vara");
  puts("[T]a bort en vara");
  puts("[R]edigera en vara");
  puts("Ån[g]ra senaste ändringen");
  puts("Lista [h]ela varukatalogen");
  puts("[A]vsluta");
}

void event_loop(tree_t* tree)
{
  char c;
  bool quit = false;
  struct action undo = { .type = NOTHING};

  do{
    print_main_menu();
    c = ask_question_menu("Skriv in en bokstav: ");
    switch (c){
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
  }
  while (!quit);
  return;
}


int main(int argc, char* argv[])
{
  tree_t *tree = tree_new(tree_copy, free_k, free_e, tree_compare);
  event_loop(tree);
  tree_delete(tree, true, true); // eller ska det vara false?
  return 0;
}

/* END OF: MENY / EVENTLOOP / MAIN */


