#ifndef __MINE_H_
#define __MINE_H_
#include <stdio.h>
#include <stdint.h>

#define MAX_RAW 100
#define MAX_COL 100

enum mine_status
  {
	clicked_on_digged,			/* already digged */
	clicked_on_gameover,			/* hit a mine */
	clicked_on_empty,
	clicked_on_number,
	clicked_on_secceed,
	clicked_on_nothing,
	clicked_on_mark,
	clicked_on_unmark,
	clicked_on_error
  };


extern uint8_t mine_count;
extern uint8_t mine_count_marked;
extern uint8_t row,col;			/* need to be initialed */
extern uint8_t mine_map[MAX_RAW][MAX_COL];
extern uint8_t mine_number[MAX_RAW][MAX_COL];
extern uint8_t mine_digged[MAX_RAW][MAX_COL];
extern uint8_t mine_marked[MAX_RAW][MAX_COL];



extern void init_mine_map(uint8_t* map_head); /* set map to what you like  */
extern void init_mine_map_with_default();
extern void init_mine_map_rand(uint32_t n); /* n means number of mine */

extern enum mine_status dig(uint8_t i,uint8_t j);
extern enum mine_status mark(uint8_t i,uint8_t j);



#endif
