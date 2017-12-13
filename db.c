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

void shelf_free(elem_t shelf);


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

void item_free(item_t *item)
{
  free(item->desc);
  list_delete(item->shelves, true);
  free(item);
}


void free_e(elem_t elem)
{
  item_free(elem.p);
}

int tree_compare(elem_t a, elem_t b)
{    
  return strcmp((char*)a.p, (char*)b.p);
  
}

elem_t shelf_copy(elem_t shelf)
{
  /// FIXME: verify no copy function needed
  /// FIXME: old copy function exists in git history
  return shelf;
}

void shelf_free(elem_t shelf)
{
  shelf_t *s = shelf.p;
  free(s->name);
  free(s);
}

int shelf_compare(elem_t a, elem_t b) //size_t??? int???
{
  if (strcmp((char*)a.p, (char*)b.p) >0)
    {
      return 2 ;
    }
  else if (strcmp((char*)a.p, (char*)b.p) < 0)
    {
      return -1;
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

shelf_t * copy_shelf(shelf_t *original)
{
  return create_shelf(strdup(original->name), original->amount);
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
      item_t *item;
      elem_t elem;
      tree_get(tree, node_name, &elem);
      item = (item_t*)elem.p;
      
      list_t *shelves1 = item->shelves;
      int length = list_length(shelves1);

      for (int j = 0; j < length; ++j)
        {
          shelf_t *shelf_item = NULL;
          elem_t element;
          list_get(shelves1, j, &element);
          shelf_item = (shelf_t*)element.p;
          
          char *other_shelf_name = shelf_item->name;

          if (strcmp(shelf_name, other_shelf_name)==0)
            {
              free(t_keys);
              return item;

            }
        }
    }
  free(t_keys);
  return NULL;
}

shelf_t *get_shelf_by_name(list_t *list, char *shelf_name)
{
  for (int i = list_length(list)-1; i >= 0; --i)
    {
      elem_t result; 
      list_get(list, i, &result);
      shelf_t *shelf = result.p;
      
      if (strcmp(shelf_name, shelf->name) == 0) return shelf;
    }

  return NULL;
}

void add_shelf_to_item2(tree_t *tree, item_t *item)
{
  shelf_t *shelf = NULL;

  while (shelf == NULL)
    {
      char *shelf_name = ask_question_shelf("Hyllplats: ");
      item_t *owner = get_item_on_shelf(tree, shelf_name);

      if (item == NULL)
        {
          shelf = create_shelf(shelf_name, 0);
          list_append(item->shelves, (elem_t) { .p = shelf });
        }
      else if (item == owner)
        {
          shelf = get_shelf_by_name(item->shelves, shelf_name);
        }
      else
        {
          free(shelf_name);
          puts("Hyllan är upptagen\n");
        }
    }

  int amount = ask_question_int("Antal: ");
  shelf->amount += amount;
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
      elem_t result;
      tree_get(tree, key, &result);
      add_shelf_to_item2(tree, result.p);
    }
  else
    {
      add_new_item(tree, name);
    }

  return;
}

void add_to_db(tree_t *db, tree_key_t name, elem_t elem)
{
  tree_insert(db, name, elem);
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
              free(str);
              free(keys);
              return (item_t*)elem.p;
            }
          break;
        }
      index += 20;
      if(index >= size)
        {
          done = true;
        }

      free(str);
    }
  free(keys);
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

  list_t* copy_of_shelves = list_new(shelf_copy, shelf_free, shelf_compare);

  int length = list_length(original->shelves);
  for (int i = 0; i < length; ++i)
    {
      elem_t elem;
      list_get(original->shelves, i, &elem);

      list_append(copy_of_shelves, (elem_t) {
          .p = copy_shelf((shelf_t *)elem.p)
        });
    }
  item_t *item = create_item(name, desc, original->price, copy_of_shelves);
  return item;
}


void edit_item(tree_t *tree, struct action *undo)
{
  item_t *item = select_item(tree);
  if(item == NULL) return;

  item_t *old = copy_item(item);
   undo->old = old; 
   undo->new = item;
   undo->type= EDIT;

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
          list_get(shelves, i, &elem);
          shelf = (shelf_t*)elem.p;
          printf("%d) %s: \t%d st\n", i + 1, shelf->name, shelf->amount);
        }
      int index = (ask_question_int("Skriv in ett index:") -1);

      if (index < 0 || index >= length)
        {
          puts("Ogiltigt index!");
          return;
        }
      elem_t element;
      shelf_t *shelf;
      list_get(shelves, index, &element);
      shelf = (shelf_t*)element.p;
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
      print_tree(tree);
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
  
  if (undo.type != NOTHING)
    {
      free(undo.old->name);
      item_free(undo.old);
    }
}

