import sys
import os
import pygame
from pygame.locals import *
import board

def terminate(board):
    with open('2048.ini', 'w') as f:
        f.write('Board=%d\n' % board.dim)
        f.write('HighScore=%d\n' % board.high_score)
    pygame.quit()
    sys.exit()

def main():
    pygame.init()
    pygame.key.set_repeat(200, 100)

    with open('2048.ini') as f:
        for line in f:
            l = ' '.join(line.split('=')).split()
            if l[0] == 'Board':
                dim = int(float(l[-1]))
            elif l[0] == 'HighScore':
                high_score = int(float(l[-1]))

    myBoard = board.Board(dim, high_score)

    while True:
        pygame.display.flip()
        pygame.display.set_caption('2048 for liying - score: %d  high score: %d' % (myBoard.score, myBoard.high_score))
        myBoard.print_board()
        test = myBoard.__deepcopy__()
        if not (test.shift_vertical(isUp=True)
                or test.shift_vertical(isUp=False)
                or test.shift_horizontal(isLeft=True)
                or test.shift_horizontal(isLeft=False)):
            myBoard.game_over()

        for event in pygame.event.get():
            if event.type == QUIT:
                terminate(myBoard)
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    terminate(myBoard)
                if event.key == K_UP:
                    if myBoard.shift_vertical(isUp=True):
                        myBoard.add_tile()
                if event.key == K_DOWN:
                    if myBoard.shift_vertical(isUp=False):
                        myBoard.add_tile()
                if event.key == K_LEFT:
                    if myBoard.shift_horizontal(isLeft=True):
                        myBoard.add_tile()
                if event.key == K_RIGHT:
                    if myBoard.shift_horizontal(isLeft=False):
                        myBoard.add_tile()

if __name__ == '__main__':
    main()
