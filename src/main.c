#include "game.h"

void main(int argc, char const *argv[])
{
    struct game *game = create_game();
    play_game(game);
}
