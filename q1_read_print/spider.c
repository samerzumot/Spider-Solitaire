#include "cs136.h"
#include "spider.h"

static const int TABLEAU_SIZE = 10;
static const int DECK_SIZE = 13;
static const int NUM_DECK = 8;

// spider_init_stacks(s) initializes all
// the stacks in the spider struct. 
// requires: s is a valid pointer.
// effects: mutates *s
static void spider_init_stacks(struct spider *s) {
  stack_init(&s->stock);
  for (int i = 0; i < TABLEAU_SIZE; ++i) {
    stack_init(&s->tableau_up[i]);
    stack_init(&s->tableau_down[i]);
  }
}

// get_random_card(card_count) repeatedly generates
// a random card value from 1 to 13 until 
// the number of cards left for that value is 
// positive according to card_count.
// requires: card_count is a valid pointer.
//           length of card_count is 13.
static int get_random_card(int card_count[]) {
  assert(card_count);

  int card_random = rand() % DECK_SIZE + 1;
  while (card_count[card_random - 1] == 0) {
    card_random = rand() % DECK_SIZE + 1;
  }
  return card_random;
}

// init_tableau(s,index,num_down,num_up,card_count)
// initialize the index-th tableau randomly by 
// putting num_down cards in the face-down pile and
// num_up cards in the face-up pile.
// requires: s and card_count are valid pointers.
//           0 <= index <= 9
//           4 <= num_down <= 5
//           num_up == 1
//           length of card_count is 13.
// effects: mutates *s
static void init_tableau(struct spider *s, 
                         int index, 
                         int num_down, 
                         int num_up, 
                         int card_count[]) {
  assert(s);
  assert(card_count);
  assert(0 <= index);
  assert(index <= 9);
  assert(num_down == 4 || num_down == 5);
  assert(num_up == 1);

  for (int j = 0; j <= num_down - 1; ++j) {
    int card_random = get_random_card(card_count);
    stack_push(card_random, &s->tableau_down[index]);
    --card_count[card_random - 1];
  }
  for (int j = 0; j <= num_up - 1; ++j) {
    int card_random = get_random_card(card_count);
    stack_push(card_random, &s->tableau_up[index]);
    --card_count[card_random - 1];
  }
}

void spider_init_random(struct spider *s, 
                        int seed) {
  assert(s);

  const int STOCK_SIZE_INITIAL = 50;

  int card_count[13];
  for (int i = 0; i < DECK_SIZE; ++i) {
    card_count[i] = 8;
  }

  srand(seed);

  // initialize the stacks
  spider_init_stacks(s); 


  // init stock pile
  s->stock_size = STOCK_SIZE_INITIAL;
  for (int i = 0; i < STOCK_SIZE_INITIAL; ++i) {
    int card_random = get_random_card(card_count);
    stack_push(card_random, &s->stock);
    --card_count[card_random - 1];
  }

  // init the tableaus
  for (int i = 0; i < 4; ++i) {
    init_tableau(s, i, 5, 1, card_count);
  }
  for (int i = 4; i <= TABLEAU_SIZE - 1; ++i) {
    init_tableau(s, i, 4, 1, card_count);
  }

  s->comp_decks = 0;
}

void spider_init_custom(struct spider *s) {
  assert(s);

  int card_read = -1;

  // initialize the stacks
  spider_init_stacks(s); 

  // init stock pile
  s->stock_size = 0;
  int r = read_card(&card_read);
  int count = 0;
  while (!r) {
    ++count;
    ++(s->stock_size);
    stack_push(card_read, &s->stock);
    r = read_card(&card_read);
  }

  // init the tableaus
  for (int i = 0; i <= TABLEAU_SIZE - 1; ++i) {

    int r = read_card(&card_read);
    while (!r) {
      stack_push(card_read, &s->tableau_down[i]);
      r = read_card(&card_read);
    }

    r = read_card(&card_read);
    while (!r) {
      stack_push(card_read, &s->tableau_up[i]);
      r = read_card(&card_read);
    }

  }

  scanf("%d", &s->comp_decks);
}

bool spider_has_won(const struct spider *s) {
  assert(s);

  return (s->comp_decks == NUM_DECK);
}

//=========================================================

// read_card(card_read) reads at most two characters representing
// the value of a playing card.  If the first character is E,
// then it returns true.  Otherwise, it returns false and
// mutates *card_read to be the integer value of the card.
// A is read as 1, J is read as 11, 
// Q is read as 12 and K is read as 13.
// If an error occurs reading in the value, the function
// prints out an error message and terminates the program.
// effects: reads input and modifies *card_read
bool read_card(int *card_read) {
  assert(card_read);
  char s = 0;
  scanf(" %c", &s);
  if (s == 'E') {
    return true;
  } 
  if (s == '1') {
    scanf("%c", &s);
    if (s == '0') {
      *card_read = 10;
      return false;
    } 
  }
  if (s == 'A') {
    *card_read = 1;
    return false;
  }
  if (s == 'J') {
    *card_read = 11;
    return false;
  }
  if (s == 'Q') {
    *card_read = 12;
    return false;
  }
  if (s == 'K') {
    *card_read = 13;
    return false;
  }
  if (s <= 57 && s >= 49) {
    *card_read = s - 48;
    return false;
  } else
    printf("Error reading in a card./n");
  exit(1);
}

