import copy
import random
import time
from threading import Thread

import imageio
import numpy as np

GIF_FPS = 4

taux_reaction_a = 0.04
taux_reaction_i = 0.002
vitesse_diffusion_a = 4
vitesse_diffusion_i = 25
taux_resorption = 0.35
seuil_activation = 122
taux_diffusion = 0.1

max_tick = 30
size = 300
mult = 5


class Cell:
    def __init__(self, a=None, i=None, c=None):
        self.a = a or random.randint(1, 100)
        self.i = i or random.randint(1, 100)
        self.color = c or [255, 255, 255]

    def reagir(self):
        old_i = self.i
        self.i += taux_reaction_i * self.a ** 2
        self.a += (taux_reaction_a * self.a ** 2) / old_i

    def get_a(self):
        return self.a

    def get_i(self):
        return self.i

    def diffuse_a(self, a_diff):
        self.a += a_diff * taux_diffusion

    def diffuse_i(self, i_diff):
        self.i += i_diff * taux_diffusion

    def resorber(self):
        self.a *= 1 - taux_resorption
        self.i *= 1 - taux_resorption

    def set_color(self, color):
        self.color = color

    def get_color(self):
        return self.color

    def __str__(self) -> str:
        return "a={a} i={i}".format(a=self.a, i=self.i)

    def __copy__(self):
        return type(self)(self.a, self.i, self.color)

    def __deepcopy__(self, memodict=None):
        copy_object = type(self)(self.a, self.i, self.color)
        return copy_object


def reagir(board):
    for l in board:
        for c in l:
            c.reagir()


def diffuser(board):
    for a_it in range(max(vitesse_diffusion_a, vitesse_diffusion_i)):
        print("\tdiffuse {i}".format(i=a_it))
        new_board = copy.deepcopy(board)
        for x in range(len(board)):
            for y in range(len(board[x])):
                cell = board[x][y]
                for dx in [-1, 0, 1]:
                    for dy in [-1, 0, 1]:
                        if dx != 0 or dy != 0:
                            if a_it < vitesse_diffusion_a:
                                new_board[(x + dx) % size][(y + dy) % size].diffuse_a(cell.get_a() / 8)
                            if a_it < vitesse_diffusion_i:
                                new_board[(x + dx) % size][(y + dy) % size].diffuse_i(cell.get_i() / 8)
        board = new_board

    return board


def resorber(board):
    for l in board:
        for c in l:
            c.resorber()


def seuiller(board):
    moy = 0
    for l in board:
        for c in l:
            moy += seuil_activation * c.get_a()
    moy /= size ** 2
    for l in board:
        for c in l:
            val = seuil_activation * c.get_a()
            if moy * 0.2 >= val:
                c.set_color([47, 244, 238])
            elif moy * 0.4 >= val:
                c.set_color([95, 234, 222])
            elif moy * 0.6 >= val:
                c.set_color([143, 223, 205])
            elif moy * 0.8 >= val:
                c.set_color([190, 213, 189])
            elif moy * 1.0 >= val:
                c.set_color([238, 203, 173])
            elif moy * 1.2 >= val:
                c.set_color([141, 171, 142])
            elif moy * 1.4 >= val:
                c.set_color([244, 139, 111])
            elif moy * 1.6 >= val:
                c.set_color([248, 107, 80])
            elif moy * 1.8 >= val:
                c.set_color([251, 75, 49])
            elif val >= moy * 1.8:
                c.set_color([255, 44, 19])


def _from_rgb(rgb):
    return "#%02x%02x%02x" % (rgb[0], rgb[1], rgb[2])


def tracerUI(board, canvas):
    if canvas:
        for x in range(len(board)):
            for y in range(len(board[x])):
                canvas.create_rectangle(x * mult, y * mult, (x + 1) * mult, (y + 1) * mult, fill=_from_rgb(board[x][y].get_color()))


def tracerGIF(board, writer):
    data = np.zeros((size, size, 3), dtype=np.uint8)
    for x in range(len(board)):
        for y in range(len(board[x])):
            data[x][y] = board[x][y].get_color()
    writer.append_data(data)


class Ticker(Thread):
    def __init__(self, canvas):
        super().__init__()
        self.canvas = canvas

    def run(self):
        board = [[Cell() for y in range(size)] for x in range(size)]
        tick = 0
        with imageio.get_writer('{t}.gif'.format(t=time.time()), mode='I', fps=GIF_FPS) as writer:
            while tick < max_tick:
                print("tick={t} reagir".format(t=tick + 1))
                reagir(board)
                print("tick={t} diffuser".format(t=tick + 1))
                board = diffuser(board)
                print("tick={t} resorber".format(t=tick + 1))
                resorber(board)
                print("tick={t} seuiller".format(t=tick + 1))
                seuiller(board)
                tick += 1
                print("tick={t} tracer".format(t=tick + 1))
                tracerUI(board, self.canvas)
                tracerGIF(board, writer)


def main():
    # fenetre = Tk()

    canvas = None
    # canvas = Canvas(fenetre, width=mult * size, height=mult * size, background='white')
    # canvas.pack()

    ticker = Ticker(canvas)
    ticker.start()

    # fenetre.mainloop()


if __name__ == "__main__":
    main()
