/************************
*                       *
* Knight_tour - 17/1/19 *     
*                       *
************************/
#ifndef OL61_KNIGHT_TOUR
#define OL61_KNIGHT_TOUR

/* Extra Info
* File name: knight_tour
* Restrictions: 
               * 64 array size.
               * No static or global variales.
               * No stack.
* Second phase:
               * timout of 5min for finding a path(after it itll return
                 timeout error).
               * heuristics (read about it).
*/

/*
* gets a knight random point on a chess board(0 - 63) and fill the array
  with path that the knight go without be on the same block wtice or more.
* Arguments: path: empty array (size 64) will contain in the future the knights
                   path.
             begin_point: random point on the board which the knight will
                          start.
* Return: none, (will fill our path array).
*/
void KnightTour(unsigned char *path, unsigned char *begin_point);

#endif /* OL61_KNIGHT_TOUR */