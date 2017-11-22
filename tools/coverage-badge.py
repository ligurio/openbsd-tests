#!/usr/bin/env python

# https://code.activestate.com/recipes/325823-draw-svg-images-in-python/

import os
import random

display_prog = 'display'  # Command to execute to display images.


class Scene:
    def __init__(self, name="svg", height=400, width=400):
        self.name = name
        self.items = []
        self.height = height
        self.width = width
        return

    def add(self, item): self.items.append(item)

    def strarray(self):
        var = ["<?xml version=\"1.0\"?>\n",
               "<svg height=\"%d\" width=\"%d\" >\n" % (
                   self.height, self.width),
               " <g style=\"fill-opacity:1.0; stroke:black;\n",
               "  stroke-width:1;\">\n"]
        for item in self.items:
            var += item.strarray()
        var += [" </g>\n</svg>\n"]
        return var

    def write_svg(self, filename=None):

        if filename:
            self.svgname = filename
        else:
            self.svgname = self.name + ".svg"
        file = open(self.svgname, 'w')
        file.writelines(self.strarray())
        file.close()
        return

    def display(self, prog=display_prog):
        os.system("%s %s" % (prog, self.svgname))
        return


class Line:
    def __init__(self, start, end, color, width, stroke):
        self.start = start  # xy tuple
        self.end = end  # xy tuple
        self.color = color
        self.stroke = stroke
        self.width = width

        return

    def strarray(self):
        return ["  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"fill: %s; stroke: %s; stroke-width: %s\" />\n" %
                (self.start[0], self.start[1], self.end[0], self.end[1], self.color, self.stroke, self.width)]
        # return ["  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n" %
        #        (self.start[0], self.start[1], self.end[0], self.end[1])]


class Rectangle:
    def __init__(self, origin, height, width, color):
        self.origin = origin
        self.height = height
        self.width = width
        self.color = color
        return

    def strarray(self):
        return ["  <rect x=\"%d\" y=\"%d\" height=\"%d\"\n" %
                (self.origin[0], self.origin[1], self.height),
                "    width=\"%d\" style=\"fill:%s;\" />\n" %
                (self.width, colorstr(self.color))]


def colorstr(rgb): return "#%x%x%x" % (rgb[0] / 16, rgb[1] / 16, rgb[2] / 16)


def test():
    height = 20
    width = 700
    start_x = 0
    start_y = 0

    scene = Scene('coverage', height, width)
    scene.add(Rectangle((start_x, start_y), start_x +
                        width, start_y + height, (255, 255, 255)))

    fill = ""
    # line_width = (rectangle width)/(number of files)
    line_width = 3
    line_numbers = width
    for l in range(1, line_numbers):
        color = random.randint(0, 2)
        if color == 0:
            fill = "red"
        elif color == 1:
            fill = "green"
        elif color == 2:
            fill = "white"
        scene.add(Line((start_x + l, start_y), (start_x + l,
                                                start_y + height), "red", line_width, fill))

    scene.write_svg()
    try:
        os.environ["DISPLAY"]
    except KeyError:
        pass
    finally:
        scene.display()

    return


if __name__ == '__main__':
    test()
