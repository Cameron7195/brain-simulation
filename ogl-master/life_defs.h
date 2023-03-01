//
//  life_defs.h
//  life
//
//  Created by Cameron Witkowski on 2021-05-30.
//

#ifndef life_defs_h
#define life_defs_h
struct grid_ll_node {
   int pt ;
   struct grid_ll_node *next;
};
typedef struct grid_ll_node grid_ll;


void ll_append(grid_ll **root, grid_ll *appendee);
void ll_remove(grid_ll **root, int pointToRemove);

#endif /* life_defs_h */
