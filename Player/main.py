import sys
import os
import pickle
import pygame
from pygame.locals import *
import board


def terminate(board):
    with open('2048.ini', 'w') as f:
        f.write('Board=%d\n' % board.dim)
        f.write('HighScore=%d\n' % board.high_score)

    with open('2048.dat', 'w') as f:
        saved = []
        saved.append(board.board)
        saved.append(board.score)
        pickle.dump(saved, f)

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

    try:
        with open('2048.dat') as f:
            saved = pickle.load(f)
    except IOError:
        saved = None

    board_2048 = board.Board(dim, high_score, saved)

    while 1:
        pygame.display.flip()
        pygame.display.set_caption(
            '2048 for liying - score: %d  high score: %d' % (board_2048.score, board_2048.high_score))
        board_2048.print_board()
        if board_2048.is_game_over():
            board_2048.game_over()

        for event in pygame.event.get():
            if event.type == QUIT:
                terminate(board_2048)
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    terminate(board_2048)
                if event.key == K_UP:
                    if board_2048.shift_vertical():
                        board_2048.add_tile()
                if event.key == K_DOWN:
                    if board_2048.shift_vertical(isUp=False):
                        board_2048.add_tile()
                if event.key == K_LEFT:
                    if board_2048.shift_horizontal():
                        board_2048.add_tile()
                if event.key == K_RIGHT:
                    if board_2048.shift_horizontal(isLeft=False):
                        board_2048.add_tile()
            elif event.type == VIDEORESIZE:
                board_2048.set_size(event.size)


if __name__ == '__main__':
    main()
