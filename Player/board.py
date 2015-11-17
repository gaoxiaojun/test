#!/usr/bin/env python

import os
import sys
import time
import random
import pygame
from pygame.locals import *
from const import *
from main import terminate


class Board(object):

    def __init__(self, dim=4, high_score=0, isTest=False):
        self.dim = 4 if dim < 4 else dim
        self.high_score = high_score
        self.block_size = 4 * 120 / self.dim
        self.px_dim = self.dim * self.block_size + (self.dim - 1) * THICK
        self.scr_h = self.px_dim + 2 * MARGIN_TOP
        self.scr_w = self.px_dim + 2 * MARGIN_LEFT

        self.board = self._blank_board()
        if not isTest:
            self.display = pygame.display.set_mode((self.scr_w, self.scr_h))
            pygame.display.set_caption('2048')
        self.add_tile()
        self.score = 0
        self.add_tile()

    def __deepcopy__(self):
        new = Board(self.dim, isTest=True)
        new.board = self.board
        return new

    def add_tile(self):
        isFull = True
        free = []

        for r, row in enumerate(self.board):
            for c, block in enumerate(row):
                if not block:
                    isFull = False
                    free.append((r, c))

        if isFull:
            self.game_over()

        r, c = random.choice(free)
        self.new_row, self.new_column = r, c

        if self.dim == 4:
            self.board[r][c] = 1 if random.random() < 0.9 else 2
        else:
            if random.random() < 0.7:
                self.board[r][c] = 1
            elif random.random() < 0.9:
                self.board[r][c] = 2
            else:
                self.board[r][c] = 3

    def shift_vertical(self, isUp=True):
        temp = self._blank_board()
        for c in range(self.dim):
            value = -1
            tempr = 0 if isUp else self.dim - 1
            order = range(0, self.dim) if isUp else range(self.dim - 1, -1, -1)
            for r in order:
                block = self.board[r][c]
                if block == value:
                    temp[tempr][c] = value + 1
                    self.score += 2**(value + 1)
                    if self.score > self.high_score:
                        self.high_score = self.score
                    tempr += 1 if isUp else -1
                    value = -1
                elif value == -1:
                    if block:
                        value = block
                elif block != value and block:
                    temp[tempr][c] = value
                    tempr += 1 if isUp else -1
                    value = block
            temp[tempr][c] = value if value > 0 else 0
        if self.board != temp:
            self.board = temp
            return True
        return False

    def shift_horizontal(self, isLeft=True):
        temp = self._blank_board()
        for r in range(self.dim):
            value = -1
            tempc = 0 if isLeft else self.dim - 1
            order = range(0, self.dim) if isLeft else range(self.dim - 1, -1, -1)
            for c in order:
                block = self.board[r][c]
                if block == value:
                    temp[r][tempc] = value + 1
                    self.score += 2**(value + 1)
                    if self.score > self.high_score:
                        self.high_score = self.score
                    tempc += 1 if isLeft else -1
                    value = -1
                elif value == -1:
                    if block:
                        value = block
                elif block != value and block:
                    temp[r][tempc] = value
                    tempc += 1 if isLeft else -1
                    value = block
            temp[r][tempc] = value if value > 0 else 0
        if self.board != temp:
            self.board = temp
            return True
        return False

    def _blank_board(self):
        return [[0 for i in range(self.dim)] for i in range(self.dim)]

    def print_board(self):
        self.display.fill(C_BACKG)
        pygame.draw.rect(self.display, C_WALLS, (MARGIN_LEFT - THICK,
                                                 MARGIN_TOP - THICK,
                                                 self.px_dim + 2 * THICK,
                                                 self.px_dim + 2 * THICK))
        for i in range(self.dim):
            pygame.draw.rect(self.display, C_WALLS, (MARGIN_LEFT,
                                                     MARGIN_TOP + i * self.block_size +
                                                     (i - 1) * THICK,
                                                     self.px_dim, THICK))
            pygame.draw.rect(self.display, C_WALLS, (MARGIN_LEFT + i * self.block_size + (i - 1) * THICK,
                                                     MARGIN_TOP, THICK, self.px_dim))

        self.print_blocks()

    def print_blocks(self):
        for r, row in enumerate(self.board):
            for c, block in enumerate(row):
                pygame.draw.rect(self.display, C_MAP[block], (MARGIN_LEFT + c * (self.block_size + THICK),
                                                              MARGIN_TOP + r *
                                                              (self.block_size + THICK),
                                                              self.block_size, self.block_size))
                if block:
                    length = len(str(2 ** block))
                    textMaker = pygame.font.Font(os.path.join(
                        sys.path[0], "clearsans.ttf"), int(FONT_FACTOR[length] * self.block_size))
                    if r == self.new_row and c == self.new_column:
                        textColor = C_BRRED
                    else:
                        textColor = C_LTTXT if block > 2 else C_DRKTX
                    text = textMaker.render(str(2 ** block), 1, textColor)
                    textBox = text.get_rect()
                    textBox.center = (MARGIN_LEFT + c * (self.block_size + THICK) + self.block_size * 0.5,
                                      MARGIN_TOP + r * (self.block_size + THICK) + self.block_size * 0.5)
                    self.display.blit(text, textBox)

    def game_over(self):
        textMaker = pygame.font.Font(
            os.path.join(sys.path[0], "clearsans.ttf"), 100)
        surface = pygame.Surface((self.scr_w, self.scr_h))
        surface.set_alpha(128)
        surface.fill(C_BACKG)
        self.display.blit(surface, (0, 0))

        text = textMaker.render("Game Over!", 1, C_BLACK)
        textBox = text.get_rect()
        textBox.center = (self.scr_w / 2, self.scr_h / 2)
        self.display.blit(text, textBox)

        pygame.display.flip()

        time.sleep(3)
        terminate(self)