void shelf_aux(FILE* persistance, list_t *shelves)
{
  int length = list_length(shelves);
  char buf_siz[255];
  snprintf(buf_siz, 255, "%d", length);
  fputs(buf_siz, persistance);
  fputs("\n", persistance);

  for (int i = 0; i < length; i++)
    {
      elem_t elem;
      list_get(shelves, i, &elem);
      char buf_siz[255];
      char *name = ((shelf_t*)elem.p)->name;
      fputs(name, persistance);
      fputs("\n", persistance);
      int amm = ((shelf_t*)elem.p)->amount;
      snprintf(buf_siz, 255, "%d", amm);
      fputs(buf_siz, persistance);
      fputs("\n", persistance);
    }
}


void savedb_aux(elem_t *elem, FILE *persistance, int size)
{
  char buf_siz[255];
  snprintf(buf_siz, 255, "%d", size);
  fputs(buf_siz, persistance);
  fputs("\n", persistance);
  for (int i = 0; i < size; i++)
    {
      elem_t to_save = elem[i];
      char *name = ((item_t*)to_save.p)->name;
      fputs(name, persistance);
      fputs("\n", persistance);
      char *desc = ((item_t*)to_save.p)->desc;
      fputs(desc, persistance);
      fputs("\n", persistance);
      int price = ((item_t*)to_save.p)->price;
      char charprice[255];
      snprintf(charprice, 255, "%d", price);
      fputs(charprice, persistance);
      fputs("\n", persistance);
      shelf_aux(persistance, ((item_t*)to_save.p)->shelves);   
    }
}

void savedb(tree_t *db)
{
 
  elem_t *elem = tree_elements(db);
  FILE *persistance;
  persistance = fopen("dbpersistance", "w");
  savedb_aux(elem, persistance, tree_size(db));
}

char* new_line_help(char* a)
{
  int i = strlen(a)-1;
  a[i] = '\0';
  return a;
  
}

void read_db_aux(tree_t *db, FILE *persistance)
{
  size_t siz = 0;
  char *line = NULL;
  getline(&line, &siz, persistance);
  int size = atoi(line);
  printf("%s%d\n","on line 830, size is", size );
 
  
  for (int i = 0; i < size; i++)
    {
      item_t *item = calloc(1, sizeof(item_t));
      siz = 0;
      line = NULL;
      getline(&line, &siz, persistance);
      char* name = new_line_help(strdup(line));
      printf("%s%s\n","on line 839, name is", name );
      item->name = name;

      siz = 0;
      line = NULL;
      getline(&line, &siz, persistance);
      char *desc = new_line_help(strdup(line));
      printf("%s%s\n","on line 844, desc is", desc );
      item->desc = desc;

      siz = 0;
      line = NULL;
      getline(&line, &siz, persistance);
      int price = atoi(line);
      printf("%s%d\n","on line 849, price is", price );
      item->price = price;

      siz = 0;
      line = NULL;
      getline(&line, &siz, persistance);
      int shelf_ammount = atoi(line);
       printf("%s%d\n","on line 854, shelf_ammount is", shelf_ammount );

      list_t *list = list_new(shelf_copy, shelf_free, shelf_compare);
      printf("%s%s\n","on line 858, list is on pointer", ((char*)list));
      
      for (int i = 0; i < shelf_ammount; i++)
        {
          shelf_t *shelf = calloc(1, sizeof(shelf_t));
          siz = 0;
          line = NULL;
          getline(&line, &siz, persistance);
          char *shelf_name = new_line_help(strdup(line));
          printf("%s%s\n","on line 864, shelfname is", shelf_name);
          shelf->name = shelf_name;

          siz = 0;
          line = NULL;
          getline(&line, &siz, persistance);
          int shelf_amm = atoi(line);
          printf("%s%d\n","on line 869, shelf_amm is", shelf_amm);
          shelf->amount = shelf_amm;

          if (i == 0)
            {
            item->shelves = list;
          }
          
          elem_t list_e = {.p = shelf};
          list_append(list ,list_e);
          
          printf("%s%s\n","this is the list", ((char*)&list));
          
          //free(shelf_name);
        }

      
      elem_t elem = {.p= item};
      tree_key_t key = {.p = item->name};
      
      tree_insert(db, key, elem);
      
      //free(name);
      //free(item);
      //free(desc);
      //free(list);
      
     
    }

  
}

void read_db(tree_t *db)
{
  FILE *persistance;
  persistance = fopen("dbpersistance", "r");
  read_db_aux(db, persistance);


}
int main(int argc, char* argv[])
{
  tree_t *tree= tree_new(tree_copy, free_k, free_e, tree_compare);
  read_db(tree);
  event_loop(tree);
  savedb(tree);
  tree_delete(tree, true, true);
  return 0;
}

/* END OF: MENY / EVENTLOOP / MAIN */


