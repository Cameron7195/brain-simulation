//
//  life_defs.cpp
//  life
//
//  Created by Cameron Witkowski on 2021-05-30.
//

#include <stdio.h>
#include <stdlib.h>
#include "life_defs.h"

void ll_append(grid_ll **root, grid_ll *appendee) {
    if (appendee->pt < 0) {printf("ERROR ERROR LESS THAN 0\n\n\n\n\n\n\n\n\n\n\n\n\n");}
    if (*root == NULL) {
        *root = appendee;
    }
    else {
       appendee->next = *root;
       *root = appendee;
    }
    return;
}

void ll_remove(grid_ll **root, int pointToRemove) {
    if (*root == NULL) {
        printf("BAD BAD BAD BAD BAD BAD BAD FIX THIS!!!!!!\n\n\n\n\n");
    }
    else if ((*root)->pt == pointToRemove) {
        grid_ll *temp = *root;
        *root = temp->next;
        free(temp);
    }
    else {
        grid_ll *lastNode = *root;
        grid_ll *curNode = *root;
        while (curNode) {
            if (curNode->pt == pointToRemove) {
                lastNode->next = curNode->next;
                free(curNode);
                return;
            }
            lastNode = curNode;
            curNode = curNode->next;
        }
    }
    
}

