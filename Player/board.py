#!/usr/bin/env python

import os
import sys
import time
import random
import pygame
from pygame.locals import *
from const import *

class Board(object):
    def __init__(self,isTest=False):
        self.board = self._blank_board()
        if not isTest:
            self.display = pygame.display.set_mode((SCR_W, SCR_H))
            pygame.display.set_caption('2048')
        self.spawn_piece()
        self.score = 0
        self.spawn_piece()

    def __deepcopy__(self):
        new = Board(isTest=True)
        new.board = self.board
        return new

    def spawn_piece(self):
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
        self.board[r][c] = 1 if random.random() < 0.9 else 2

    def shift_vertical(self, isUp=True):
        temp = self._blank_board()
        for c in range(DIM):
            value = -1
            tempr = 0 if isUp else DIM-1
            order = range(0,DIM) if isUp else range(DIM-1,-1,-1)
            for r in order:
                block = self.board[r][c]
                if block == value:
                    temp[tempr][c] = value + 1
                    self.score += 2**(value+1)
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
        for r in range(DIM):
            value = -1
            tempc = 0 if isLeft else DIM-1
            order = range(0,DIM) if isLeft else range(DIM-1,-1,-1)
            for c in order:
                block = self.board[r][c]
                if block == value:
                    temp[r][tempc] = value + 1
                    self.score += 2**(value+1)
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
        return [[0 for i in range(DIM)] for i in range(DIM)]

    def print_board(self):
        self.display.fill(C_BACKG)
        pygame.draw.rect(self.display, C_WALLS, (MARGIN_LEFT - THICK, 
                                                 MARGIN_TOP - THICK,
                                                 PX_DIM + 2 * THICK,
                                                 PX_DIM + 2 * THICK))
        for i in range(DIM):
            pygame.draw.rect(self.display, C_WALLS, (MARGIN_LEFT,
                                                     MARGIN_TOP + i * BLOCK_SIZE + (i - 1) * THICK,
                                                     PX_DIM, THICK))
            pygame.draw.rect(self.display, C_WALLS, (MARGIN_LEFT + i * BLOCK_SIZE + (i - 1) * THICK,
                                                     MARGIN_TOP, THICK, PX_DIM))

        self.print_blocks()

    def print_blocks(self):
        for r, row in enumerate(self.board):
            for c, block in enumerate(row):
                pygame.draw.rect(self.display, C_MAP[block], (MARGIN_LEFT + c * (BLOCK_SIZE + THICK),
                                                              MARGIN_TOP + r * (BLOCK_SIZE + THICK),
                                                              BLOCK_SIZE, BLOCK_SIZE))
                if block:
                    length = len(str(2 ** block))
                    textMaker = pygame.font.Font(os.path.join(sys.path[0], "clearsans.ttf"), FONT_SIZE[length])
                    textColor = C_LTTXT if block > 2 else C_DRKTX
                    text = textMaker.render(str(2 ** block), 1, textColor)
                    textBox = text.get_rect()
                    textBox.center = (MARGIN_LEFT + c * (BLOCK_SIZE + THICK) + BLOCK_SIZE * 0.5,
                                      MARGIN_TOP + r * (BLOCK_SIZE + THICK) + BLOCK_SIZE * 0.5)  
                    self.display.blit(text, textBox)
                
    def game_over(self):
        textMaker = pygame.font.Font(os.path.join(sys.path[0], "clearsans.ttf"), 100)
        surface = pygame.Surface((SCR_W, SCR_H))
        surface.set_alpha(128)
        surface.fill(C_BACKG)
        self.display.blit(surface, (0, 0))

        text = textMaker.render("Game Over!", 1, C_BLACK)
        textBox = text.get_rect()
        textBox.center = (SCR_W / 2, SCR_H / 2)
        self.display.blit(text, textBox)

        pygame.display.flip()
        
        time.sleep(3)
        while True:
            for event in pygame.event.get():
                if event.type == KEYDOWN or event.type == QUIT:
                    pygame.quit()
                    sys.exit()