void spider_print_faceup(struct spider *s) {
  assert(s);
  // Replace the code below with your implementation.
  printf("spider_print_faceup has not been implemented yet.\n");
}

// stack_len(a) returns the length of a given stack 
// using the stack_pop function
int stack_len(struct stack a) {
  int i = 0;
  while(!(stack_is_empty(&a))){
    stack_pop(&a);
    ++i;
  }
  return i;
}

// stack_arr(x, a, len) converts a stack into an array using the stack_pop 
// function 
// requires: a is a valid array (non-null)
//           len is positive
void stack_arr(struct stack x, int a[], int len){
  assert(a);
  assert(len > 0);
  struct stack new = {0};
  stack_init(&new);
  while (!(stack_is_empty(&x))){
    stack_push(stack_pop(&x), &new);
  }
  for(int i = 0; i < len; ++i) {
    a[i] = stack_pop(&new);
  }
}


// spider_print(s) prints out the statue of the game including
// the number of cards left in the stock pile
// the face-down and face-up cards in each tableau pile
// the number of complete decks in the foundation piles
// effects: produces output
void spider_print(struct spider *s) {
  assert(s);
  printf("The number of cards left in the stock pile: %d\n", s->stock_size);
  printf("The number of completed decks: %d\n", s->comp_decks);
  for(int i = 0; i < 10; ++i) {
    printf("%d: ", i);
    struct stack stackdown = s->tableau_down[i];
    struct stack stackup = s->tableau_up[i];

    int lendown = stack_len(stackdown);
    int lenup = stack_len(stackup);
    int a[104] = {0};
    stack_arr(stackup, &a[0], lenup);
    for(int s = 0; s < lendown; ++s){
      printf("_ ");
    }
    for(int m = 0; m < lenup; ++m){
      int n = a[m];
      if (n == 1) {
        printf("A");
      } else
        if (n == 11) {
        printf("J");
      } else
        if (n == 12) {
        printf("Q");
      } else
        if (n == 13) {
        printf("K");
      }
      else {
        printf("%d", n);
      }

      if (m < (lenup - 1)) {
        printf(" ");
      }
    }
    printf("\n");
  }
}

void spider_get_stock(struct spider *s) {
  // Replace the code below with your implementation.
  printf("spider_get_stock has not been implemented yet.\n");
}

// spider_find_seq(s_src_up,temp,card) attempts
// to find a descending sequence of cards starting 
// with the provided card on top of s_src_up.  
// If such a sequence does not exist, 
// this function finds the longest descending sequence
// of cards on top of s_src_up.  After finding
// such a sequence, the function moves this sequence
// to temp in reverse order.  Then, the function
// returns true if the descending sequence ends with 
// the provided card.  It returns false otherwise.
// requires: s_src_up is non-empty.
//           s_src_up and temp are valid pointers.
//           1 <= card <= 13
static bool spider_find_seq(struct stack *s_src_up, 
                            struct stack *temp,
                            int card) {
  // Replace the code below with your implementation.
  printf("spider_find_seq has not been implemneted yet.\n");
  return false;
}

void spider_test_find_seq(void) {
  struct stack src = {};
  stack_init(&src);
  struct stack temp = {};
  stack_init(&temp);
  int card = 0;

  // Marmoset public test
  stack_push(1, &src);
  card = 1;
  bool found = spider_find_seq(&src, &temp, card);
  assert(found);
  assert(stack_pop(&temp) == 1);
  assert(stack_is_empty(&src));
  assert(stack_is_empty(&temp));

  // The first example
  stack_push(5, &src);
  stack_push(4, &src);
  stack_push(3, &src);
  stack_push(2, &src);
  card = 2;
  found = spider_find_seq(&src, &temp, card);
  assert(found);
  assert(stack_pop(&temp) == 2);
  assert(stack_is_empty(&temp));
  assert(stack_pop(&src) == 3);
  assert(stack_pop(&src) == 4);
  assert(stack_pop(&src) == 5);
  assert(stack_is_empty(&src));

  // The second example
  stack_push(5, &src);
  stack_push(4, &src);
  stack_push(3, &src);
  stack_push(2, &src);
  card = 5;
  found = spider_find_seq(&src, &temp, card);
  assert(found);
  assert(stack_pop(&temp) == 5);
  assert(stack_pop(&temp) == 4);
  assert(stack_pop(&temp) == 3);
  assert(stack_pop(&temp) == 2);
  assert(stack_is_empty(&temp));
  assert(stack_is_empty(&src));


  // Add your own tests below

}

void spider_move(struct spider *s, 
                 int src, int card, int dest) {
  // Replace the code below with your implementation.
  printf("spider_move has not been implemented yet.\n");
}


