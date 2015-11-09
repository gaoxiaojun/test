import sys
import pygame
from pygame.locals import *
import board

def terminate():
    pygame.quit()
    sys.exit()

def main():
    pygame.init()
    pygame.key.set_repeat(200,100)
    myBoard = board.Board()

    while True:
        pygame.display.flip()
        pygame.display.set_caption('2048-Score: %d' % myBoard.score)
        myBoard.print_board()
        test = myBoard.__deepcopy__()
        if not (test.shift_vertical(isUp=True)
                or test.shift_vertical(isUp=False)
                or test.shift_horizontal(isLeft=True)
                or test.shift_horizontal(isLeft=False)):
            myBoard.game_over()

        for event in pygame.event.get():
            if event.type == QUIT:
                terminate()
            elif event.type == KEYDOWN:
                if event.key==K_ESCAPE:
                    terminate()
                if event.key==K_UP:
                    if myBoard.shift_vertical(isUp=True):
                        myBoard.spawn_piece()
                if event.key==K_DOWN:
                    if myBoard.shift_vertical(isUp=False):
                        myBoard.spawn_piece()
                if event.key==K_LEFT:
                    if myBoard.shift_horizontal(isLeft=True):
                        myBoard.spawn_piece()
                if event.key==K_RIGHT:
                    if myBoard.shift_horizontal(isLeft=False):
                        myBoard.spawn_piece()
                if event.key == K_SPACE:
                    print "Test board:"
                    print test.board;
                    print "my board:"
                    print myBoard.board;

if __name__ == '__main__':
    main()
                
